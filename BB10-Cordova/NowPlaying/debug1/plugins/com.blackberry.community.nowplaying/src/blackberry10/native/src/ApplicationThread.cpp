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

#include "ApplicationThread.hpp"

#include <QEventLoop>
#include <bb/Application>
#include <bb/cascades/Application>
#include <bb/cascades/Window>
#include <bb/cascades/Page>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>

#include <screen/screen.h>
#include <bps/bps.h>

#ifdef DEBUG_NowPlaying
#define DEBUG_ApplicationThread
#endif

/* If anyone has a provably better way to do this I would love a patch !!
 * I left my attempts in for you to consider.
 */

#define APPLICATION_STRATEGY_2

namespace bb {
namespace webworks {
namespace extensions {

ApplicationThread::ApplicationThread(QThread::Priority priority,
		QObject *parent) :
		QThread(parent), app(NULL) {
	int argc = 1;
	char* argv[] = { (char*) "NowPlaying", };

	app = new bb::Application(argc, argv);
#ifdef APPLICATION_STRATEGY_2
	app->moveToThread(this);
#endif
	start(priority);
}

ApplicationThread::~ApplicationThread() {
#ifdef DEBUG_ApplicationThread
	qDebug() << "~ApplicationThread START";
#endif
#if defined(APPLICATION_STRATEGY_2)
	app->quit();
#else
	QThread::quit();
#endif
	wait();
#ifdef DEBUG_ApplicationThread
	qDebug() << "~ApplicationThread" << this->isFinished();
#endif
	if (app) {
		delete (bb::Application*)app;
	}
#ifdef DEBUG_ApplicationThread
	qDebug() << "~ApplicationThread DONE";
#endif
}

void ApplicationThread::join(QByteArray windowGroup) {
	bb::Application *app = dynamic_cast<bb::Application *>(this->app);
	if (app)
		app->setMainWindowGroup(windowGroup);
}

void ApplicationThread::run() {
#ifdef DEBUG_ApplicationThread
	qDebug() << "[THREAD] running" << &app;
#endif

#if defined(APPLICATION_STRATEGY_2)
	app->exec();
#else
	QThread::run();
#endif

#ifdef DEBUG_ApplicationThread
	qDebug() << "[THREAD] event loop returned";
#endif
#if defined(APPLICATION_STRATEGY_2)
	app->moveToThread(thread());
#endif

#ifdef DEBUG_ApplicationThread
	qDebug() << "[THREAD] run complete";
#endif
	return;
}

} /* namespace extensions */
} /* namespace webworks */
} /* namespace bb */
