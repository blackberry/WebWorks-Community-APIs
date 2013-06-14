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
#ifndef THUMBNAIL_JS_HPP_
#define THUMBNAIL_JS_HPP_
#include <img/img.h>
#include <string>
#include <pthread.h>
#include "../public/plugin.h"
#define TMP_PATH "tmp/"
class ThumbNail: public JSExt {

public:
    explicit ThumbNail(const std::string& id);
    virtual ~ThumbNail();

// Interfaces of JSExt
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);


private:
    std::string createThumbNail(const std::string path, int width, int height, unsigned int quality, int flag);
    std::string convertLongToString(long l);
    std::string m_id;
};

#endif /* THUMBNAIL_JS_HPP_ */
