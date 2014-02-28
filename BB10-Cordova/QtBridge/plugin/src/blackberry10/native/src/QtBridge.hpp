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

#ifndef QTBRIDGE_HPP_
#define QTBRIDGE_HPP_

#include <bb/data/JsonDataAccess>

#include <QVariantList>
#include <QHash>
#include <QByteArray>
#include <QObject>

class QThread;
class QCoreApplication;

namespace bb {
namespace webworks {
namespace extensions {

class QJnextMainLoop;
class ApplicationThread;

class QtBridge {
public:
	QtBridge(void* context, uint id, void (*&sendEventFunc)(const char*, void*));
	virtual ~QtBridge();

	uint getId() { return id; }

	QByteArray invokeFunction(uint id, const QByteArray& message);

	void sendMessage(const QByteArray& event, const QVariant& message);


	static QByteArray takeParam(QByteArray& src, const QByteArray& sep);
	static QByteArray takeLastParam(QByteArray& src, const QByteArray& sep);

	bb::data::JsonDataAccess * json() { return &jda; }

	ApplicationThread* applicationThread() const { return thread; };

private:
	QVariantList parseMessage(const QByteArray& message, QByteArray &errorMessage);

	uint userDataId;
	uint id;
	uint idBase;
	bb::data::JsonDataAccess jda;

	void (*&SendEvent)(const char*, void*);
	void *context;
	ApplicationThread *thread;
	QJnextMainLoop *mainLoop;
};

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */



#endif /* QTBRIDGE_HPP_ */
