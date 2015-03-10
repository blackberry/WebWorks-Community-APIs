/*
* Copyright (c) 2013 BlackBerry Limited.
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

#ifndef __MEDIAKEYS_JS_HPP__
#define __MEDIAKEYS_JS_HPP__

#include <string>
#include <QThread>
#include "../public/plugin.h"
#include "Logger.hpp"
#include "mediaKeys_ndk.hpp"
#include "ApplicationThread.hpp"


class MediaKeysJS: public JSExt {

public:
    explicit MediaKeysJS(const std::string& id);
    virtual ~MediaKeysJS();
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);
    void NotifyEvent(const std::string& event);
    bb::webworks::extensions::ApplicationThread* applicationThread() const { return m_thread; };
    webworks::Logger* getLog();

private:
    std::string m_id;
	bb::webworks::extensions::ApplicationThread *m_thread;

    // Definition of a pointer to the actual native extension code
    webworks::MediaKeysNDK *m_sysDialogMgr;
    webworks::Logger *m_pLogger;
};

#endif // __MEDIAKEYS_JS_HPP__
