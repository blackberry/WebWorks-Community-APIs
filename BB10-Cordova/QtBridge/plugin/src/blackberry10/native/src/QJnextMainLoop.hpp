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

#ifndef QJNEXTMAINLOOP_HPP_
#define QJNEXTMAINLOOP_HPP_

#include <bb/data/JsonDataAccess>

#include <QDeclarativeEngine>
#include <QVariantMap>
#include <QByteArray>
#include <QObject>

namespace bb {
namespace webworks {
namespace extensions {

class QtBridge;
class SignalHandler;

class QJnextMainLoop : public QObject {
	Q_OBJECT
public:
	QJnextMainLoop(QtBridge* bridge);
	virtual ~QJnextMainLoop();

public slots:
	void init();
	void cleanup();

	void createObjectFromDocument(QVariantList& args, QByteArray* result);
	void createObject(QVariantList& args, QByteArray* result);
	void deleteObject(uint id, QByteArray* result);


	void invokeFunction(QVariantList args, QByteArray* result);

	void sendMessage(const QByteArray& event, const QVariant& message);
	void sendEvent(const QByteArray& event, const QVariant& message);

	void introspect(QObject *object, QVariantList& args, QByteArray* retval);
	void listNamespace(QVariantList& args, QByteArray* retval);

	void call(QObject *object, QVariantList& args, QByteArray* retval);
	void get(QObject *object, QVariantList& args,
			QByteArray* retval);
	void set(QObject *object, QVariantList& args,
				QByteArray* retval);

	void connect(QObject *object, uint id, QVariantList& args, QByteArray *retval);
	void disconnect(QObject *object, uint id, QVariantList& args, QByteArray *retval);

private:
	void continueLoadingComponent(QDeclarativeComponent* component, QByteArray* result);

private:
	static uint userDataId;
	uint idBase;
	QtBridge *bridge;
	QDeclarativeEngine *declarativeEngine;
	QHash<uint, QObject*> objects;
	QHash<QByteArray, SignalHandler*> signalHandlers;
};

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
#endif /* QJNEXTMAINLOOP_HPP_ */
