/*
 * Copyright 2013-2014 BlackBerry Limited.
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

#include <bps/bps.h>
#include <bps/event.h>
#include <bps/navigator.h>
#include <bps/led.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <bps/screen.h>
#include <sys/keycodes.h>

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <screen/screen.h>
#include "joypad_ndk.hpp"
#include "joypad_js.hpp"
#include <pthread.h>
#include <unistd.h>
#include "Logger.hpp"

static screen_context_t _screen_ctx = 0;
static int MAX_CONTROLLERS = 2;
static int MAX_BUTTONS = 20;
static GameController _controllers[2];

#define MUTEX_LOCK() pthread_mutex_trylock(&m_lock)
#define MUTEX_UNLOCK() pthread_mutex_unlock(&m_lock)

static bool m_eventsEnabled = false;
static pthread_mutex_t m_lock;
static pthread_t m_thread = 0;

const char *gcid[] = { 	"20D6-0DAD",
						"1038-1412",
						"25B6-0001",
						"045E-028E",
						NULL };
const char *gcfriendly[] = { "Moga Pro HID",
							 "Zeemote: SteelSeries FREE",
							 "Gametel Bluetooth Controller",
							 "XBox 360 Wired Controller",
							 NULL };

static void pollDevices();
bool loadController(GameController *);
static void initController(GameController *, int);

static void initController(GameController *controller, int player)
{
    // Initialize controller values.
    controller->handle = 0;
    controller->type = 0;
    controller->analogCount = 0;
    controller->buttonCount = 0;
    controller->buttons = 0;
    controller->analog0[0] = controller->analog0[1] = controller->analog0[2] = 0;
    controller->analog1[0] = controller->analog1[1] = controller->analog1[2] = 0;
    sprintf(controller->deviceString, "Player %d: No device detected.", player + 1);
}

bool loadController(GameController *controller)
{
	const char *nameorid;
	int i = 0;

    // Query libscreen for information about this device.
	if(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_TYPE, &controller->type) == -1) {
    	return false;
    }

	if(screen_get_device_property_cv(controller->handle, SCREEN_PROPERTY_ID_STRING, sizeof(controller->id), controller->id) == -1) {
    	return false;
    }

	if(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTON_COUNT, &controller->buttonCount) == -1) {
    	return false;
    }

    // Check for the existence of analog sticks.
    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0)) {
    	++controller->analogCount;
    }

    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1)) {
    	++controller->analogCount;
    }

    nameorid = controller->id;

    while(gcid[i]) {
    	if(strncmp(&(controller->id[2]), gcid[i], 9) == 0) {
    	    nameorid = gcfriendly[i];
    	    break;
    	}
    	i++;
    }

    sprintf(controller->deviceString, "%s", nameorid);

    return true;
}

static void pollDevices()
{
    int i;
    for (i = 0; i < MAX_CONTROLLERS; i++) {
        GameController* controller = &_controllers[i];

        if (controller->handle) {
            // Get the current state of a gamepad device.
            screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTONS, &controller->buttons);

            if (controller->analogCount > 0) {
            	screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0);
            }

            if (controller->analogCount > 1) {
            	screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1);
            }
        }
    }
}

namespace webworks {


joypadNDK::joypadNDK(joypadJS *parent) : threadHalt(false) {
	int i;

	m_pParent = parent;
	controllerIndex = 0;
	for(i=0; i<MAX_CONTROLLERS; i++) {
		initController(&_controllers[i], i);
	}
    bps_initialize();
    m_pParent->getLog()->info("JoypadNDK Created");
}

joypadNDK::~joypadNDK() {
    m_pParent->getLog()->info("JoypadNDK Shutting down");
    StopEvents();
	bps_shutdown();
}

std::string joypadNDK::start() {
	std::string rval;
	m_pParent->getLog()->info("JoypadNDK Start");
	rval = discoverControllers();
	StartEvents();

	return rval;
}

std::string joypadNDK::stop() {
    m_pParent->getLog()->info("JoypadNDK Stop");
	StopEvents();
	return "";
}

std::string joypadNDK::discoverControllers() {
	Json::FastWriter writer;
	Json::Value rval;
	int rc;
	int deviceCount = 0;

	if(screen_create_context(&_screen_ctx, SCREEN_APPLICATION_CONTEXT) == -1) {
	    m_pParent->getLog()->error("screen_create_context() failed");
		return "{'status': false, 'error': 'screen_create_context() failed' }";
	}

	if(screen_get_context_property_iv(_screen_ctx, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount) == -1) {
	    m_pParent->getLog()->error("SCREEN_PROPERTY_DEVICE_COUNT failed");
		return "{'status': false, 'error': 'SCREEN_PROPERTY_DEVICE_COUNT failed' }";
	}

	screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));

	if(screen_get_context_property_pv(_screen_ctx, SCREEN_PROPERTY_DEVICES, (void**)devices) == -1) {
	    m_pParent->getLog()->error("SCREEN_PROPERTY_DEVICES failed");
		return "{'status': false, 'error': 'SCREEN_PROPERTY_DEVICES failed' }";
	}

	int i;
	for (i = 0; i < deviceCount; i++) {
	    int type;

	    if((rc = screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type)) == -1) {
	    	free(devices);
	    	m_pParent->getLog()->error("SCREEN_PROPERTY_TYPE failed");
	    	return "{'status': false, 'error': 'SCREEN_PROPERTY_TYPE failed' }";
	    }

	    if (!rc && (type == SCREEN_EVENT_GAMEPAD ||
	            type == SCREEN_EVENT_JOYSTICK)) {
	        // Assign this device to control Player 1 or Player 2.
	        GameController* controller = &_controllers[controllerIndex];
	        controller->handle = devices[i];
	        if(loadController(controller)) {

				controllerIndex++;
				if (controllerIndex == MAX_CONTROLLERS) {
					break;
				}
	        }
	    }
	}

	free(devices);

	rval["status"] = true;
	rval["error"] = false;
	rval["controllers"] = controllerIndex;

	for(i=0; i<controllerIndex; i++) {
		rval["connected"][i] = _controllers[i].deviceString;
	}

	return writer.write(rval);
}

webworks::Logger* joypadNDK::getLog() {
    return m_pParent->getLog();
}

// getter for the stop value
bool joypadNDK::isThreadHalt() {
    bool isThreadHalt;
    MUTEX_LOCK();
    isThreadHalt = threadHalt;
    MUTEX_UNLOCK();
    return isThreadHalt;
}

// BPS Event handler functions


void *HandleEvents(void *args)
{
	joypadNDK *parent = static_cast<joypadNDK *>(args);
	parent->getLog()->debug("JoypadNDK EventHandler");
	int i, j, mask, changed;
	OldControllerState oldState[MAX_CONTROLLERS];

    if(_screen_ctx) {
        m_eventsEnabled = true;

		while(!parent->isThreadHalt()) {
			MUTEX_LOCK();
			for(i=0; i<MAX_CONTROLLERS; i++) {
				oldState[i].buttons = _controllers[i].buttons;
				for(j=0; j<3; j++) {
					oldState[i].analog0[j] = _controllers[i].analog0[j];
					oldState[i].analog1[j] = _controllers[i].analog1[j];
				}
			}
			pollDevices();
			mask = 1;
			for(i=0; i<MAX_CONTROLLERS; i++) {
				if(_controllers[i].handle) {
				    bool doUpdate = false;
				    stringstream logMessage;
					// XOR old and new state to get a bitmap of changed buttons
					changed = oldState[i].buttons ^ _controllers[i].buttons;
					if (changed) {
					    doUpdate = true;
                        logMessage << "Changed: ";
                        logMessage << changed;
                        parent->getLog()->debug(logMessage.str().c_str());
                        logMessage.str("");
                        logMessage.clear();
					}
					// Use mask as a bitwise counter
//					for(j=0; j<MAX_BUTTONS; j++) {
//						// If the button has changed
//						if(changed & mask) {
//						    logMessage << "Button ";
//						    logMessage << j;
//						    logMessage << " was ";
//							// Signal JS the button has been pressed / released
//							if(changed & _controllers[i].buttons) {
//							    logMessage << "pressed";
//								parent->joypadEventCallback(0, i, j, 1);
//							} else {
//							    logMessage << "depressed";
//								parent->joypadEventCallback(0, i, j, 0);
//							}
//							parent->getLog()->debug(logMessage.str().c_str());
//						}
//						mask = mask << 1;
//					}
					for(j=0; j<3; j++) {
						if(oldState[i].analog0[j] != _controllers[i].analog0[j]) {
						    doUpdate = true;
//							parent->joypadEventCallback(1, i, j, _controllers[i].analog0[j]);
						}
						if(oldState[i].analog1[j] != _controllers[i].analog1[j]) {
						    doUpdate = true;
//							parent->joypadEventCallback(2, i, j, _controllers[i].analog1[j]);
						}
					}
					if (doUpdate) {
					    parent->joypadEventCallback(i);
					}
				}
			}
			MUTEX_UNLOCK();

			// Fast poll
//			usleep(1000);
		}
    }
    return NULL;
}

bool joypadNDK::StartEvents()
{
	if(!m_eventsEnabled) {
		if (!m_thread) {
		    threadHalt = false;
		    pthread_attr_t thread_attr;
		    pthread_attr_init(&thread_attr);
		    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
			int error = pthread_create(&m_thread, &thread_attr, HandleEvents, static_cast<void *>(this));
			pthread_attr_destroy(&thread_attr);
			if (error) {
			    m_pParent->getLog()->error("Thread Failed to start");
				m_thread = 0;
				return false;
			} else {
			    m_pParent->getLog()->info("Thread Started");
				MUTEX_LOCK();
				return true;
			}
		}
	}

	return false;
}

void joypadNDK::StopEvents()
{
	if(m_eventsEnabled) {
		if (m_thread) {
		    MUTEX_LOCK();
		    threadHalt = true;
		    MUTEX_UNLOCK();
		    m_pParent->getLog()->debug("JoypadNDK joining event thread");
			pthread_join(m_thread, NULL);
			m_thread = 0;
			m_eventsEnabled = false;
			m_pParent->getLog()->debug("JoypadNDK event thread stopped");
		}
	}
}

// The callback method that sends an event through JNEXT
void joypadNDK::joypadEventCallback(int type, int ctrl, int id, int val) {
        std::string event = "community.joypad.eventCallback";
        Json::FastWriter writer;
        Json::Value root;
        root["type"] = type;
        root["ctrl"] = ctrl;
        root["id"] = id;
        root["value"] = val;
        m_pParent->NotifyEvent(event + " " + writer.write(root));
}


void joypadNDK::joypadEventCallback(int ctrl) {
        std::string event = "community.joypad.eventCallback";
        Json::FastWriter writer;
        Json::Value root;
        root["ctrl"] = ctrl;
        int mask = 1;
        for(int j=0; j<MAX_BUTTONS; j++) {
            if(mask & _controllers[ctrl].buttons) {
                root["buttons"][j]["pressed"] = true;
                root["buttons"][j]["value"] = 1.0f;
            } else {
                root["buttons"][j]["pressed"] = false;
                root["buttons"][j]["value"] = 0.0f;
            }
            mask = mask << 1;
        }
        for(int j=0; j<3; j++) {
            root["axes"][j] = _controllers[ctrl].analog0[j];
            root["axes"][j+3] = _controllers[ctrl].analog1[j];
        }
        m_pParent->NotifyEvent(event + " " + writer.write(root));
}


} /* namespace webworks */
