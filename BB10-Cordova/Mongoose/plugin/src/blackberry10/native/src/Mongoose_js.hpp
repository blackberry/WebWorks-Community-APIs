/*
* Copyright (c) 2014 BlackBerry Limited
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

#ifndef MongooseJS_HPP_
#define MongooseJS_HPP_

#include <string>
#include "../public/plugin.h"
#include "Mongoose_ndk.hpp"
#include "Logger.hpp"

class MongooseJS: public JSExt {

public:
    explicit MongooseJS(const std::string& id);
    virtual ~MongooseJS();
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);
    void NotifyEvent(const std::string& event);
    webworks::Logger* getLog();

private:
    std::string m_id;
    // Definition of a pointer to the actual native extension code
    webworks::MongooseNDK *m_pMongooseController;
    webworks::Logger *m_pLogger;
};

#endif /* MongooseJS_HPP_ */