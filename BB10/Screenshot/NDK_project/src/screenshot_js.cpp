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
* Portions Copyright 2012 Research In Motion Limited.
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
#include <screen/screen.h>
#include <img/img.h>
#include <bb/utility/ImageConverter.hpp>
#include <bb/core/ImageData.hpp>
#include <json/reader.h>
#include "screenshot_js.hpp"


#if TRACE==1
	#define trace(x) NotifyTrace(x)
#else
	#define trace(x) ;
#endif

using namespace std;

/**
 * misc utility funcs
 */
std::string intToString(int i) {
	stringstream s;
	s<<i;
	return s.str();
}
std::string intToHex(int i) {
	stringstream s;
	s<<std::hex<<i;
	return s.str();
}

/**
 * Screenshot constructor
 */
Screenshot::Screenshot(const std::string& id)  {
	m_id = id;
}

/**
 * Screenshot destructor
 */
Screenshot::~Screenshot() {
}

/**
 * Objects implmemented by this extension
 */
char* onGetObjList() {
    static char name[] = "Screenshot";
    return name;
}

/**
 * called by JNext to instantiate our object
 */
JSExt* onCreateObject(const string& className, const string& id) {
    if (className == "Screenshot") {
        return new Screenshot(id);
    }

    return NULL;
}

/**
 * called by JNext to figure out whether our object can be deleted
 */
bool Screenshot::CanDelete() {
    return true;
}

/**
 * called by JNext to invoke methods on our object
 */
string Screenshot::InvokeMethod(const std::string& command) {
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
		return hello();
	} else if (strCommand == "execute") {
    	return execute(args);
    }

    return "error:unsupported method "+strCommand;
}

/**
 * Send an event to JavasCript side
 */
void Screenshot::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
/**
 * Send a trace event to JavasCript side
 */
#ifdef TRACE
	void Screenshot::NotifyTrace(const std::string& output) {
		NotifyEvent("trace: "+output);
	}
#endif

/* **************************************************************
 * ********************** Implementation ************************
 * **************************************************************
 */

/**
 * hello() returns an identifier with version number
 */
std::string Screenshot::hello() {
	return "WebWorks Screenshot v"+std::string(VERSION)+" - Jon Webb - www.innovatology.nl";
}

/**
 * execute(...) performs the screenshot
 *
 * function parameters:
 *
 * 		handle: the screen_window_t handle of the window we're interested in (determined in index.js)
 * 		userargs: an arguments object passed from the user-side JS, containing...
 *
 * 			rect: {x: int, y:int, w: int, h: int}
 *
 * 					the rectangle to capture (optional, defaults to full screen)
 *
 * 			quality: int
 *
 * 					jpeg/png quality factor (1-100, default 90)
 *
 * 			dest: string
 *
 * 					either "data:", which returns the image as a data-URL (default) using "mime" argument
 * 					or a complete file path with extension, which saves the image to the file
 *
 * 			mime: string
 *
 * 					for "data:" destination, defines the mime type to encode (defaults to image/jpeg)
 *
 *			chmod: int
 *
 *					for screenshots to file, which chmod should be performed
 *
 * returns either:
 *   - the data: url (including data: prefix)
 *   - or error: followed by an error message
 *   - or the full path of the destination file
 *
 */
std::string Screenshot::execute(Json::Value args) {
	screen_context_t ctx = NULL;
	long int handle;
	screen_window_t screen_window;
	int screen_dimensions[2] = {0,0};
	screen_pixmap_t screen_pix = NULL;
	screen_buffer_t screenshot_buf = NULL;
	char *screenshot_ptr = NULL;
	int screenshot_stride = 0;
	int pixmap_usage;
	int pixmap_format;
	std::string result;

	try {

		trace("native:execute");

		// get window handle

		trace("native:getting handle from "+args["handle"].asString());
		handle = std::strtol(args["handle"].asCString(), NULL, 10);
		trace("handle="+intToString(handle));
		screen_window = (screen_window_t) handle;

		// create screen context

		trace("native:creating context");
		if(screen_create_context(&ctx,0))
			throw std::runtime_error("screen_create_context");

		// get screen dimensions

		trace("native:getting size");
		if(screen_get_window_property_iv(screen_window, SCREEN_PROPERTY_BUFFER_SIZE, screen_dimensions))
			throw std::runtime_error("screen_get_window_property size");
		trace("size="+intToString(screen_dimensions[0])+"x"+intToString(screen_dimensions[1]));

		// allow user to override capture region

		int rect[4] = { 0, 0, screen_dimensions[0], screen_dimensions[1] };
		if(args["userargs"].isMember("rect")) {
			Json::Value jr = args["userargs"]["rect"];
			rect[0] = jr["x"].asInt();
			rect[1] = jr["y"].asInt();
			rect[2] = jr["w"].asInt();
			rect[3] = jr["h"].asInt();
			if(rect[2]>screen_dimensions[0]) rect[2]=screen_dimensions[0];
			if(rect[3]>screen_dimensions[1]) rect[3]=screen_dimensions[1];
			if(rect[2]<1) rect[2]=1;
			if(rect[3]<1) rect[3]=1;
		};

		// create pixmap

		trace("native:creating pixmap");
		if(screen_create_pixmap(&screen_pix, ctx))
			throw std::runtime_error("screen_create_pixmap");

		// set pixmap properties

		trace("native:setting pixmap usage");
		pixmap_usage = SCREEN_USAGE_READ | SCREEN_USAGE_NATIVE;
		if(screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &pixmap_usage))
			throw std::runtime_error("screen_set_pixmap_property usage");

		trace("native:setting pixmap format");
		pixmap_format = SCREEN_FORMAT_RGBX8888;
		if(screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &pixmap_format))
			throw std::runtime_error("screen_set_pixmap_property format");

		trace("native:setting pixmap size");
		if(screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, &rect[2]))
			throw std::runtime_error("screen_set_pixmap_property size");

		// create buffer, get properties

		trace("native:creating pixmap buffer");
		if(screen_create_pixmap_buffer(screen_pix))
			throw std::runtime_error("screen_create_pixmap_bufer");

		trace("native:getting pixmap renderbuffers");
		if(screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&screenshot_buf))
			throw std::runtime_error("screen_get_pixmap_propery renderbuffers");

		if(screen_get_buffer_property_pv(screenshot_buf, SCREEN_PROPERTY_POINTER, (void**)&screenshot_ptr))
			throw std::runtime_error("screen_get_buffer_property pointer");

		if(screen_get_buffer_property_iv(screenshot_buf, SCREEN_PROPERTY_STRIDE, &screenshot_stride))
			throw std::runtime_error("screen_get_buffer_property stride");
		trace("stride="+intToString(screenshot_stride));


		// perform the screenshot - read window

		trace("native:read window");
		if(screen_read_window(screen_window, screenshot_buf, 1, (const int*) &rect, 0))
			throw std::runtime_error("screen_read_window");

		// convert pixels to a usable format

		img_convert_f *convFunc;
		convFunc = img_convert_getfunc(IMG_FMT_PKLE_XRGB8888, IMG_FMT_PKLE_XBGR8888);
		if(!convFunc)
			throw std::runtime_error("img_convert_getfunc");
		trace("converting XRGB->XBGR");
		uint8_t *line = (uint8_t*) screenshot_ptr;
		for (int i = 0; i < rect[3]; i++) {
			convFunc(line, line, rect[2]);
			line+=screenshot_stride;
		}

		// create an ImageData structure from the pixels

		bb::ImageData imgdata = bb::ImageData::fromPixels((const unsigned char*)screenshot_ptr, bb::PixelFormat::RGBX, rect[2], rect[3], screenshot_stride);

		// what to do with the screenshot?

		std::string dest = args["userargs"].get("dest", "data:").asString();
		dest = QUrl::fromPercentEncoding(QByteArray(dest.c_str())).toStdString();
		trace("dest = "+dest);

		// and in what quality?

		int quality = args["userargs"].get("quality", 90).asInt();

		// are we returning a data URL?

		if(dest=="data:") {

			std::string mime = args["userargs"].get("mime", "image//jpeg").asString();
			QByteArray qba = bb::utility::ImageConverter::encode(QString::fromStdString(mime), imgdata, quality);
			result = "data:"+mime+";base64,"+std::string(qba.toBase64().constData());

		} else {

			// we're saving to file...

			// remove existing file
			if(remove(dest.c_str()))
				if(errno!=ENOENT)
					throw runtime_error("Error "+intToString(errno)+" ("+std::string(strerror(errno))+") in remove() for "+dest);

			// save new file
			if(!bb::utility::ImageConverter::encode(QUrl(QString::fromStdString(dest)), imgdata, quality)) {
				throw runtime_error("Error "+intToString(errno)+" ("+std::string(strerror(errno))+") in ImageConverter::encode "+dest);
			}

			// chmod (for window covers etc)
			int mode = args["userargs"].get("chmod", 0xEEEEEE).asInt();
			trace("chmod "+intToHex(mode));
			if(mode!=0xEEEEEE) {
				if(chmod(dest.c_str(), mode))
					throw runtime_error(std::string(strerror(errno))+" in chmod() for "+dest);
			}
			result = dest;
		}

		trace("done");

	} catch (exception& ex) {
		result = "error:"+std::string(ex.what());
		trace(result);
	}

	// clean up...
	if(screen_pix!=NULL)
		screen_destroy_pixmap(screen_pix);
	if(ctx!=NULL)
		screen_destroy_context(ctx);

	return result;
}

