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

#ifndef SimpleXpBeaconPlugin_NDK_HPP_
#define SimpleXpBeaconPlugin_NDK_HPP_

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

#define RETURN_VIA_CALLBACK(__callbackid, ...) \
    do { \
        m_pParent->NotifyEvent(__callbackid + " " + __VA_ARGS__); \
        return EMPTY_RETURN_STRING; \
    } while (0)

class SimpleXpBeaconPlugin_JS;

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

class SimpleXpBeaconPlugin_NDK {
public:
    SimpleXpBeaconPlugin_JS *m_pParent;

    explicit SimpleXpBeaconPlugin_NDK(SimpleXpBeaconPlugin_JS *parent = NULL);
	virtual ~SimpleXpBeaconPlugin_NDK();

	// The extension methods are defined here

	std::string initialiseBluetooth(const std::string& callbackId);
    std::string terminateBluetooth(const std::string& callbackId);

    std::string pluginVersion(const std::string& callbackId);

    std::string startMonitoring(const std::string& callbackId);
    std::string stopMonitoring(const std::string& callbackId);

    void parseBeaconData(const char *data, int len, int8_t rssi, const char *bdaddr);

private:
    bool m_bt_initialised;
    bool m_monitoring;
    std::string m_monitoring_callback_id;
};

} // namespace webworks

#endif /* SimpleXpBeaconPlugin_NDK_HPP_ */
