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

#include "QtBridge.hpp"
#include "QJnextMainLoop.hpp"
#include "ApplicationThread.hpp"

#include <QThread>
#include <QObject>

#define jnextERROR         "Error "
#define jnextOK            "Ok "
#define jnextDISPOSE       "Dispose"
#define jnextINVOKE        "InvokeMethod"
#define jnextCREATE        "CreateObj"

#ifdef DEBUG_QtBridge
#define DEBUG_QtBridgeCpp
#endif

namespace bb {
namespace webworks {
namespace extensions {

class UserData: public QObjectUserData {
public:
	UserData(int id, void * context) :
			id(id), context(context) {
	}
	virtual ~UserData() {
	}

	int id;
	void* context;
};

QtBridge::QtBridge(void* context, uint id,
		void (*&sendEventFunc)(const char*, void*)) :
		id(id), idBase(1), SendEvent(sendEventFunc), context(context) {
	static uint userDataId = QObject::registerUserData();
	this->userDataId = userDataId;

#ifdef DEBUG_QtBridgeCpp
	qDebug() << "[QtBridge] QtBridge()" << id << QThread::currentThreadId()
			<< QThread::currentThread();
#endif

	thread = new ApplicationThread(QThread::LowPriority);
	//QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	mainLoop = new QJnextMainLoop(this);
	mainLoop->moveToThread(thread);
	QMetaObject::invokeMethod(mainLoop, "init", Qt::QueuedConnection);
}

QtBridge::~QtBridge() {
	QMetaObject::invokeMethod(mainLoop, "cleanup",
			Qt::BlockingQueuedConnection);
	thread->wait(50);

	delete thread;
#ifdef DEBUG_QtBridgeCpp
	qDebug() << "[QtBridge] ~QtBridge()";
#endif
}

QVariantList QtBridge::parseMessage(const QByteArray& message,
		QByteArray &errorMessage) {
	QVariant value = jda.loadFromBuffer(message);
	if (jda.hasError())
		errorMessage = QByteArray("JSON Parse Error: ")
				+ jda.error().errorMessage().toLocal8Bit();
	else if (!value.canConvert<QVariantList>()) {
		if (value.canConvert<QByteArray>())
			return (QVariantList() << value);
		errorMessage = QByteArray(
				"JSON Parse Error: could not convert to list");
	} else
		return value.value<QVariantList>();
	return QVariantList();
}

QByteArray QtBridge::invokeFunction(uint id, const QByteArray& message) {
#ifdef DEBUG_QtBridgeCpp
	qDebug() << "[QtBridge] invokeFunction" << id << message;
#else
	Q_UNUSED(id);
#endif

	QByteArray err;
	QVariantList args = parseMessage(message, err);
	if (args.size() < 2) {
		return ((err.size() ? err.constData() : "JSON value error"));
	}
#ifdef DEBUG_QtBridgeCpp
	qDebug() << "ARGS" << args;
#endif

	QByteArray result;
	bool success = QMetaObject::invokeMethod(mainLoop, "invokeFunction",
			Qt::BlockingQueuedConnection, Q_ARG(QVariantList, args),
			Q_ARG(QByteArray*, &result));
	if (!success)
		result = "Method Call Failed";

	return result;
}

QByteArray QtBridge::takeParam(QByteArray& src, const QByteArray& sep) {
	const char * srcChar = src.constData();
	int sz = src.size();
	int index = src.indexOf(sep);
	if (index < 0)
		index = sz;
	QByteArray result = QByteArray::fromRawData(srcChar, index);
	index += sep.size();
	if (index >= sz)
		index = sz;
	src = QByteArray::fromRawData(srcChar + index, sz - index);
	return result;
}

QByteArray QtBridge::takeLastParam(QByteArray& src, const QByteArray& sep) {
	const char * srcChar = src.constData();
	int sz = src.size();
	int index = src.lastIndexOf(sep);
	if (index < 0)
		index = sz;
	src = QByteArray::fromRawData(srcChar, index);
	index += sep.size();
	if (index >= sz)
		index = sz;
	return QByteArray::fromRawData(srcChar + index, sz - index);
}

void QtBridge::sendMessage(const QByteArray& event, const QVariant& message) {
	QByteArray data;
	jda.saveToBuffer(message, &data);
	if (jda.hasError())
		data = "JsonDataAccess Error";

	QByteArray messageStr = QByteArray::number(id) + " " + event + " " + data;
#ifdef DEBUG_QtBridgeCpp
	qDebug() << "[QtBridge.sendMessage]\t" << messageStr;
#endif

	SendEvent(messageStr.data(), context);
}

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
