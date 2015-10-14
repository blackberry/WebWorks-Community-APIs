/*
 * Copyright (c) 2013 BlackBerry Limited
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
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include <bps/navigator.h>
#include "setlockscreenwallpaper_ndk.hpp"
#include "setlockscreenwallpaper_js.hpp"

namespace webworks {

Setlockscreenwallpaper_NDK::Setlockscreenwallpaper_NDK(Setlockscreenwallpaper_JS *parent):
	m_pParent(parent),
	setlockscreenwallpaperProperty(50),
	setlockscreenwallpaperThreadCount(1),
	threadHalt(true),
	m_thread(0) {
		cond  = PTHREAD_COND_INITIALIZER;
		mutex = PTHREAD_MUTEX_INITIALIZER;
		m_pParent->getLog()->info("Setlockscreenwallpaper Created");
}

Setlockscreenwallpaper_NDK::~Setlockscreenwallpaper_NDK() {
}

// These methods are the true native code we intend to reach from WebWorks

std::string Setlockscreenwallpaper_NDK::ping() {
	return "pong";
}

std::string Setlockscreenwallpaper_NDK::setlockscreenwallpaper(const std::string& inputString) {
   //NOTE: path DONT have to start with file:///, it starts with for eg /accounts/1000/xxx
    const char *filepath = inputString.c_str();
    if (navigator_set_lockscreen_wallpaper(filepath) != BPS_SUCCESS) {
        return "error";
    } else {
       return "success";
    }
}

} /* namespace webworks */
