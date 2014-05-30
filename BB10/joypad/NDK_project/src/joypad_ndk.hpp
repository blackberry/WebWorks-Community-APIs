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

#ifndef joypadNDK_HPP_
#define joypadNDK_HPP_

#include <string>
#include <screen/screen.h>
#include <bps/event.h>
#include "Logger.hpp"

class joypadJS;

typedef struct GameController_t {
    // Static device info.
    screen_device_t handle;
    int type;
    int analogCount;
    int buttonCount;
    char id[64];

    // Current state.
    int buttons;
    int analog0[3];
    int analog1[3];

    // Text to display to the user about this controller.
    char deviceString[256];
    char buttonsString[128];
    char analog0String[128];
    char analog1String[128];
} GameController;

typedef struct OldControllerState_t {
	// Current state.
	int buttons;
	int analog0[3];
	int analog1[3];
} OldControllerState;

namespace webworks {

class joypadNDK {
public:
	explicit joypadNDK(joypadJS *parent = NULL);
	virtual ~joypadNDK();

	// The extension methods are defined here
	std::string start();
	std::string stop();

	// BPS Stuff
    bool StartEvents();
    void StopEvents();
    void joypadEventCallback(int, int);
    void bpsEvents(Logger* log);
    void handleScreenEvent(bps_event_t *event, Logger* log);

    // Joypad Stuff
    std::string discoverControllers();
    webworks::Logger* getLog();
    bool isThreadHalt();

private:
	joypadJS *m_pParent;
	int controllerIndex;
	bool threadHalt;
};

} // namespace webworks

#endif /* joypadNDK_H_ */
