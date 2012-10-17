/*
 * compass_js.hpp
 *
 *  Created on: 2012-09-29
 *      Author: starpax
 */

#ifndef COMPASS_JS_HPP_
#define COMPASS_JS_HPP_

#include <string>
#include <pthread.h>
#include "../public/plugin.h"

class Compass: public JSExt
{

public:

    explicit Compass(const std::string& id);
    virtual ~Compass();

    // Interfaces of JSNext
    virtual bool CanDelete();
    virtual std::string InvokeMethod(const std::string& command);

    // Methods for the memory thread
    void SendCompassInfo();
    void NotifyEvent(const std::string& event);

    bool isDeviceCompassCompatible();
    float readCompass();

private:

    string convertLongToString(long l);
    string convertFloatToString(float l);

    // Memory monitoring thread function
    string MonitorCompassNative();
    bool StartThread();

    std::string m_id;

    // member flag to check for compass availability
    bool m_compassExists;

    pthread_t m_thread;
};


#endif
