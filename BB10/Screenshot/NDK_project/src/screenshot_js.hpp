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


#ifndef SCREENSHOT_JS_HPP_
#define SCREENSHOT_JS_HPP_

#define VERSION "1.0" // extension version number
#define TRACE 0 // send trace events back to JS?

#include <string.h>
#include "../public/plugin.h"
#include <json/reader.h>

class Screenshot: public JSExt {

public:
    explicit Screenshot(const std::string& id);
    virtual ~Screenshot();

    // Interfaces of JSExt
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);


private:
    // plumbing
    std::string m_id;

    // utility
    void NotifyEvent(const std::string& event);
    void NotifyTrace(const std::string& output);

    // implementation
    std::string hello();
    std::string execute(Json::Value args);

};

#endif /* SCREENSHOT_JS_HPP_ */
