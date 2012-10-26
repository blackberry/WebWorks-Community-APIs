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

using std::string;

class Compass: public JSExt
{
public:

    explicit Compass(const string& id);
    virtual ~Compass();

    // Interfaces of JSNext
    virtual bool CanDelete();
    virtual string InvokeMethod(const string& command);

    // Methods for the Monitoring thread
    void SendCompassInfo();
    void NotifyEvent(const string& event);

    bool isDeviceCompatible();
    float readCompass();

private:

    string convertFloatToString(float l);

    // Compass Thread Monitoring Methods
    string StartMonitoringNative();
    string StopMonitoringNative();
    bool StartMonitoringThread();

    string m_id;

    // member flag to check for compass availability
    bool m_compassExists;

    pthread_t m_thread;
};


#endif
