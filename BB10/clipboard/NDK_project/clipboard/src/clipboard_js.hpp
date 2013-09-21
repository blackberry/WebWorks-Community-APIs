/*
 * clipboard_js.hpp
 *
 *  Created on: 2013-8-17
 *      Author: Merrick Zhang ( anphorea@gmail.com )
 */

#ifndef CLIPBOARD_JS_HPP_
#define CLIPBOARD_JS_HPP_

#include <string.h>
#include <clipboard/clipboard.h>
#include "../public/plugin.h"

class JsClipboard: public JSExt {
public:
	explicit JsClipboard(const std::string& id);
	virtual ~JsClipboard();

	// Interfaces of JSExt
	virtual bool CanDelete();
	virtual std::string InvokeMethod(const std::string& command);
private:
	std::string getTextFromClipboard();
	int setClipboardText(const std::string& arg);
	std::string m_id;
	std::string convertLongToString(long l);

};

#endif /* CLIPBOARD_JS_HPP_ */
