/*
* screenshot_js.hpp
*
* Defines the native side of WebWorks BB10 screenshot extension
*
* https://github.com/jonwebb/webworks-bb10-screenshot
*
* Author: Jon Webb, jon@jonwebb.net
*
* Portions Copyright 2013 Innovatology.
* Portions Copyright 2014 Blackberry Limited.
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

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpp/sstream>
#include <json/reader.h>
#include "screenshot_js.hpp"

#if TRACE==1
	#define trace(x) NotifyTrace(x)
#else
	#define trace(x) ;
#endif

using namespace std;

/**
 * Screenshot constructor
 */
ScreenshotJS::ScreenshotJS(const std::string& id)  {
	m_pLogger = new webworks::Logger("ScreenshotJS", this);
	m_pTemplateController = new webworks::ScreenshotNDK();
	m_id = id;
}

/**
 * Screenshot destructor
 */
ScreenshotJS::~ScreenshotJS() {
	delete m_pTemplateController;
	delete m_pLogger;
}

/**
 * Objects implemented by this extension
 */
char* onGetObjList() {
    static char name[] = "ScreenshotJS";
    return name;
}

/**
 * called by JNext to instantiate our object
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "ScreenshotJS") {
        return new ScreenshotJS(id);
    }

    return NULL;
}

/**
 * called by JNext to figure out whether our object can be deleted
 */
bool ScreenshotJS::CanDelete() {
    return true;
}

/**
 * called by JNext to invoke methods on our object
 */
std::string ScreenshotJS::InvokeMethod(const std::string& command) {
	trace("InvokeMethod");
	trace("command="+command);

	int space;
	std::string strCommand;
	std::string strArgs;
	Json::Reader jsr;
	Json::Value args;

	/* split command and arguments */
	space = command.find_first_of(" ");
	if(space<0)
		return "error:No arguments specified";

	strCommand = command.substr(0, space);
	strArgs = command.substr(space+1, command.length());
	if(!jsr.parse(strArgs, args))
		return "error:parsing JSON arguments";

	/* which method is being invoked? */
	if(strCommand=="hello") {
		return m_pTemplateController->hello();
	} else if (strCommand == "execute") {
    	return m_pTemplateController->execute(args);
    }

    return "error:unsupported method "+strCommand;
}

/**
 * Send an event to JavasCript side
 */
void ScreenshotJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
/**
 * Send a trace event to JavaScript side
 */
#ifdef TRACE
	void ScreenshotJS::NotifyTrace(const std::string& output) {
		std::string traceOutput = "trace: "+output;
		m_pLogger->debug(traceOutput.c_str());
	}
#endif
