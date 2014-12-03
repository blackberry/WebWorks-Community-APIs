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
#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "simplebtsppplugin_ndk.hpp"
#include "simplebtsppplugin_js.hpp"
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <math.h>

static webworks::SimpleBtSppPlugin_NDK *s_btController = NULL;

namespace webworks {

    void btEvent(const int event, const char *bt_addr, const char *event_data)
    {
        LOGD("btEvent - event=%s, bt_addr=%s, event_data=%s", Util::btEventName(event), bt_addr, event_data);
    }

    void sppCallback(long param, int fd)
    {
        LOGD("sppCallback");
        SimpleBtSppPlugin_NDK *pParent = reinterpret_cast<SimpleBtSppPlugin_NDK *>(param);

        if (pParent) {
            pParent->handleSppServerCallback(fd);
        }
    }

    void *scanForDevicesThread(void *parent)
    {
        SimpleBtSppPlugin_NDK *pParent = static_cast<SimpleBtSppPlugin_NDK *>(parent);

        pthread_mutex_lock(&(pParent->m_scan_mutex));
        pParent->m_scan_thread_running = true;
        pthread_mutex_unlock(&(pParent->m_scan_mutex));

        LOGD("scanForDevicesThread() - entered - thread=%d", pthread_self());

        pParent->scanForDevices();

        pthread_mutex_lock(&(pParent->m_scan_mutex));
        pParent->m_scan_thread_running = false;
        pthread_mutex_unlock(&(pParent->m_scan_mutex));

        LOGD("scanForDevicesThread() - exiting - thread=%d", pthread_self());

        return (NULL);
    }

    void *deviceConnectionThread(void *parent)
    {
        SimpleBtSppPlugin_NDK *pParent = static_cast<SimpleBtSppPlugin_NDK *>(parent);

        pthread_mutex_lock(&(pParent->m_connection_mutex));
        pParent->m_connection_thread_running = true;
        pthread_mutex_unlock(&(pParent->m_connection_mutex));

        LOGD("deviceConnectionThread() - entered - thread=%d", pthread_self());

        pParent->connectionHandler();

        pthread_mutex_lock(&(pParent->m_connection_mutex));
        pParent->m_connection_thread_running = false;
        pthread_mutex_unlock(&(pParent->m_connection_mutex));

        LOGD("deviceConnectionThread() - exiting - thread=%d", pthread_self());

        return (NULL);
    }

    SimpleBtSppPlugin_NDK::SimpleBtSppPlugin_NDK(SimpleBtSppPlugin_JS *parent)
        : m_pParent(parent)
        , m_scan_thread_running(false)
        , m_connection_thread_running(false)
        , m_resynch_counter(0)
        , m_resync_in_progress(false)
        , m_connected(false)
        , m_remote_disconnected_us(false)
        , m_disconnect_in_progress(false)
        , m_current_buffer_location(0)
        , m_scan_callback_id("")
        , m_connection_callback_id("")
        , m_bluetooth_address("")
        , m_sppfd(-1)
        , m_scanthread(0)
        , m_connectionthread(0)
        , m_bt_initialised(false)
        , m_stop_connection_thread(true)
        , m_ssp_server_mode(false)
        , m_spp_service_uuid(SPP_SERVICE_UUID)
        , m_rfcomm_mode(false)
        , m_rfcomm_service_port(SPP_RFCOMM_SERVICE_PORT)
    {
        s_btController = this;

        pthread_mutex_init(&m_scan_mutex, NULL);
        pthread_mutex_init(&m_connection_mutex, NULL);
        pthread_cond_init(&m_stopthread_cond, NULL);

        LOGI("SimpleBtSppPlugin_NDK Created");
    }

    SimpleBtSppPlugin_NDK::~SimpleBtSppPlugin_NDK()
    {
        LOGI("SimpleBtSppPlugin_NDK destructor");

        bool need_to_stop_connection = false;
        pthread_mutex_lock(&m_scan_mutex);
        need_to_stop_connection = m_scan_thread_running;
        pthread_mutex_unlock(&m_scan_mutex);

        if (need_to_stop_connection) {
            disconnectFromDevice();
        }

        m_connected = false;
        m_remote_disconnected_us = false;
        m_disconnect_in_progress = false;
        m_bt_initialised = false;
        m_bluetooth_address = "";
        bt_spp_deinit();
        bt_device_deinit();
        pthread_mutex_destroy(&m_scan_mutex);
        pthread_mutex_destroy(&m_connection_mutex);
        pthread_cond_destroy(&m_stopthread_cond);
        s_btController = NULL;
    }


    std::string SimpleBtSppPlugin_NDK::initialiseBluetooth()
    {
        LOGD("initialiseBluetooth");

        Json::FastWriter writer;
        Json::Value root;
        char temp[500];

        if (m_bt_initialised) {
            LOGD("Bluetooth Already Initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Already Initialised";
            return writer.write(root);
        }

        m_connected = false;
        m_remote_disconnected_us = false;
        m_disconnect_in_progress = false;

        int rc = bt_device_init(btEvent);

// START *** TEMP workaround ***
//
#if BBNDK_VERSION_AT_LEAST(10,3,0)
        bt_ldev_set_filters(BT_EVT_ALL_EVENT, true); // COREOS-96977 / 825695 -- workaround for this defect in 10.3.1E
#endif
//
// END  *** TEMP workaround ***

        if (rc == EOK) {
            LOGD("Bluetooth bt_device_init() OK");
        } else {
            snprintf(temp, sizeof(temp), "Bluetooth bt_device_init() error - rc=%d, error=%s", rc, strerror(errno));
            LOGD("%s", temp);
            m_bt_initialised = false;
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            return writer.write(root);
        }

        if (!bt_ldev_get_power()) {
            LOGD("Bluetooth calling bt_ldev_set_power()");
            bt_ldev_set_power(true);
        }

        rc = bt_spp_init();
        if (rc == EOK) {
            LOGD("Bluetooth bt_spp_init() OK");
        } else {
            snprintf(temp, sizeof(temp), "Bluetooth bt_spp_init() error - rc=%d, error=%s", rc, strerror(errno));
            LOGD("%s", temp);
            m_bt_initialised = false;
            bt_device_deinit();

            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            return writer.write(root);
        }

        m_bt_initialised = true;

        pthread_mutex_init(&m_scan_mutex, NULL);

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Initialised";
        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::terminateBluetooth()
    {
        LOGD("terminateBluetooth");

        Json::FastWriter writer;
        Json::Value root;

        bool need_to_stop_connection = false;
        pthread_mutex_lock(&m_connection_mutex);
        need_to_stop_connection = m_connection_thread_running;
        pthread_mutex_unlock(&m_connection_mutex);

        if (need_to_stop_connection) {
            disconnectFromDevice();
        }

        m_connected = false;
        m_remote_disconnected_us = false;
        m_disconnect_in_progress = false;
        m_bt_initialised = false;

        pthread_mutex_destroy(&m_scan_mutex);
        pthread_mutex_destroy(&m_connection_mutex);
        pthread_cond_destroy(&m_stopthread_cond);
        bt_spp_deinit();
        bt_device_deinit();

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Terminated";
        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::startDeviceScan(const std::string& callbackId)
    {
        LOGD("startDeviceScan");

        Json::FastWriter writer;
        Json::Value root;
        char temp[500];

        root[JSON_KEY_SOURCE] = JSON_VALUE_FUNCTION;

        if (!m_bt_initialised) {
            LOGD("Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (m_scan_thread_running) {
            LOGD("Scan already in progress");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Scan already in progress";
            return writer.write(root);
        }

        m_scan_callback_id = callbackId;

        int policy;
        struct sched_param param;

        pthread_attr_init(&m_attr_s);
        pthread_attr_setdetachstate(&m_attr_s, PTHREAD_CREATE_DETACHED); // not interested in joining thread
        pthread_attr_setinheritsched(&m_attr_s, PTHREAD_EXPLICIT_SCHED);
        pthread_getschedparam(pthread_self(), &policy, &param);

        param.sched_priority = 12;

        pthread_attr_setschedparam(&m_attr_s, &param);
        pthread_attr_setschedpolicy(&m_attr_s, SCHED_RR);

        pthread_mutex_init(&m_scan_mutex, NULL);

        if (pthread_create(&m_scanthread, &m_attr_s, scanForDevicesThread, static_cast<void *>(this)) != EOK) {
            snprintf(temp, sizeof(temp), "pthread_create() error - error=%s", strerror(errno));
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            return writer.write(root);
        }
        pthread_attr_destroy(&m_attr_s);

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Device Scan Started";

        LOGD("exiting startDeviceScan");

        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::connectToDevice(const std::string& callbackId)
    {
        LOGD("connectToDevice");

        Json::FastWriter writer;
        Json::Value root;
        char temp[500];

        root[JSON_KEY_SOURCE] = JSON_VALUE_FUNCTION;

        if (!m_bt_initialised) {
            LOGD("Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (m_ssp_server_mode) {
            LOGD("Running in SSP Server mode");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Running in SSP Server mode";
            return writer.write(root);
        }

        if (m_connection_thread_running) {
            LOGD("Device is already connected");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Device is already connected";
            return writer.write(root);
        }

        establishPipe();

        int fd = 0;

        if (m_rfcomm_mode) {
#if BBNDK_VERSION_AT_LEAST(10,3,0)
            fd = bt_spp_open_ex(
                    (char *) m_bluetooth_address.c_str(),
                    (char *) "",
                    (char *) m_spp_service_uuid.c_str(),
                    m_rfcomm_service_port,
                    false,
                    NULL,
                    0);
#else
            LOGD("Unable to open SPP file descriptor - bt_spp_open_ex() not supported");
            m_sppfd = -1;
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Unable to open SPP file descriptor - bt_spp_open_ex() not supported";
            return writer.write(root);
#endif
        } else {
            fd = bt_spp_open(
                    (char *) m_bluetooth_address.c_str(),
                    (char *) m_spp_service_uuid.c_str(),
                    false);
        }

        if (fd >= 0) {
            m_sppfd = fd;
        } else {
            LOGD("Unable to open SPP file descriptor");
            m_sppfd = -1;
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Unable to open SPP file descriptor";
            return writer.write(root);
        }

        m_connected = true;
        m_remote_disconnected_us = false;
        m_disconnect_in_progress = false;

        pthread_mutex_lock(&m_connection_mutex);
        m_stop_connection_thread = false;
        pthread_mutex_unlock(&m_connection_mutex);
        pthread_cond_signal(&m_stopthread_cond);

        m_connection_callback_id = callbackId;

        if (startThread() == EOK) {
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Connection Thread Started";
        } else {
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Connection Thread Failed to Start";
        }

        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::disconnectFromDevice()
    {
        LOGD("disconnectFromDevice");

        Json::FastWriter writer;
        Json::Value root;
        int rc;

        if (!m_bt_initialised) {
            LOGD("Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (!m_connection_thread_running) {
            LOGD("Device not connected");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Device not connected";
            return writer.write(root);
        }

        if (!m_remote_disconnected_us) {
            LOGD("Normal disconnection");
            if (!m_connected) {
                LOGD("Not connected");
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "Error disconnecting since not already connected";
                return writer.write(root);
            }

            m_disconnect_in_progress = true;

            pthread_mutex_lock(&m_connection_mutex);
            m_stop_connection_thread = true;
            pthread_mutex_unlock(&m_connection_mutex);
            pthread_cond_signal(&m_stopthread_cond);

            /*
             * Bit of a hack since we don't have pselect() :-(
             * write to a looped back pipe being select()ed by thread
             */
            LOGD("Writing to pipe");
            ssize_t rc = ::write(m_pfd[1], "x", 1);
            LOGD("Written to pipe rc=%d", rc);

            void *exit_status;
            pthread_join(m_connectionthread, &exit_status);

        } else {
            LOGD("Remote disconnected us");
            LOGD("Waiting until thread ends");
            rc = pthread_join(m_connectionthread, NULL);
            LOGD("pthread_join rc = %d", rc);
        }

        pthread_cond_destroy(&m_stopthread_cond);
        pthread_mutex_destroy(&m_connection_mutex);

        m_connectionthread = 0;
        m_stop_connection_thread = true;

        if (::close(m_pfd[1]) < 0) {
            LOGD("Error closing write side of pipe");
        }

        m_sppfd = -1;
        m_connected = false;
        m_remote_disconnected_us = false;

        LOGD("Connection Thread Stopped");

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Connection Thread Stopped";

        LOGD("Exiting disconnectFromDevice");

        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::sendMessage(const std::string& inputString)
    {
        LOGD("sendMessage");
        Json::FastWriter writer;
        Json::Value root;

        if (!m_bt_initialised) {
            LOGD("sendMessage() - Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (!m_connection_thread_running) {
            LOGD("sendMessage() - Device not connected");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Device not connected";
            return writer.write(root);
        }

        return sendToRemoteDevice(inputString);
    }

    std::string SimpleBtSppPlugin_NDK::sendToRemoteDevice(const std::string& inputString)
    {
        LOGD("sendToRemoteDevice");

        Json::Reader reader;
        Json::Value input_root;
        Json::FastWriter writer;
        Json::Value root;
        Json::Value command = Json::Value(Json::arrayValue);
        ssize_t bytes_written = 0;

        LOGD("sending: %s", inputString.c_str());

        bool parsed_success = reader.parse(inputString, input_root, false);

        if (!parsed_success) {
            LOGD("sendToRemoteDevice() - JSON command parse failure");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "JSON command parse failure";
            return writer.write(root);
        }
        command = input_root[JSON_KEY_COMMAND];

        memset(m_out_buffer, 0, sizeof(m_out_buffer));

        for(unsigned int i=0; i < command.size(); i++) {
            if (i < sizeof(m_out_buffer)) {
                uint8_t command_byte = (((uint8_t)(command[i].asInt())) & 0xff);
                m_out_buffer[i] = command_byte;
            } else {
                LOGD("sendToRemoteDevice() - Command too long");
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "Command too long";
                return writer.write(root);
            }
        }

        char text_buffer[1024];
        char *o = text_buffer;
        for (int i=0; i<(int)command.size(); i++) {
            o += snprintf(o, sizeof(text_buffer)-(o-text_buffer), "0x%02X,", m_out_buffer[i]);
        }
        LOGD("Command: len=%d, data=%s", command.size(), text_buffer);

        if (m_sppfd > 0) {
            LOGD("sendToRemoteDevice() - writing data to remote device");
            bytes_written = ::write(m_sppfd, m_out_buffer, command.size());
            LOGD("sendToRemoteDevice() - number of bytes written=%d", bytes_written);
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "Data written to file descriptor";
            return writer.write(root);
        }

        LOGD("sendToRemoteDevice() - File descriptor not valid");
        root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
        root[JSON_KEY_DESCRIPTION] = "File descriptor not valid";

        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::listenForConnection(const std::string& callbackId)
    {
        LOGD("listenForConnection");

        Json::FastWriter writer;
        Json::Value root;
        char temp[500];
        int rc = 0;

        root[JSON_KEY_SOURCE] = JSON_VALUE_FUNCTION;

        if (!m_bt_initialised) {
            LOGD("Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (m_ssp_server_mode) {
            LOGD("Already running in SSP Server mode");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Already running in SSP Server mode";
            return writer.write(root);
        }

        if (m_connection_thread_running) {
            LOGD("Device is already connected");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Device is already connected";
            return writer.write(root);
        }

        m_ssp_server_mode = true;
        m_connection_callback_id = callbackId;

        if (m_rfcomm_mode) {
#if BBNDK_VERSION_AT_LEAST(10,3,0)
            rc = bt_spp_open_server_ex(
                    (char *) "",
                    (char *) m_spp_service_uuid.c_str(),
                    m_rfcomm_service_port,
                    false,
                    sppCallback,
                    reinterpret_cast<long>(this));
#else
            m_ssp_server_mode = false;
            LOGD("Bluetooth bt_spp_open_server_ex() -- API not supported");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth bt_spp_open_server_ex() -- API not supported";
            return writer.write(root);
#endif
        } else {
            rc = bt_spp_open_server(
                    (char *) "",
                    (char *) m_spp_service_uuid.c_str(),
                    false,
                    sppCallback,
                    reinterpret_cast<long>(this));
        }

        if (rc == EOK) {

            LOGD("listenForConnection() - Started listener");
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "Started listener";

        } else {
            m_ssp_server_mode = false;
            snprintf(temp, sizeof(temp), "Bluetooth bt_spp_open_server() error=%s", strerror(errno));
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
        }

        return writer.write(root);
    }

    std::string SimpleBtSppPlugin_NDK::stopListening()
    {
        LOGD("stopListening");

        Json::FastWriter writer;
        Json::Value root;
        char temp[500];
        int rc;

        root[JSON_KEY_SOURCE] = JSON_VALUE_FUNCTION;

        if (!m_bt_initialised) {
            LOGD("Bluetooth Not initialised");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Bluetooth Not initialised";
            return writer.write(root);
        }

        if (!m_ssp_server_mode) {
            LOGD("Not running in SSP Server mode");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Not running in SSP Server mode";
            return writer.write(root);
        }

        m_ssp_server_mode = false;

        rc = bt_spp_close_server((char *) m_spp_service_uuid.c_str());
        if (rc == EOK) {

            LOGD("stopListening() - Stopped listener");
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "Stopped listener";

        } else {
            snprintf(temp, sizeof(temp), "Bluetooth bt_spp_close_server() error - rc=%d, error=%s", rc, strerror(errno));
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
        }

        return writer.write(root);
    }

    void SimpleBtSppPlugin_NDK::handleSppServerCallback(int fd)
    {
        LOGD("sppCallback");

        Json::FastWriter writer;
        Json::Value root;
        char address[18];
        int rc;

        memset(address, 0, sizeof(address));

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_EVENT] = JSON_VALUE_CONNECTION;

        if (fd >= 0) {
            m_sppfd = fd;

            rc = bt_spp_get_address(m_sppfd, address);
            root[JSON_KEY_ADDRESS] = address;

            if (m_connected) {
                ::close(fd);
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "Connection request received whilst already connected";
                m_pParent->NotifyEvent(m_connection_callback_id + " " + writer.write(root));
                return;
            }

            m_connected = true;
            m_remote_disconnected_us = false;
            m_disconnect_in_progress = false;

            pthread_mutex_lock(&m_connection_mutex);
            m_stop_connection_thread = false;
            pthread_mutex_unlock(&m_connection_mutex);
            pthread_cond_signal(&m_stopthread_cond);

            if (startThread() == EOK) {
                root[JSON_KEY_STATUS] = JSON_VALUE_OK;
                root[JSON_KEY_DESCRIPTION] = "Inbound Connection Event";
            } else {
                ::close(m_sppfd);
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "Unable to start thread on inbound connection";
            }
            m_pParent->NotifyEvent(m_connection_callback_id + " " + writer.write(root));
        }
    }

    int SimpleBtSppPlugin_NDK::startThread()
    {
        LOGD("startThread");

        char temp[500];

        establishPipe();

        int policy;
        struct sched_param param;

        pthread_attr_init(&m_attr_c);
        pthread_attr_setdetachstate(&m_attr_c, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setinheritsched(&m_attr_c, PTHREAD_EXPLICIT_SCHED);
        pthread_getschedparam(pthread_self(), &policy, &param);

        param.sched_priority = 12;

        pthread_attr_setschedparam(&m_attr_c, &param);
        pthread_attr_setschedpolicy(&m_attr_c, SCHED_RR);

        pthread_mutex_init(&m_connection_mutex, NULL);
        pthread_cond_init(&m_stopthread_cond, NULL);

        if (pthread_create(&m_connectionthread, &m_attr_c, deviceConnectionThread, static_cast<void *>(this)) != EOK) {
            snprintf(temp, sizeof(temp), "pthread_create() error - error=%s", strerror(errno));
            LOGD("%s", temp);
            return -1;
        }
        pthread_attr_destroy(&m_attr_c);

        LOGD("exiting startThread");

        return EOK;
    }

    void *SimpleBtSppPlugin_NDK::scanForDevices()
    {
        LOGD("In scanForDevices()");

        bt_disc_start_inquiry(BT_INQUIRY_GIAC);

        bt_remote_device_t **remote_device_array = 0;
        bt_remote_device_t *remote_device = 0;
        int total_device_count = 0;

        LOGD("Calling bt_disc_retrieve_by_service");

        remote_device_array = bt_disc_retrieve_devices(BT_DISCOVERY_ALL, &total_device_count);

        LOGD("Total devices=%d", total_device_count);

        bt_device_information_t device_info[total_device_count];

        int device_count = 0;

        if (remote_device_array) {
            int device_index;
            int number_of_services = 0;
            int i;
            for (i = 0; (remote_device = remote_device_array[i]); ++i) {
                if (isCandidateDevice(remote_device)) {
                    const int device_type = bt_rdev_get_type(remote_device);
                    LOGD("Device type=%d", device_type);
                    device_index = device_count++;
                    bt_rdev_get_address(remote_device, device_info[device_index].address);
                    LOGD("Address=%s", device_info[device_index].address);
                    bt_rdev_get_friendly_name(remote_device, device_info[device_index].name, sizeof(device_info[device_index].name));
                    LOGD("Name=%s", device_info[device_index].name);
                }
            }
            LOGD("Freeing remote device array");
            bt_rdev_free_array(remote_device_array);
        }

        LOGD("device_count=%d", device_count);

        Json::FastWriter writer;
        Json::Value root;
        Json::Value device;
        Json::Value devices = Json::Value(Json::arrayValue);

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "List of candidate devices";

        for (int i = 0; i < device_count; i++) {
            device.clear();
            device["address"] = device_info[i].address;
            device["name"] = device_info[i].name;
            devices.append(device);
        }
        root[JSON_KEY_DEVICES] = devices;

        m_pParent->NotifyEvent(m_scan_callback_id + " " + writer.write(root));

        return (NULL);
    }

    bool SimpleBtSppPlugin_NDK::isCandidateDevice(bt_remote_device_t *remoteDevice)
    {
        LOGD("isCandidateDevice()");

        bool is_good_match = true;
        char buffer[256];

        if (!remoteDevice) {
            LOGD("isCandidateDevice() - no devices - exiting");
            return false;
        }

        return is_good_match;
    }


    void *SimpleBtSppPlugin_NDK::connectionHandler()
    {
        LOGD("In connectionHandler()");

        fd_set read_file_descriptor_set;
        int num_file_descriptors = 0;
        struct timeval time_val;

        m_resynch_counter = 0;

        Json::FastWriter writer;
        Json::Value root;
        Json::Value header;

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;

        int rc = 0;

        while (!connectionThreadToStop()) {

            FD_ZERO(&read_file_descriptor_set);
            FD_SET(m_sppfd, &read_file_descriptor_set);
            FD_SET(m_pfd[0], &read_file_descriptor_set);
            num_file_descriptors = (m_sppfd > m_pfd[0]) ? m_sppfd : m_pfd[0];
            time_val.tv_sec = DATA_INTERVAL_SECONDS;
            time_val.tv_usec = 0;
            rc = select(num_file_descriptors + 1, &read_file_descriptor_set, NULL, NULL, &time_val);

            if (rc > 0) {
                LOGD("select() filedescriptor ready rc=%d", rc);
                /*
                 * Bit of a hack since we don't seem to have pselect() :-(
                 */
                if (FD_ISSET(m_pfd[0], &read_file_descriptor_set)) {
                    LOGD("select() data on pipe -- terminate");
                    Util::flushFileDescriptor(m_pfd[0]);
                }
                if (FD_ISSET(m_sppfd, &read_file_descriptor_set)) {
                    LOGD("select() data on SPP -- read it");
                    LOGD("About to read packet");

                    if (m_sppfd <= 0) {
                        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
                        root[JSON_KEY_EVENT] = JSON_VALUE_DISCONNECTION;
                        root[JSON_KEY_DESCRIPTION] = "Connection terminated";

                        m_pParent->NotifyEvent(m_connection_callback_id + " " + writer.write(root));

                        if (!m_disconnect_in_progress) {
                            LOGD("Hmm looks like remote end has disconnected");
                            setConnectionThreadToStop();
                            m_remote_disconnected_us = true;
                            m_connected = false;
                        } else {
                            LOGD("Looks like a disconnect is in progress - let it work its way through");
                        }
                        continue;
                    }
                    processMessageFromRemoteDevice();
                }
            } else if (rc == 0) {
                if (m_resync_in_progress) {
                    m_resync_in_progress = false;
                    processMessageFromRemoteDevice();
                }
                continue;
            } else if (rc < 0) {
                LOGD("select() error rc=%d, error=%s", rc, strerror(errno));

                if (!m_disconnect_in_progress) {
                    LOGD("Hmm looks like remote end has disconnected");

                    char temp[500];
                    snprintf(temp, sizeof(temp), "Connection terminated - errno=%d, str_errno=%s", errno, strerror(errno));
                    LOGD("%s", temp);

                    root[JSON_KEY_STATUS] = JSON_VALUE_OK;
                    root[JSON_KEY_EVENT] = JSON_VALUE_DISCONNECTION;
                    root[JSON_KEY_DESCRIPTION] = temp;

                    m_pParent->NotifyEvent(m_connection_callback_id + " " + writer.write(root));

                    setConnectionThreadToStop();
                    m_disconnect_in_progress = true;
                    m_remote_disconnected_us = true;
                    m_connected = false;
                } else {
                    LOGD("Looks like a disconnect is in progress - let it work its way through");
                }
            }
        }
        /*
         * Flush the read side of pipe and close it
         */
        LOGD("Exiting thread");
        Util::flushFileDescriptor(m_pfd[0]);
        ::close(m_pfd[0]);
        ::close(m_sppfd);
        m_sppfd = -1;
        m_connected = false;

        return (NULL);
    }

    void *SimpleBtSppPlugin_NDK::processMessageFromRemoteDevice()
    {

        LOGD("processMessageFromRemoteDevice()");

        Json::FastWriter writer;
        Json::Value root;
        Json::Value header;
        Json::Value data;

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Device data";

        ssize_t length_read = ::read(m_sppfd,
                (m_in_buffer + m_current_buffer_location),
                BUFFER_SIZE - m_current_buffer_location);

        LOGD("[A] processMessageFromRemoteDevice() - read returned=%d", length_read);

        if (length_read >= 0) {
            m_current_buffer_location += length_read;
        } else {
            LOGD("[B] processMessageFromRemoteDevice() - expecting file descriptor to disconnect!");
        }

        char text_buffer[1024];
        char *o = text_buffer;
        for (int i=0; i< m_current_buffer_location; i++) {
            o += snprintf(o, sizeof(text_buffer)- (o- text_buffer), "0x%02X,", m_in_buffer[i]);
        }

        root[JSON_KEY_FORMAT] = MESSAGE_FORMAT_ID_RAW;
        Json::Value raw = Json::Value(Json::arrayValue);
        raw.clear();
        for (int i = 0; i < m_current_buffer_location; i++) {
            raw.append(m_in_buffer[i]);
        }
        root[JSON_KEY_DATA] = raw;
        m_current_buffer_location = 0;
        m_pParent->NotifyEvent(m_connection_callback_id + " " + writer.write(root));

        return (NULL);
    }

    std::string SimpleBtSppPlugin_NDK::pluginVersion()
    {
        return PLUGIN_VERSION;
    }

    std::string SimpleBtSppPlugin_NDK::getSppServiceUuid()
    {
        return m_spp_service_uuid;
    }

    void SimpleBtSppPlugin_NDK::setSppServiceUuid(const std::string& inputString)
    {
        m_spp_service_uuid = inputString;
    }

    std::string SimpleBtSppPlugin_NDK::getBluetoothAddress()
    {
        return m_bluetooth_address;
    }

    void SimpleBtSppPlugin_NDK::setBluetoothAddress(const std::string& inputString)
    {
        m_bluetooth_address = inputString;
    }

    bool SimpleBtSppPlugin_NDK::getRfcommMode()
    {
        return m_rfcomm_mode;
    }

    void SimpleBtSppPlugin_NDK::setRfcommMode(const bool rfcommMode)
    {
        m_rfcomm_mode = rfcommMode;
    }

    int SimpleBtSppPlugin_NDK::getRfcommServicePort()
    {
        return m_rfcomm_service_port;
    }

    void SimpleBtSppPlugin_NDK::setRfcommServicePort(const int rfcommServicePort)
    {
        m_rfcomm_service_port = rfcommServicePort;
    }

    bool SimpleBtSppPlugin_NDK::connectionThreadToStop()
    {
        bool threadToStop;

        pthread_mutex_lock(&m_connection_mutex);
        threadToStop = m_stop_connection_thread;
        pthread_mutex_unlock(&m_connection_mutex);

        return threadToStop;
    }

    void SimpleBtSppPlugin_NDK::setConnectionThreadToStop()
    {
        pthread_mutex_lock(&m_connection_mutex);
        m_stop_connection_thread = true;
        pthread_mutex_unlock(&m_connection_mutex);
    }

    void SimpleBtSppPlugin_NDK::establishPipe()
    {
        int flags;

        pipe(m_pfd);

        flags = fcntl(m_pfd[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(m_pfd[0], F_SETFL, flags);

        flags = fcntl(m_pfd[1], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(m_pfd[1], F_SETFL, flags);
    }

} /* namespace webworks */
