/*
 * Copyright (c) 2014 BlackBerry Limited
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
#ifndef SimpleBtLeHrPlugin_NDK_HPP_
#define SimpleBtLeHrPlugin_NDK_HPP_

#include "Util.hpp"
#include "LocalTypes.hpp"

#include <pthread.h>
#include <btapi/btdevice.h>
#include <btapi/btgatt.h>
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

class SimpleBtLeHrPlugin_JS;

namespace webworks {

void btEvent(const int event, const char *bt_addr, const char *event_data);
void *scanForDevicesThread(void *parent);

class SimpleBtLeHrPlugin_NDK {
public:
	explicit SimpleBtLeHrPlugin_NDK(SimpleBtLeHrPlugin_JS *parent = NULL);
	virtual ~SimpleBtLeHrPlugin_NDK();

	SimpleBtLeHrPlugin_JS *m_pParent;

    std::string initialiseBluetooth();
    std::string terminateBluetooth();

    std::string getBluetoothAddress();
    void setBluetoothAddress(const std::string& inputString);

    std::string startDeviceScan(const std::string& callbackId);
    std::string pluginVersion();

    std::string startMonitoringHr(const std::string& callbackId);
    std::string stopMonitoringHr();

    void *scanForDevices();

    void handleGattServiceConnected(const char *bt_addr, const char *service, int instance, int err, uint16_t conn_int, uint16_t latency, uint16_t super_timeout);
    void handleGattServiceDisconnected(const char *bt_addr, const char *service, int instance, int reason);
    void handleGattServiceUpdated(const char *bt_addr, int instance, uint16_t conn_int, uint16_t latency, uint16_t super_timeout);
    void handleNotifications_cb(int instance, uint16_t handle, const uint8_t *val, uint16_t len);

    pthread_mutex_t m_scan_mutex;
    bool m_scan_thread_running;

private:
    std::string m_scan_callback_id;
    std::string m_bluetooth_address;
    pthread_attr_t m_attr_s;
    pthread_t m_scanthread;
    bool m_bt_initialised;

    bt_gatt_callbacks_t m_gatt_callbacks;
    bool m_monitoring_hr;
    std::string m_monitoring_hr_callback_id;
    uint16_t m_hr_handle;
    uint16_t m_hr_value_handle;
    int m_hr_instance;

};

} // namespace webworks

#endif /* SimpleBtLeHrPlugin_NDK_HPP_ */
