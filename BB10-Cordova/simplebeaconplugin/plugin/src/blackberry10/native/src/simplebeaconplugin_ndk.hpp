/*
* Copyright (c) 2015 BlackBerry Limited
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

#ifndef SimpleBeaconPlugin_NDK_HPP_
#define SimpleBeaconPlugin_NDK_HPP_

#include "Util.hpp"
#include "LocalTypes.hpp"

#include <btapi/btdevice.h>
#include <btapi/btle.h>
#include <bbndk.h>

#define LOGD(...) \
    do { \
        char __temp[500]; \
        sprintf(__temp, "XXXX " __VA_ARGS__); \
        if (s_btController) s_btController->m_pParent->getLog()->debug(__temp); \
    } while (0)

#define LOGI(...) \
    do { \
        char __temp[500]; \
        sprintf(__temp, "XXXX " __VA_ARGS__); \
        if (s_btController) s_btController->m_pParent->getLog()->info(__temp); \
    } while (0)

class SimpleBeaconPlugin_JS;

namespace webworks {

void btEvent(const int event, const char *btAddr, const char *eventData);
/*
 * Don't use deprecated API if we're building for >= 10.3.0
 */
#if BBNDK_VERSION_AT_LEAST(10,3,0)
void btLeAdvertisementCbExt(const char *bdaddr, int8_t rssi, bt_le_advert_packet_event_t eventType, const char *data, int len, void *userData);
#else
void btLeAdvertisementCb(const char *bdaddr, int8_t rssi, const char *data, int len, void *userData);
#endif

class SimpleBeaconPlugin_NDK {
public:
    SimpleBeaconPlugin_JS *m_pParent;

    explicit SimpleBeaconPlugin_NDK(SimpleBeaconPlugin_JS *parent = NULL);
	virtual ~SimpleBeaconPlugin_NDK();

	// The extension methods are defined here

	std::string initialiseBluetooth();
    std::string terminateBluetooth();

    std::string pluginVersion();

    std::string startMonitoring(const std::string& callbackId);
    std::string stopMonitoring();

    void parseBeaconData(const char *data, int len, int8_t rssi, const char *bdaddr);

private:
    bool m_bt_initialised;
    bool m_monitoring;
    std::string m_monitoring_callback_id;
};

} // namespace webworks

#endif /* SimpleBeaconPlugin_NDK_HPP_ */
