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

#include "SignalHandler.hpp"
#include "QJnextMainLoop.hpp"

#include <QMetaMethod>

#ifdef DEBUG_QtBridge
#define DEBUG_SignalHandler
#endif

namespace bb {
namespace webworks {
namespace extensions {

SignalHandler::SignalHandler(QJnextMainLoop *bridge, QObject *obj, uint id,
		const QByteArray &sid, const QMetaObject* targetMetaObject,
		const QMetaMethod targetMetaMethod) :
		QObject(obj), __ref(1), id(id), sid(sid), targetMetaMethod(
				targetMetaMethod), bridge(bridge), targetMetaObject(
				targetMetaObject) {

#ifdef Q_CC_BOR
	const int memberOffset = QObject::staticMetaObject.methodCount();
#else
	static const int memberOffset = QObject::staticMetaObject.methodCount();
#endif
	Q_ASSERT(obj);

	sig = targetMetaMethod.signature();

	if (!QMetaObject::connect(obj, targetMetaMethod.methodIndex(), this,
			memberOffset, Qt::DirectConnection, 0)) {
		qWarning(
				"[SignalHandler]\t: QMetaObject::connect returned false. Unable to connect.");
		return;
	}

	/* lets hope that all our enum parameters have the same type as a property on this object
	 * not sure if we really need the recursive search
	 */
	for (const QMetaObject *meta = targetMetaObject; meta != NULL;
			meta = meta->superClass()) {

		for (int i = 0; i < meta->propertyCount(); ++i) {
			QMetaProperty prop = meta->property(i);
			if (prop.isEnumType()) {
				QMetaEnum & enumeration = enumerations[prop.typeName()];
				if (!enumeration.isValid()) {
					enumeration = prop.enumerator();
#ifdef DEBUG_SignalHandler
					qDebug() << "[SignalHandler]\tFoundEnumType: " << prop.typeName();
#endif
				}
			}
		}

	}

}

SignalHandler::~SignalHandler() {

}

int SignalHandler::qt_metacall(QMetaObject::Call call, int methodId, void **a) {
	methodId = QObject::qt_metacall(call, methodId, a);
	if (methodId < 0)
		return methodId;

	if (call == QMetaObject::InvokeMetaMethod) {
		if (methodId == 0) {
			sendEvent(a);
		}
		--methodId;
	}
	return methodId;
}

void SignalHandler::sendEvent(void **a) {
	QVariantList list;
	list << id;

	const QList<QByteArray> &parameterTypes = targetMetaMethod.parameterTypes();
	for (int i = 0; i < parameterTypes.count(); ++i) {
		QVariant param;
		const QByteArray &typeName = parameterTypes.at(i);

		int tp = QMetaType::type(typeName.constData());
		QVariant value(tp, a[i + 1]);

		if (tp == QMetaType::Void || !value.canConvert<QByteArray>()) {
			QMap<QByteArray, QMetaEnum>::const_iterator iEnum =
					enumerations.constFind(typeName);
			if (iEnum == enumerations.constEnd()) {
				qWarning(
						"[SignalHandler]\tDon't know how to handle '%s', use qRegisterMetaType to register it.",
						parameterTypes.at(i).constData());
				continue;
			}

			const void * data = a[i + 1];
			const int * enumValue = static_cast<const int *>(data);
			if (!enumValue) {
				qWarning(
						"[SignalHandler]\tDon't know how to handle enum '%s', patches welcome @SignalHandler.cpp !",
						parameterTypes.at(i).constData());
				continue;
			}

			if (iEnum->isFlag())
				value = iEnum->valueToKeys(*enumValue);
			else
				value = iEnum->valueToKey(*enumValue);
		}

		
		if (!value.isValid())
			qWarning() << "[SignalHandler]\tUnable to un-marshall parameter" << i << "of type" << typeName;
		else
			list << value;
	}

#ifdef DEBUG_SignalHandler
	qDebug() << "[SignalHandler]\t" << sig << "\t" << id << sid << list;
#endif
	bridge->sendEvent(sid,
			QVariant::fromValue(list));
}

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
