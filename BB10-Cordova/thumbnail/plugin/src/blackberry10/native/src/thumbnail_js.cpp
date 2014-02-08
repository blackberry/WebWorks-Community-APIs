/*
 * Copyright (c) 2013 BlackBerry Limited
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

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "thumbnail_js.hpp"
#include "thumbnail_ndk.hpp"
#include "../public/tokenizer.h"
#include <vector>
#include <errno.h>
using namespace std;

/**
 * Default constructor.
 */
ThumbnailJS::ThumbnailJS(const string& id) {
	m_pLogger = new webworks::Logger("ThumbnailJS", this);
	m_pTemplateController = new webworks::ThumbnailNDK();
	m_id = id;
}

/**
 * Memory destructor.
 */
ThumbnailJS::~ThumbnailJS() {
	if (m_pTemplateController)
		delete m_pTemplateController;
	if (m_pLogger)
		delete m_pLogger;
}

webworks::Logger* ThumbnailJS::getLog() {
	return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
    static char name[] = "ThumbnailJS";
    return name;
}

/**
 * This method is used by JNext to instantiate the Memory object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "ThumbnailJS") {
        return new ThumbnailJS(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool ThumbnailJS::CanDelete() {
    return true;
}

// Notifies JavaScript of an event
void ThumbnailJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string ThumbnailJS::InvokeMethod(const string& command) {
	vector<string> cmdTokenized;
	tokenize(command, cmdTokenized, " ");
	string strCommand = cmdTokenized.at(0);
	string path = cmdTokenized.at(1);
    if (strCommand == "createThumbNailNative") {
    	string width = cmdTokenized.at(2);
    	string height = cmdTokenized.at(3);
    	string quality = cmdTokenized.at(4);
    	//return "createThumbNailNative";
        return m_pTemplateController->createThumbNail(path, atoi(width.c_str()), atoi(height.c_str()), atoi(quality.c_str()), IMG_W | IMG_H);
    }
    else if(strCommand == "createThumbNailWidthNative"){
    	//return cmdTokenized.at(0) + "\t" + cmdTokenized.at(1) + "\t" + cmdTokenized.at(2) + "\t" + cmdTokenized.at(3);
    	string stringWidth = cmdTokenized.at(2);
    	string quality = cmdTokenized.at(3);
    	int width = atoi(stringWidth.c_str());
    	//return "createThumbNailWidthNative";
    	return m_pTemplateController->createThumbNail(path, width , width, atoi(quality.c_str()), IMG_W);
    }
    else if(strCommand == "createThumbNailHeightNative"){
    	string stringHeight = cmdTokenized.at(2);
    	string quality = cmdTokenized.at(3);
    	int height = atoi(stringHeight.c_str());
    	//return "createThumbNailHeightNative";
    	return m_pTemplateController->createThumbNail(path, height , height, atoi(quality.c_str()), IMG_H);
    }
    else{
        return "Unsupported Method";
    }
}
