/*
 * compass_js.cpp
 *
 *  Created on: 2012-09-29
 *      Author: starpax
 */
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sensor/libsensor.h>
#include <sensor/sensor.h>
#include <fcntl.h>
#include <devctl.h>
#include <math.h>
#include "compass_js.hpp"

using namespace std;

/**
 * Default constructor.
 */
Compass::Compass(const std::string& id)
	: m_id(id)
	, m_compassExists(false)
	, m_thread(0)
{
    isDeviceCompatible();
}

/**
 * Memory destructor.
 */
Compass::~Compass()
{
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList()
{
    static char name[] = "Compass";
    return name;
}

/**
 * This method is used by JSExt to instantiate the Compass object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id)
{
    if (className == "Compass")
    {
        return new Compass(id);
    }

    return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool Compass::CanDelete()
{
    return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string Compass::InvokeMethod(const string& command)
{
    // Determine which function should be executed
    if (command == "monitorCompassNative")
    {
        return StartMonitoringNative();
    }
    else if (command == "stopMonitoringNative")
    {
    	return StopMonitoringNative();
    }
    else if (command == "readCompassNative")
    {
    	return convertFloatToString(readCompass());
    }
    else
    {
        return "Unsupported Method";
    }
}

/**
 * Method to check if this device has a working compass.
 * @return: bool - true if working compass is available
 */
bool Compass::isDeviceCompatible()
{
	const char* compassPath = "/dev/sensor/compass";
	int checkResult = access(compassPath, F_OK);

	if ( checkResult != -1 )
	{
		m_compassExists = true;
	}
	else
	{
		m_compassExists = false;
	}

	return m_compassExists;
}

float Compass::readCompass()
{
	if ( m_compassExists )
	{
		const char* sensorPath = "/dev/sensor/compass";
		sensor_event_t sensorEvent;
		ssize_t length = 0;

		// Open the sensor with read-only access
		int checkSensor = open(sensorPath, O_RDONLY);

		if ( checkSensor == -1 )
		{
			// TODO: Log error here - Accessing path sensorPath
			return 0.0f;
		}

		// Send the skip duplicate event device control.
		// The device must be moved for a new event to be sent.
		sensor_devctl_skipdupevent_u deviceControl;
		memset(&deviceControl, 0, sizeof(deviceControl));
		deviceControl.tx.enable = 1;
		int readCheck = devctl(checkSensor, DCMD_SENSOR_SKIPDUPEVENT, &deviceControl, sizeof(deviceControl), NULL);

		if ( readCheck != EOK )
		{
			// TODO: Log error here - Attempting to skip duplicates
		}

		while (true)
		{
			memset(&sensorEvent, 0, sizeof(sensorEvent));

			// Block waiting for new sensor data
			length = read(checkSensor, &sensorEvent, sizeof(sensorEvent));

			// Check for errors
			if ( length == -1 )
			{
				// TODO: Log error here - Read error
				break;
			}
			if ( length < (int)sizeof(sensorEvent) )
			{
				// TODO: Log error here - Read size less than expected
				continue;
			}
			if ( sensorEvent.type != SENSOR_TYPE_COMPASS )
			{
				// TODO: Log error here - Sensor type different than expected
				continue;
			}

			// Get compass azimuth (0->360 degrees from magnetic north)
			if ( sensorEvent.compass_s.is_face_down == 0 )
			{
				return sensorEvent.compass_s.azimuth;
			}
			else
			{
				// Device is upside down, flip compass heading
				return fmodf(sensorEvent.compass_s.azimuth + 180.0f, 360.0f);
			}
		}
	}

	return 0.0f;
}

/**
 * Thread that retrieves the current leading of the compass every second and
 * sends it to the JavaScript side using an event. The thread shall continue
 * to retrieve the memory usage until the native object is destroyed on the
 * JavaScript side.
 */
void* CompassThread(void* parent)
{
    Compass *pParent = static_cast<Compass *>(parent);

    // Endless loop that collects compass information and send it to JNext JavaScript side
    while (true)
    {
        pParent->SendCompassInfo();
        sleep(1);
    }

    return NULL;
}

/**
 * Method responsible for starting the thread to get compass leading. Only one
 * thread can be created per native JavaScript instance. This method returns
 * true if the thread was created successfully and false otherwise.
 */
bool Compass::StartMonitoringThread()
{
    if (!m_thread)
    {
        pthread_attr_t thread_attr;
        pthread_attr_init(&thread_attr);
        pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

        pthread_create(&m_thread, &thread_attr, CompassThread, static_cast<void *>(this));
        pthread_attr_destroy(&thread_attr);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Method used to start the get memory usage thread. The method shall return a
 * string to the JavaScript side indicating whether or not the memory
 * monitoring was initialized.
 */
string Compass::StartMonitoringNative()
{
    if (StartMonitoringThread())
    {
        return "Compass monitored";
    }
    else
    {
        return "Compass already being monitored";
    }
}

string Compass::StopMonitoringNative()
{
	if (m_thread)
	{
		pthread_abort(m_thread);
		m_thread = 0;
		return "Compass monitoring has stopped";
	}
	else
	{
		return "Compass isn't already being monitored";
	}
}

/**
 * Method used by the getMemoryUsage thread to pass the amount of free memory
 * on the JavaScript side by firing an event.
 */
void Compass::SendCompassInfo()
{
	std::string eventString = "CompassLeading " + convertFloatToString(readCompass());
    NotifyEvent(eventString);
}

// Notifies JavaScript of an event
void Compass::NotifyEvent(const std::string& event)
{
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}

/**
 * Utility function to convert a float into a string
 */
string Compass::convertFloatToString(float l)
{
	stringstream ss;
	ss << l;
	return ss.str();
}




