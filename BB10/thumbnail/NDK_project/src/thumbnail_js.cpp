/*
* Copyright 2012 Research In Motion Limited.
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
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "thumbnail_js.hpp"
#include "../public/tokenizer.h"
#include <vector>
#include <errno.h>
using namespace std;

/**
 * Default constructor.
 */
ThumbNail::ThumbNail(const std::string& id) : m_id(id) {


}

/**
 * Memory destructor.
 */
ThumbNail::~ThumbNail() {
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
    static char name[] = "ThumbNail";
    return name;
}

/**
 * This method is used by JNext to instantiate the Memory object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "ThumbNail") {
        return new ThumbNail(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool ThumbNail::CanDelete() {
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string ThumbNail::InvokeMethod(const string& command) {
	vector<string> cmdTokenized;
	tokenize(command, cmdTokenized, " ");
	string strCommand = cmdTokenized.at(0);
	string path = cmdTokenized.at(1);
    if (strCommand == "createThumbNailNative") {
    	string width = cmdTokenized.at(2);
    	string height = cmdTokenized.at(3);
    	string quality = cmdTokenized.at(4);
        return createThumbNail(path, atoi(width.c_str()), atoi(height.c_str()), atoi(quality.c_str()), IMG_W | IMG_H);
    }
    else if(strCommand == "createThumbNailWidthNative"){
    	string stringWidth = cmdTokenized.at(2);
    	string quality = cmdTokenized.at(3);
    	int width = atoi(stringWidth.c_str());
    	return createThumbNail(path, width , width, atoi(quality.c_str()), IMG_W);
    }
    else if(strCommand == "createThumbNailHeightNative"){
    	string stringHeight = cmdTokenized.at(2);
    	string quality = cmdTokenized.at(3);
    	int height = atoi(stringHeight.c_str());
    	return createThumbNail(path, height , height, atoi(quality.c_str()), IMG_H);
    }
    else{
        return "Unsupported Method";
    }

}

string ThumbNail::createThumbNail(string path, int width, int height, unsigned int quality, int flag){

	img_lib_t ilib = NULL;
	int rc;
	if ((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
	                fprintf(stderr, "img_lib_attach() failed: %d\n", rc);
	                return "";
	}

	img_t img;
	img.w = width;
	img.h = height;
	img.flags = flag | IMG_QUALITY;
	int resizeResult = img_load_resize_file( ilib, path.c_str(), NULL, &img );

	string tempFileName = convertLongToString(clock()) + path.substr(path.find_last_of("."), path.length());
	string tempFilePath = string(getcwd(NULL, 0)) + "/" + TMP_PATH + tempFileName;
	int writeResult = img_write_file( ilib, tempFilePath.c_str(), NULL, &img );
	img_lib_detach(ilib);

	return tempFilePath;
}
string ThumbNail::convertLongToString(long l) {
	stringstream ss;
	ss << l;
	return ss.str();
}









