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
#ifndef UNZIP_JS_HPP_
#define UNZIP_JS_HPP_

#include <string>
#include <pthread.h>
#include "../public/plugin.h"

class Memory: public JSExt {

public:
    explicit Memory(const std::string& id);
    virtual ~Memory();

// Interfaces of JSExt
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);

// Methods for the memory thread
    void SendMemoryInfo();
    void NotifyEvent(const std::string& event);

private:
// Utility functions relate to memory function
    long getMemory();
    string convertLongToString(long l);

// Memory monitoring thread function
    string MonitorMemoryNative();
    bool StartThread();

    std::string m_id;
    pthread_t m_thread;
};

#endif /* UNZIP_JS_HPP_ */
