/*
 * clipboard_js.cpp
 *
 *  Created on: 2013-8-17
 *      Author: Merrick Zhang ( anphorea@gmail.com )
 */

#include "clipboard_js.hpp"
#include <sstream>

JsClipboard::JsClipboard(const std::string& id) :
		m_id(id) {
	//nothing to do.
}
JsClipboard::~JsClipboard() {
	//nothing to do.
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool JsClipboard::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string JsClipboard::InvokeMethod(const std::string& command) {

	// Determine which function should be executed
	size_t splitter = command.find_first_of(" ");
	string strCommand = command.substr(0, splitter);

	if (strCommand == "setText") {
		if (command.length() > splitter) {
			string params = command.substr(splitter + 1, command.length());
			return convertLongToString(setClipboardText(params));
		} else {

		}
	} else if (strCommand == "getText") {
		return this->getTextFromClipboard();
	}
	return "Unsupported Method";

}

/*
 * get Text From Clip board
 */
std::string JsClipboard::getTextFromClipboard() {

	char* pbuffer=0;
	int ret=get_clipboard_data("text/plain", &pbuffer);
	if (ret>0){
		//return the clip board string.
		std::string data(pbuffer);
		free(pbuffer);
		return data;
	}else{
		//something bad happened,return empty string.
		free(pbuffer);
		return "";
	}



}

/*
 * set Text to clip board.
 */
int JsClipboard::setClipboardText(const std::string& arg) {
	if (get_clipboard_can_write() == 0) {
		//could write clip board.

		//always empty clip board.why ? API told me.
		empty_clipboard();

		//write to the clip board
		int ret = set_clipboard_data("text/plain", arg.size(), arg.c_str());
		return ret;
	} else {
		return -1;
	}
}

/**
 * Utility function to convert a long into a string.
 */
string JsClipboard::convertLongToString(long l) {
    stringstream ss;
    ss << l;
    return ss.str();
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "JsClipboard";
	return name;
}

/**
 * This method is used by JNext to instantiate the Memory object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "JsClipboard") {
		return new JsClipboard(id);
	}
	return NULL;
}
