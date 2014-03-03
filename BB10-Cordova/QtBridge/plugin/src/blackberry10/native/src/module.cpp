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

#include <QMetaObject>
#include <QVariantMap>
#include <QStringList>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDebug>

#include <string.h>

#include "QtBridge.hpp"

extern "C" {
#include <slog2.h>
}

#define MODULE_NAME "Qt"

#ifdef DEBUG_QtBridge
#define DEBUG_Module
#endif

/*	BEGIN JNEXT COMPAT	*/

#define jnextERROR         "Error "
#define jnextOK            "Ok "

#define jnextDISPOSE       "Dispose"
#define jnextINVOKE        "InvokeMethod"
#define jnextCREATE        "CreateObj"

/*	END JNEXT COMPAT */

/*	JNEXT Plugin API External Symbols	*/
#pragma GCC visibility push(default)
extern "C" {
char* SetEventFunc(void (*funcPtr)(const char*, void*));
char* InvokeFunction(const char* szCommand, void* pContext);
}
#pragma GCC visibility pop

static char* invokeFunction(const char* szCommand, void* pContext);

static QHash<void*, bb::webworks::extensions::QtBridge*> QtBridges;
static void (*JnextSendEvent)(const char*, void*) = NULL;

static QByteArray buffer;

static void initialize_slogger2();

char* SetEventFunc(void (*funcPtr)(const char*, void*)) {
	static char * name = NULL;
	if (name == NULL) {
		name = (char*) MODULE_NAME;
		initialize_slogger2();
#ifdef DEBUG_Module
		qDebug() << "[QWebWorksExtensions] init()";
#endif
	}
	JnextSendEvent = funcPtr;
	return name;
}
char* InvokeFunction(const char* szCommand, void* pContext) {
	if (JnextSendEvent == NULL) {
		static char * error = (char*) "JnextSendEvent == NULL";
		return error;
	}
#ifdef DEBUG_Module
	qDebug() << "[MODULE.extern] InvokeFunction()" << szCommand << pContext;
#endif
	return invokeFunction(szCommand, pContext);
}

void initialize_slogger2() {
	static slog2_buffer_config_t slog2_buffer_config = { MODULE_NAME,
			SLOG2_MAX_BUFFERS };
	static slog2_buffer_set_config_t slog2_config = { 1, MODULE_NAME,
			SLOG2_DEBUG1, { slog2_buffer_config } };

	static slog2_buffer_t slog2_buffers[1];
	slog2_register(&slog2_config, slog2_buffers, 0);
	slog2_set_default_buffer(slog2_buffers[0]);
}

#define EXTRA_DEBUG
static inline char * INVOKE_RETURN(const QByteArray& data) {
#if defined(DEBUG_Module) && defined(EXTRA_DEBUG)
	qDebug() << "[Module.invokeReturn]" << data.left(24).replace("\n", "");
#endif
	buffer = data;
	return (char*) buffer.constData();
}
#define takeParam bb::webworks::extensions::QtBridge::takeParam

static char* invokeFunction(const char* szCommand, void* pContext) {
#ifdef DEBUG_Module
	qDebug() << "[MODULE]" << szCommand << pContext;
#endif
	QByteArray jnextMessage = QByteArray::fromRawData(szCommand,
			strlen(szCommand));
	QByteArray command = takeParam(jnextMessage, " ");
	QByteArray param = takeParam(jnextMessage, " ");

	if (command.length() == 0 || param.length() == 0) {
		qWarning() << "[Module] Invalid command:" << szCommand;
		QByteArray result = QByteArray(jnextERROR) + "Invalid command: "
				+ szCommand;
		return INVOKE_RETURN(result);
	}
#ifdef DEBUG_Module
	qDebug() << "[Module.invokeFunction]" << command << param << jnextMessage;
#endif

	if (command == jnextCREATE) {
		QByteArray bId = takeParam(jnextMessage, " ");
		bool idOk = false;
		uint id = bId.toUInt(&idOk);
		if (!idOk || bId.size() == 0) {
			qWarning() << "[Module] Invalid command:" << szCommand;
			QByteArray result = QByteArray(jnextERROR) + "Invalid command: "
			+ szCommand;
			return INVOKE_RETURN(result);
		}

		/* main object from CreateObj ( "QtBridge" ) */
		bb::webworks::extensions::QtBridge *& qtBridge = QtBridges[pContext];
		if (qtBridge == NULL) {
			qtBridge = new bb::webworks::extensions::QtBridge(pContext, id,
				JnextSendEvent);
		}
		QByteArray result = jnextOK + bId;
		return INVOKE_RETURN(result.data());
	} else if (command != jnextINVOKE)
	return INVOKE_RETURN(QByteArray(jnextERROR) + "Invalid command: " +jnextMessage);

	bb::webworks::extensions::QtBridge * qtBridge = QtBridges.value(pContext,
			NULL);
	bool idOk = false;
	uint id = param.toUInt(&idOk);
	if (!idOk || qtBridge == NULL) {
		return INVOKE_RETURN(
				QByteArray(jnextERROR) + "Invalid command: " + jnextMessage);
	}

	if (command == jnextINVOKE && jnextMessage == jnextDISPOSE) {
		if (id == qtBridge->getId()) {
			delete qtBridge;
			QtBridges.remove(pContext);
			qDebug() << "[Module] ~QtBridge";
		} else {
			qWarning() << "[MODULE] WARNING: delete object not found" << id;
		}
		return INVOKE_RETURN(jnextOK + QByteArray::number(id));
	}

	if (id != qtBridge->getId())
		return INVOKE_RETURN(jnextERROR + QByteArray::number(id));

	return INVOKE_RETURN(qtBridge->invokeFunction(id, jnextMessage));
}
