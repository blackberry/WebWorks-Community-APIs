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
#ifndef SimpleBtSppPlugin_NDK_HPP_
#define SimpleBtSppPlugin_NDK_HPP_

#include "Util.hpp"
#include "LocalTypes.hpp"

#include <pthread.h>
#include <btapi/btdevice.h>
#include <btapi/btspp.h>

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

class SimpleBtSppPlugin_JS;

namespace webworks {

void btEvent(const int event, const char *bt_addr, const char *event_data);
void *scanForDevicesThread(void *parent);
void *deviceConnectionThread(void *parent);
void sppCallback(long param, int fd);

class SimpleBtSppPlugin_NDK {
public:
	explicit SimpleBtSppPlugin_NDK(SimpleBtSppPlugin_JS *parent = NULL);
	virtual ~SimpleBtSppPlugin_NDK();

	SimpleBtSppPlugin_JS *m_pParent;

    std::string initialiseBluetooth();
    std::string terminateBluetooth();

    std::string getBluetoothAddress();
    void setBluetoothAddress(const std::string& inputString);
    std::string getSppServiceUuid();
    void setSppServiceUuid(const std::string& inputString);

    std::string startDeviceScan(const std::string& callbackId);
    std::string connectToDevice(const std::string& callbackId);
    std::string disconnectFromDevice();
    std::string sendMessage(const std::string& inputString);
    std::string listenForConnection(const std::string& callbackId);
    std::string stopListening();
    std::string pluginVersion();

    void *scanForDevices();
    void *connectionHandler();
    void handleSppServerCallback(int fd);

    pthread_mutex_t m_scan_mutex;
    pthread_mutex_t m_connection_mutex;
    bool m_scan_thread_running;
    bool m_connection_thread_running;

private:
    int startThread();
    void establishPipe();

    bool connectionThreadToStop();
    void setConnectionThreadToStop();

    bool isCandidateDevice(bt_remote_device_t *remoteDevice);

    void *processMessageFromRemoteDevice();
    std::string sendToRemoteDevice(const std::string& inputString);

    int m_resynch_counter;
    bool m_resync_in_progress;
    bool m_connected;
    bool m_remote_disconnected_us;
    bool m_disconnect_in_progress;
    int m_current_buffer_location;
    uint8_t m_in_buffer[BUFFER_SIZE];
    uint8_t m_out_buffer[BUFFER_SIZE];
    pthread_cond_t m_stopthread_cond;
    std::string m_scan_callback_id;
    std::string m_connection_callback_id;
    std::string m_bluetooth_address;
    int m_pfd[2];
    int m_sppfd;
    pthread_attr_t m_attr_s;
    pthread_attr_t m_attr_c;
    pthread_t m_scanthread;
    pthread_t m_connectionthread;
    bool m_bt_initialised;
    bool m_stop_connection_thread;
    bool m_ssp_server_mode;
    std::string m_spp_service_uuid;
};

} // namespace webworks

#endif /* SimpleBtSppPlugin_NDK_HPP_ */
