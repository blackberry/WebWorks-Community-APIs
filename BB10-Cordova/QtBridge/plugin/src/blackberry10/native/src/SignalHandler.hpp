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

#ifndef SIGNALHANDLER_HPP_
#define SIGNALHANDLER_HPP_

#include <QList>
#include <QMetaObject>
#include <QMetaEnum>
#include <QVariant>
#include <QObject>
#include <QByteArray>

namespace bb {
namespace webworks {
namespace extensions {

class QJnextMainLoop;

class SignalHandler: public QObject {
public:
	SignalHandler(QJnextMainLoop *bridge, QObject *obj, uint id,
			const QByteArray &sid, const QMetaObject* targetMetaObject,
			const QMetaMethod targetMetaMethod);
	virtual ~SignalHandler();

	inline void ref() {
		__ref += 1;
	}
	inline bool unref() {
		if (--__ref == 0) {
			deleteLater();
			return true;
		}
		return false;
	}

	inline bool isValid() const {
		return !sig.isEmpty();
	}
	inline QByteArray signal() const {
		return sig;
	}

	int qt_metacall(QMetaObject::Call call, int methodId, void **a);

private:
	void sendEvent(void **a);

	// the full, normalized signal name
	uint __ref;
	uint id;
	const QByteArray sid;
	QByteArray sig;
	const QMetaMethod targetMetaMethod;
	QMap<QByteArray, QMetaEnum> enumerations;
	QJnextMainLoop *bridge;
	const QMetaObject *targetMetaObject;
};

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
#endif /* SIGNALHANDLER_HPP_ */
