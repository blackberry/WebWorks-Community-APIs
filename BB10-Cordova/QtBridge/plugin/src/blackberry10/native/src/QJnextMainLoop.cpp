/*
 * Copyright (c) 2013 BlackBerry Limited
 * Author: Isaac Gordezky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "QJnextMainLoop.hpp"
#include "QtBridge.hpp"
#include "SignalHandler.hpp"
#include "ApplicationThread.hpp"

#include "QtDeclarativePrivate/qdeclarativemetatype_p.hpp"

#include "CustomModules.hpp"

#include <QMetaObject>
#include <QMetaMethod>
#include <QDeclarativeComponent>
#include <QThread>
#include <QVariantMap>
#include <QVariantList>
#include <QVariant>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QCoreApplication>

#include <bb/Application>
#include <bb/data/JsonDataAccess>

#ifdef DEBUG_QtBridge
#define DEBUG_QJnextMainLoop
#endif

/*   Notes on QByteArray usage here
 *
 *   * QByteArray(const char*, int) performs a deep-copy (you get a null-terminated string?)
 *   * QByteArray::fromRawBytes(const char*, int) does not copy the data, but takes a
 *     reference. It behaves like QStringRef for strings. Ensure the const char* data is
 *     not destroyed while the returned QByteArray exists
 */

namespace bb {
namespace webworks {
namespace extensions {

class QJnextUserData: public QObjectUserData {
	uint m_id;
public:
	QHash<QByteArray, SignalHandler*> signalHandlers;

	QJnextUserData(uint id) :
			m_id(id) {
	}
	virtual ~QJnextUserData() {
#ifdef DEBUG_QJnextMainLoop
		qDebug() << "[QJnextMainLoop]\t~QJnextUserData" << signalHandlers.size();
#endif
	}

	inline uint id() {
		return m_id;
	}

	static QJnextUserData* cast(QObjectUserData* userdata) {
		return dynamic_cast<QJnextUserData*>(userdata);
	}
};

uint QJnextMainLoop::userDataId = QObject::registerUserData();

QJnextMainLoop::QJnextMainLoop(QtBridge *bridge) :
		QObject(), idBase(1), bridge(bridge), declarativeEngine(NULL) {
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tQJnextMainLoop()";
#endif
}

QJnextMainLoop::~QJnextMainLoop() {
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\t~QJnextMainLoop";
#endif
}

void QJnextMainLoop::init() {
	registerCustomTypes();

	declarativeEngine = new QDeclarativeEngine(this);

#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tinit()" << QThread::currentThreadId();
#endif
}

void QJnextMainLoop::cleanup() {
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tcleanup()" << QThread::currentThreadId();
#endif
	qDeleteAll(objects.values());
	deleteLater();
}

void QJnextMainLoop::invokeFunction(QVariantList args, QByteArray* result) {
	QByteArray command = args.takeFirst().toByteArray();

	if (command == "createObject") {
		createObject(args, result);
		return;
	} else if (command == "createObjectFromDocument") {
		createObjectFromDocument(args, result);
		return;
	} else if (command == "listNamespace") {
		listNamespace(args, result);
		return;
	} else if (command == "join") {
		bridge->applicationThread()->join(args.takeFirst().toByteArray());
		return;
	}


	QVariant vId = args.takeFirst();
	bool ok;
	uint id = vId.toUInt(&ok);
	QObject *object = objects.value(id, NULL);
	if (!ok || object == NULL) {
		*result = "Unknown object with id " + vId.toByteArray();
		qWarning() << *result;
		return;
	}

	if (command == "deleteObject") {
		deleteObject(id, result);
	} else if (command == "call") {
		call(object, args, result);
	} else if (command == "get") {
		get(object, args, result);
	} else if (command == "set") {
		set(object, args, result);
	} else if (command == "introspect") {
		args.push_front(vId);
		introspect(object, args, result);
	} else if (command == "connect") {
		connect(object, id, args, result);
	} else if (command == "disconnect") {
		disconnect(object, id, args, result);
	} else {
		*result = "Unknown command " + command;
		qWarning() << *result;
	}
}

void QJnextMainLoop::createObjectFromDocument(QVariantList& args,
		QByteArray* result) {
	QByteArray uri = args.takeFirst().toByteArray();

	Q_UNUSED(result);

	QDeclarativeComponent * component = new QDeclarativeComponent(
			declarativeEngine, uri);
	if (component->isLoading()) {
		*result = "Unimplemented: Network components";
		qWarning() << *result;
		return;

		// TODO: support loading qml documents from the network
		/*QObject::connect(component,
		 SIGNAL(statusChanged(QDeclarativeComponent::Status)), this,
		 SLOT(continueLoadingComponent()));*/
	}

	continueLoadingComponent(component, result);
}

void QJnextMainLoop::continueLoadingComponent(QDeclarativeComponent* component,
		QByteArray* result) {
	if (component->isError()) {
		*result = component->errorString().toLatin1();
		qWarning() << *result;
		component->deleteLater();
		return;
	}

	uint id = idBase++;
	QObject *& object = objects[id];
	if (object != NULL) {
		*result = "Error object already exists for id" + QByteArray::number(id);
		qWarning() << *result;
		component->deleteLater();
		return;
	}

	object = component->create();
	object->setUserData(userDataId, new QJnextUserData(id));
	object->setParent(this);
	component->setParent(object);

	*result = QByteArray::number(id);
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tCreated object" << id;
#endif
}

void QJnextMainLoop::createObject(QVariantList& args, QByteArray* result) {
	uint id = idBase++;
	// this will OWN the backing char array
	QByteArray param = args.takeFirst().toByteArray();
	QByteArray ns(param);
	QByteArray ver = QtBridge::takeLastParam(ns, "@");
	if (ver.size() == 0)
		ver = "1.0";
	QByteArray cls = QtBridge::takeLastParam(ns, ".");
	QString qmlcls = QString(ns) + "/" + cls;

	QObject *& object = objects[id];
	if (object != NULL) {
		*result = "Error object already exists for id" + QByteArray::number(id);
		qWarning() << *result;
		return;
	}

#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tQDeclarative" << "import " + ns + " " + ver + "; " + cls + " {}";
#endif
	QDeclarativeComponent component(declarativeEngine, this);

	component.setData(
			"import " + ns + " " + ver + "; import QtQuick 1.0; QtObject {}",
			QUrl());
	if (component.isError() || !component.isReady()) {
		qWarning() << "COMPONENT ERROR" << component.errorString();
	}

	int majorVersion = -1, minorVersion = -1;
	QDeclarativeType * dtype = NULL;
	QString scls = cls;

	QList<QDeclarativeType*> types = QDeclarativeMetaType::qmlTypes();
	foreach(QDeclarativeType* type, types)
	{
		if (ns == type->module()) {
			QString qmlType = QString::fromLatin1(type->qmlTypeName());

			if (qmlcls.compare(qmlType, Qt::CaseInsensitive) == 0) {
				if (type->majorVersion() > majorVersion
						&& type->minorVersion() > minorVersion) {
					majorVersion = type->majorVersion();
					minorVersion = type->minorVersion();
					dtype = type;
				}
			}
		}
	}
	if (dtype == NULL && ns == "bb" && cls == "Application") {
		object = bb::Application::instance();
		object->setUserData(userDataId, new QJnextUserData(id));
		goto end_createObject;
	}
	if (dtype == NULL) {
		*result = "No such Type " + cls + " in namespace " + ns;
		qWarning() << *result;
		return;
	} else if (!dtype->isCreatable()) {
		*result = "Class " + ns + "." + cls + " is not creatable: "
				+ dtype->noCreationReason().toLatin1();
		qWarning() << *result;
		return;
	}

#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\t" << dtype->typeId() << dtype->typeName()
			<< QString("%1.%2").arg(QString::number(dtype->majorVersion())).arg(
					QString::number(dtype->minorVersion()));
#endif
	object = dtype->create();
	if (!object) {
		qWarning() << "COMPONENT.CREATE ERROR";
		*result = "Error creating component";
		return;
	}
	object->setUserData(userDataId, new QJnextUserData(id));
	object->setParent(this);

end_createObject:
	*result = QByteArray::number(id);
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tCreated object" << id;
#endif
}

void QJnextMainLoop::deleteObject(uint id, QByteArray* result) {
	QObject *object = objects.take(id);
	if (object != bb::Application::instance())
		object->deleteLater();
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tObject" << id << "destroyed";
#endif
	*result = "";
}

#define _Q_RETURN_ARG(method, returnValue) QGenericReturnArgument(method->typeName(), returnValue.data())
#define _Q_ARG(argTypes, args, idx) QGenericArgument(argTypes.value(idx), args.value(idx).constData())

static int doCall(QObject* object, const QMetaMethod *method,
		QVariantList args, QByteArray* retval) {
	QGenericReturnArgument ret(0);
	int success;

	QList<QByteArray> argTypes = method->parameterTypes();
	QVariant returnValue(QVariant::nameToType(method->typeName()));

	for (int i = 0; i < args.size(); ++i) {
		if (!args[i].convert(QVariant::nameToType(argTypes[i]))) {
			retval->append(
					"Unable to convert parameter " + QByteArray::number(i) + " "
							+ args.value(i).toString() + " to "
							+ argTypes.value(i));
#ifdef DEBUG_QJnextMainLoop
			qDebug() << "[QJnextMainLoop]\ttcalling" << method->signature() << *retval << args;
#endif
			return -1;
		}
	}

#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\t" << method->signature() << "called with" << args.size() << "args: " << args;
#endif
	success = method->invoke(object, Qt::DirectConnection,
			_Q_RETURN_ARG(method, returnValue), _Q_ARG(argTypes, args, 0),
			_Q_ARG(argTypes, args, 1), _Q_ARG(argTypes, args, 2),
			_Q_ARG(argTypes, args, 3), _Q_ARG(argTypes, args, 4),
			_Q_ARG(argTypes, args, 5), _Q_ARG(argTypes, args, 6),
			_Q_ARG(argTypes, args, 7), _Q_ARG(argTypes, args, 8),
			_Q_ARG(argTypes, args, 9));

#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tcalled" << success << returnValue;
#endif
	*retval = returnValue.toByteArray();
	return success;
}

static inline QByteArray getMethodName(const QMetaMethod& method) {
	QByteArray name(method.signature());
	int bracket = name.indexOf("(");
	if (bracket >= 0)
		name.remove(bracket, name.size() - bracket);
	return name;
}

QList<QMetaMethod> matchMetamethod(QObject *object, QByteArray& methodName,
		QVariantList& args, QByteArray* retval) {
	const QMetaObject* meta = object->metaObject();
	QList<QMetaMethod> methods;
	for (int i = 0; i < meta->methodCount(); ++i) {
		QMetaMethod method = meta->method(i);
		if (QByteArray(method.signature()).startsWith(methodName + "("))
			methods.append(method);
	}
	if (methods.size() == 0) {
		retval->operator =("No such method " + methodName + " ");
		foreach (QVariant var, args)
			retval->append(var.toByteArray() + " ");
		retval->remove(retval->size() - 1, 1);
		return methods;
	}

// TODO: better algorithm
	QList<QMetaMethod> exactMethods;
	for (int i = 0; i < methods.size(); ++i) {
		if (methods[i].parameterNames().size() == args.size()) {
			exactMethods << methods[i];
		}
	}
	if (exactMethods.size() > 0)
		return exactMethods;
	return methods;
}

void QJnextMainLoop::call(QObject *object, QVariantList& args,
		QByteArray* retval) {
	QByteArray methodName = args.takeFirst().toByteArray();

	QList<QMetaMethod> methods = matchMetamethod(object, methodName, args,
			retval);
	if (methods.size() == 0)
		return;

	for (int i = 0; i < methods.size(); ++i) {
		if (doCall(object, &(methods[i]), args, retval) != -1)
			return;
	}

	*retval = "No method with matching parameters for " + methodName + " ";
	foreach (QVariant var, args)
		retval->append(var.toByteArray() + " ");
	retval->remove(retval->size() - 1, 1);
}

void QJnextMainLoop::get(QObject *object, QVariantList& args,
		QByteArray* retval) {
	QByteArray property = args.takeFirst().toByteArray();

	const QMetaObject * meta = object->metaObject();
	int propertyIndex = meta->indexOfProperty(property);
	if (propertyIndex < 0) {
		retval->append("No such property " + property);
		return;
	}

	QMetaProperty metaprop = meta->property(propertyIndex);
	if (!metaprop.isReadable()) {
		retval->append("Property " + property + " is not readable");
		return;
	}

	QVariant value = metaprop.read(object);
	if (value.isNull()) {
		qWarning() << "[QJnextMainLoop]\tNULL value ignored" << object->property(property);
	}
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tVALUE" << value << object->property(property) << metaprop.isEnumType();
#endif

    if (metaprop.isEnumType()) {
        bool ok;
        int enumValue = value.toInt(&ok);
        if (!ok) {
            int status = -1;
            void *argv[] = { 0, &value, &status };
            QMetaObject::metacall(const_cast<QObject*>(object), QMetaObject::ReadProperty,
                    propertyIndex + meta->propertyOffset(), argv);
            const int * enumRaw = static_cast<const int *>(argv[0]);

            if (status == -1 && enumRaw != 0) {
                const QMetaEnum & iEnum = metaprop.enumerator();
                if (iEnum.isFlag())
                    value = iEnum.valueToKeys(*enumRaw);
                else
                    value = iEnum.valueToKey(*enumRaw);
            } else {
                // someone is evil and didn't register enumerations properly
                qDebug() << "[QJnextMainLoop]\t" << "!!!!!!!" << argv[0] << value;
            }
        } else if (metaprop.enumerator().isFlag()) {
            *retval = metaprop.enumerator().valueToKeys(enumValue);
            return;
        } else {
            *retval = metaprop.enumerator().valueToKey(enumValue);
            return;
        }
    }
	*retval = value.toByteArray();
}

void QJnextMainLoop::set(QObject *object, QVariantList& args,
		QByteArray* retval) {
	QByteArray property = args.takeFirst().toByteArray();

	const QMetaObject * meta = object->metaObject();
	int propertyIndex = meta->indexOfProperty(property);
	if (propertyIndex < 0) {
		retval->append("No such property " + property);
		return;
	}

	QMetaProperty metaprop = meta->property(propertyIndex);
	if (!metaprop.isWritable()) {
		retval->append("Property " + property + " is not writable");
		return;
	}
	QVariant vValue = args.takeFirst();

	/* handle enum inputs as text */
	if (metaprop.isEnumType()) {
		int id;
		const QMetaEnum &enumerator = metaprop.enumerator();
		QByteArray keys;
		for (int i = 0; i < enumerator.keyCount(); ++i)
			keys += enumerator.key(i) + QByteArray(" ");
#ifdef DEBUG_QJnextMainLoop
		qDebug() << "[QJnextMainLoop]\tEnumerator" << enumerator.isFlag() << enumerator.scope() << enumerator.name() <<
		keys;
#endif

		if (enumerator.isFlag())
			id = enumerator.keyToValue(vValue.toByteArray().constData());
		else
			id = enumerator.keysToValue(vValue.toByteArray().constData());

		if (id != -1)
			vValue = QVariant(id);
	}
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tSET" << meta->className() << property << vValue
	<< vValue.canConvert(metaprop.type());
#endif

	if (!vValue.convert(metaprop.type())) {
		retval->append(
				"Unable to convert \"" + vValue.toByteArray() + "\" to "
						+ metaprop.typeName());
		return;
	}

	if (!metaprop.write(object, vValue))
		retval->append(
				QByteArray("Unable to set property ") + meta->className() + "."
						+ property + " to " + vValue.toByteArray());
	else
		*retval = QByteArray();
}

void QJnextMainLoop::connect(QObject *object, uint id, QVariantList& args,
		QByteArray *retval) {
	QByteArray methodName = args.takeFirst().toByteArray();
	QByteArray sid = args.takeFirst().toByteArray();
	int argCount = -1;

	const QMetaObject* meta = object->metaObject();
	QMetaMethod method;
	for (int i = 0; i < meta->methodCount(); ++i) {
		method = meta->method(i);
		if (QByteArray(method.signature()).startsWith(methodName + "(")) {
			QList<QByteArray> types = method.parameterTypes();
			if (types.size() > argCount) {
				// TODO : always pick enum types
				argCount = method.parameterTypes().size();
				break;
			}
		}
	}
	if (argCount == -1) {
		*retval = "No signal named " + methodName + " found";
		return;
	}
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tSIGNAL.connect" << methodName << id << sid;
#endif

	QJnextUserData * jnextData = dynamic_cast<QJnextUserData*>(object->userData(
			QJnextMainLoop::userDataId));
	if (jnextData == NULL) {
		*retval = "Unable to retrieve jnextData from object "
				+ QByteArray::number(id);
		return;
	}

	SignalHandler *& handler = jnextData->signalHandlers[sid];
	if (handler != NULL) {
#ifdef DEBUG_QJnextMainLoop
		qDebug() << "[QJnextMainLoop]\tCONNECTED (chained)";
#endif
		return;
	}
	handler = new SignalHandler(this, object, id, "com.blackberry.qt" + sid, meta, method);
	if (!handler->isValid()) {
		*retval = "QMetaObject::connect returned false. Unable to connect";
		return;
	}
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\tCONNECTED";
#endif
}

void QJnextMainLoop::disconnect(QObject *object, uint id, QVariantList& args,
		QByteArray *retval) {
	QByteArray sid = args.takeFirst().toByteArray();
	if (sid.size() == 0) {
		*retval = QByteArray("You must specify a signal id");
		return;
	}
	QJnextUserData * jnextData = dynamic_cast<QJnextUserData*>(object->userData(
			QJnextMainLoop::userDataId));
	if (jnextData == NULL) {
		*retval = "Unable to retrieve jnextData from object " + id;
		return;
	}

	SignalHandler *& handler = jnextData->signalHandlers[sid];
	if (handler == NULL) {
		qWarning() << "No known connection to " + sid;
		*retval = "No know connection to " + sid;
		return;
	}
	signalHandlers.remove(sid);
	handler->unref();
}

void QJnextMainLoop::introspect(QObject *object, QVariantList& args,
		QByteArray* retval) {
	const QMetaObject * meta = object->metaObject();

	QVariantMap data;
	QVariantList properties;
	for (int i = 0; i < meta->propertyCount(); ++i) {
		QVariantMap propData;
		QMetaProperty prop = meta->property(i);

		propData["name"] = prop.name();
		propData["type"] = prop.typeName();
		if (!prop.isReadable()) {
			if (!prop.isWritable())
				continue;
			propData["writeOnly"] = true;
		}
		if (!prop.isWritable())
			propData["readOnly"] = true;
		if (prop.hasNotifySignal())
			propData["notifySignal"] = getMethodName(prop.notifySignal());
		if (prop.isEnumType())
			propData["enumeration"] = true;

		properties.append(QVariant::fromValue(propData));
	}
	data["properties"] = properties;

	QVariantList methods;
	QVariantList sigs;
	for (int i = 0; i < meta->methodCount(); ++i) {
		QVariantMap methodData;
		QMetaMethod method = meta->method(i);

		QByteArray name = getMethodName(method);
		if (method.access() == QMetaMethod::Private || name.startsWith("_") || name == "deleteLater" || name == "destroyed"
				|| name == "creationCompleted")
			continue;
		else if (method.access() == QMetaMethod::Protected && method.methodType() != QMetaMethod::Signal)
		    continue;
		methodData["name"] = name;

		QByteArray rType(method.typeName());
		if (!rType.isEmpty())
			methodData["type"] = rType;

		QList<QByteArray> pTypes = method.parameterTypes();
		QList<QByteArray> pNames = method.parameterNames();

		QVariantList params;
		for (int i = 0; i < pTypes.count(); ++i) {
			QVariantMap tp;
			tp["type"] = pTypes.at(i);
			tp["name"] = pNames.value(i);
			params.append(tp);
		}
		if (params.size() > 0)
			methodData["parameters"] = params;

		switch (method.methodType()) {
		case QMetaMethod::Method:
			//qDebug() << "=== METHOD ===" << methodData << method.signature();
			methods.append(QVariant::fromValue(methodData));
			break;
		case QMetaMethod::Signal:
			sigs.append(QVariant::fromValue(methodData));
			break;
		case QMetaMethod::Constructor:
			methodData["constructor"] = true;
			methods.append(QVariant::fromValue(methodData));
			break;
		case QMetaMethod::Slot:
			methods.append(QVariant::fromValue(methodData));
			break;
		}

	}
	if (methods.size() > 0)
		data["methods"] = methods;
	if (sigs.size() > 0)
		data["signals"] = sigs;

	data["id"] = args.takeFirst();

	bridge->json()->saveToBuffer(QVariant::fromValue(data), retval);
#ifdef DEBUG_QJnextMainLoop
	qDebug() << "[QJnextMainLoop]\t[INTROSPECT]" << retval;
#endif
}

void QJnextMainLoop::listNamespace(QVariantList& args, QByteArray* retval) {
	QByteArray param = args.takeFirst().toByteArray();
	QByteArray ns(param);
	QByteArray ver = QtBridge::takeLastParam(ns, "@");
	if (ver.size() == 0)
		ver = "1.0";

	QDeclarativeComponent component(declarativeEngine, this);

	component.setData(
			"import " + ns + " " + ver + "; import QtQuick 1.0; QtObject {}",
			QUrl());
	if (component.isError() || !component.isReady()) {
		qWarning() << "COMPONENT ERROR" << component.errorString();
	}

	QList<QDeclarativeType*> types = QDeclarativeMetaType::qmlTypes();
	QSet<QString> typeNames;
	foreach(QDeclarativeType* type, types)
	{
#ifdef DEBUG_QJnextMainLoop
		qDebug() << "[QJnextMainLoop]\t" << type->qmlTypeName() << type->majorVersion()
				<< type->minorVersion();
#endif

		if (ns == type->module() && type->isCreatable()) {
			QString qmlType = QString::fromLatin1(type->qmlTypeName());
			typeNames << qmlType.split("/").takeLast();
		}
	}
	QVariantList v;
	foreach (QString name, typeNames) {
		v << name;
	}
	bridge->json()->saveToBuffer(QVariant::fromValue(v), retval);
}

void QJnextMainLoop::sendMessage(const QByteArray& event,
		const QVariant& message) {
	bridge->sendMessage(event, message);
}

void QJnextMainLoop::sendEvent(const QByteArray& event,
		const QVariant& message) {
	bridge->sendMessage(event, message);
}

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
