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

#ifndef __NOWPLAYING_JS_HPP__
#define __NOWPLAYING_JS_HPP__

#include <string>
#include <QThread>
#include "../public/plugin.h"
#include "NowPlaying_ndk.hpp"
#include "ApplicationThread.hpp"

using namespace std;

class NowPlayingJS: public JSExt {

    public:
        explicit NowPlayingJS(const std::string& id);
        virtual ~NowPlayingJS();
        virtual bool CanDelete() { return true; };
        void NotifyEvent(const std::string& event);
        virtual std::string InvokeMethod(const std::string& command);
        bb::webworks::extensions::ApplicationThread* applicationThread() const { return m_thread; };

    private:
        std::string m_id;
        bb::webworks::extensions::ApplicationThread *m_thread;

        // Definition of a pointer to the actual native extension code
        webworks::NowPlayingNDK *m_NowPlayingMgr;

}; // class NowPlayingJS



#endif
