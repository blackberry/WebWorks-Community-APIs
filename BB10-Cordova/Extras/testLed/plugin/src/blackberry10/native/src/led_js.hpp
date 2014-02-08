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
#ifndef MEMORY_JS_HPP_
#define MEMORY_JS_HPP_

#include <string>
#include <pthread.h>
#include "../public/plugin.h"
#include <bps/led.h>
#include <bps/bps.h>
class FlashLed: public JSExt {

public:
    explicit FlashLed(const std::string& id);
    virtual ~FlashLed();

// Interfaces of JSExt
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);

private:
    string startLed(int color, int blinkCount);
    string stopLed(string id);
    std::string m_id;
    string convertLongToString(long l);
};



#endif /* LED_JS_HPP_ */
