/*
* Copyright 2013 Research In Motion Limited.
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

#include <string>
#include "../public/plugin.h"
#include <qstring.h>
#include <qhash.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#ifndef PGAUDIO_JS_H_
#define PGAUDIO_JS_H_

class PGaudio: public JSExt {

public:
    explicit PGaudio(const std::string& id);
    virtual ~PGaudio();
    virtual std::string preload(QString path, QString name, int voices);
    virtual std::string unload(QString name);
    virtual std::string stop(QString name);
    virtual std::string play(QString filename);
    virtual std::string loop(QString filename);
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);
private:
    std::string m_id;
    QHash<QString, ALuint> m_soundBuffersHash;
    QHash<QString, ALuint> m_sourceIndexHash;
};

#endif /* PGAUDIO_JS_HPP_ */
