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
#include "simplebtlehrplugin_ndk.hpp"
#include "simplebtlehrplugin_js.hpp"
#include <errno.h>
#include <stdio.h>

static webworks::SimpleBtLeHrPlugin_NDK *s_btController = NULL;

namespace webworks {

    void btEvent(const int event, const char *bt_addr, const char *event_data)
    {
        LOGD("btEvent - event=%s, bt_addr=%s, event_data=%s", Util::btEventName(event), bt_addr, event_data);
    }

    void gattServiceConnected(const char *bt_addr, const char *service, int instance, int err, uint16_t conn_int, uint16_t latency, uint16_t super_timeout, void *user_data)
    {
        SimpleBtLeHrPlugin_NDK *pParent = static_cast<SimpleBtLeHrPlugin_NDK *>(user_data);
        LOGD("Bluetooth service connected: %s on %s instance %d", bt_addr, service, instance);

        if (pParent) {
            pParent->handleGattServiceConnected(bt_addr, service, instance, err, conn_int, latency, super_timeout);
        }
    }

    void gattServiceDisconnected(const char *bt_addr, const char *service, int instance, int reason, void *user_data)
    {
        SimpleBtLeHrPlugin_NDK *pParent = static_cast<SimpleBtLeHrPlugin_NDK *>(user_data);
        LOGD("Bluetooth service disconnected: %s instance %d", bt_addr, instance);

        if (pParent) {
            pParent->handleGattServiceDisconnected(bt_addr, service, instance, reason);
        }
    }

    void gattServiceUpdated(const char *bt_addr, int instance, uint16_t conn_int, uint16_t latency, uint16_t super_timeout, void *user_data)
    {
        SimpleBtLeHrPlugin_NDK *pParent = static_cast<SimpleBtLeHrPlugin_NDK *>(user_data);
        LOGD("Bluetooth service updated: %s instance %d", bt_addr, instance);

        if (pParent) {
            pParent->handleGattServiceUpdated(bt_addr, instance, conn_int, latency, super_timeout);
        }
    }

    void notifications_cb(int instance, uint16_t handle, const uint8_t *val, uint16_t len, void *user_data)
    {
        SimpleBtLeHrPlugin_NDK *pParent = static_cast<SimpleBtLeHrPlugin_NDK *>(user_data);
        LOGD("Bluetooth service notification call-back received");

        if (pParent) {
            pParent->handleNotifications_cb(instance, handle, val, len);
        }
    }

    void *scanForDevicesThread(void *parent)
    {
        SimpleBtLeHrPlugin_NDK *pParent = static_cast<SimpleBtLeHrPlugin_NDK *>(parent);

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

    SimpleBtLeHrPlugin_NDK::SimpleBtLeHrPlugin_NDK(SimpleBtLeHrPlugin_JS *parent)
        : m_pParent(parent)
        , m_scan_thread_running(false)
        , m_scan_callback_id("")
        , m_bluetooth_address("")
        , m_scanthread(0)
        , m_bt_initialised(false)
        , m_monitoring_hr(false)
        , m_hr_handle(0)
        , m_hr_value_handle(0)
        , m_hr_instance(0)
    {
        s_btController = this;

        m_gatt_callbacks.connected = gattServiceConnected;
        m_gatt_callbacks.disconnected = gattServiceDisconnected;
        m_gatt_callbacks.updated = gattServiceUpdated;

        LOGI("SimpleBtLeHrPlugin_NDK Created");
    }

    SimpleBtLeHrPlugin_NDK::~SimpleBtLeHrPlugin_NDK()
    {
        LOGI("SimpleBtLeHrPlugin_NDK destructor");

        m_bt_initialised = false;
        m_bluetooth_address = "";
        bt_device_deinit();
        bt_gatt_deinit();
        pthread_mutex_destroy(&m_scan_mutex);
        s_btController = NULL;
    }


    std::string SimpleBtLeHrPlugin_NDK::initialiseBluetooth()
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

        rc = bt_gatt_init(&m_gatt_callbacks);
        if (rc == EOK) {
            LOGD("Bluetooth bt_gatt_init() OK");
            m_bt_initialised = true;
        } else {
            snprintf(temp, sizeof(temp), "Bluetooth bt_gatt_init() error - rc=%d, error=%s", rc, strerror(errno));
            LOGD("%s", temp);
            m_bt_initialised = false;
            bt_device_deinit();

            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            return writer.write(root);
        }

        pthread_mutex_init(&m_scan_mutex, NULL);

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Initialised";
        return writer.write(root);
    }

    std::string SimpleBtLeHrPlugin_NDK::terminateBluetooth()
    {
        LOGD("terminateBluetooth");

        Json::FastWriter writer;
        Json::Value root;

        m_bt_initialised = false;

        pthread_mutex_destroy(&m_scan_mutex);
        bt_gatt_deinit();
        bt_device_deinit();

        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Bluetooth Terminated";
        return writer.write(root);
    }

    std::string SimpleBtLeHrPlugin_NDK::startDeviceScan(const std::string& callbackId)
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

    void *SimpleBtLeHrPlugin_NDK::scanForDevices()
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
                const int device_type = bt_rdev_get_type(remote_device);
                LOGD("Device type=%d", device_type);

                if ((device_type == BT_DEVICE_TYPE_LE_PUBLIC) || (device_type == BT_DEVICE_TYPE_LE_PRIVATE)) {
                    LOGD("Got a Bluetooth LE device\n");
                    char **services_array = bt_rdev_get_services_gatt(remote_device);
                    if (services_array) {
                        LOGD("Enumerating services on device\n");
                        int j;
                        for (j = 0; services_array[j]; j++) {
                            LOGD("Found service: %s\n", services_array[j]);
                            if (strcmpi(services_array[j], HEART_RATE_SERVICE_UUID) == 0) {
                                device_index = device_count++;
                                bt_rdev_get_address(remote_device, device_info[device_index].address);
                                LOGD("Address=%s\n", device_info[device_index].address);
                                bt_rdev_get_friendly_name(remote_device, device_info[device_index].name, sizeof(device_info[device_index].name));
                                LOGD("Name=%s\n", device_info[device_index].name);
                            }
                            number_of_services++;
                        }
                        bt_rdev_free_services(services_array);
                    } else {
                        LOGD("Unable to get service list - errno: %s\n", strerror(errno));
                    }
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
            device[JSON_KEY_ADDRESS] = device_info[i].address;
            device[JSON_KEY_NAME] = device_info[i].name;
            devices.append(device);
        }
        root[JSON_KEY_DEVICES] = devices;

        m_pParent->NotifyEvent(m_scan_callback_id + " " + writer.write(root));

        return (NULL);
    }

    std::string SimpleBtLeHrPlugin_NDK::pluginVersion()
    {
        return PLUGIN_VERSION;
    }

    std::string SimpleBtLeHrPlugin_NDK::getBluetoothAddress()
    {
        return m_bluetooth_address;
    }

    void SimpleBtLeHrPlugin_NDK::setBluetoothAddress(const std::string& inputString)
    {
        m_bluetooth_address = inputString;
    }

    std::string SimpleBtLeHrPlugin_NDK::startMonitoringHr(const std::string& callbackId)
    {
        LOGD("startMonitoringHr");

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

        if (m_monitoring_hr) {
            LOGD("Already monitoring Heart Rate");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Already monitoring Heart Rate";
            return writer.write(root);
        }

        m_monitoring_hr = false;
        m_monitoring_hr_callback_id = callbackId;

        bt_gatt_conn_parm_t con_parm;
        con_parm.minConn = 0x30;
        con_parm.maxConn = 0x50;
        con_parm.latency = 0;
        con_parm.superTimeout = 50;

        LOGD("calling bt_gatt_connect_service()");
        LOGD("address=%s", m_bluetooth_address.c_str());
        LOGD("Service=%s", HEART_RATE_SERVICE_UUID);
        if (bt_gatt_connect_service(m_bluetooth_address.c_str(), HEART_RATE_SERVICE_UUID, NULL, &con_parm, this) != EOK) {


            // there's a known issue where sometimes we get ERRNO=EBUSY (16)
            // when this is not the case and we've connected to the service OK.
            // So for now we ignore this errno value.

            if (errno != EBUSY) {
                snprintf(temp, sizeof(temp), "GATT connect service request failed: %s", strerror(errno));
                LOGD("%s", temp);
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = temp;
                root[JSON_KEY_ERROR_CODE] = (int)errno;
                return writer.write(root);
            }
        }

        LOGD("Requested connection to Heart Rate service OK");
        root[JSON_KEY_STATUS] = JSON_VALUE_OK;
        root[JSON_KEY_DESCRIPTION] = "Requested connection to Heart Rate service OK";
        return writer.write(root);
    }

    std::string SimpleBtLeHrPlugin_NDK::stopMonitoringHr()
    {
        LOGD("In stopMonitoringHr()");

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

        if (!m_monitoring_hr) {
            LOGD("Not monitoring Heart Rate");
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "Not monitoring Heart Rate";
            return writer.write(root);
        }

        if (bt_gatt_disconnect_service(m_bluetooth_address.c_str(), HEART_RATE_SERVICE_UUID) != EOK) {
            snprintf(temp, sizeof(temp), "GATT disconnect service request failed: %s", strerror(errno));
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            return writer.write(root);
        } else {
            LOGD("GATT disconnect from Heart Rate service request OK");
            m_monitoring_hr = false;
            m_hr_handle = 0;
            m_hr_instance = 0;
            m_hr_value_handle = 0;
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "GATT disconnect from Heart Rate service request OK";
            return writer.write(root);
        }
    }

    void SimpleBtLeHrPlugin_NDK::handleGattServiceConnected(const char *bt_addr, const char *service, int instance, int err, uint16_t conn_int, uint16_t latency, uint16_t super_timeout)
    {
        LOGD("handleGattServiceConnected()");
        Json::FastWriter writer;
        Json::Value root;
        char temp[500];
        int rc = 0;
        std::string callback_id;
        std::string characteristic_to_monitor;
        bool *service_being_monitored = NULL;
        uint16_t *handle_for_service;
        uint16_t *value_handle_for_service;

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_EVENT] = JSON_VALUE_CONNECTION;

        if (strcmpi(service, HEART_RATE_SERVICE_UUID) == 0) {
            callback_id = m_monitoring_hr_callback_id;
            characteristic_to_monitor = HEART_RATE_MEASUREMENT;
            m_hr_instance = instance;
            service_being_monitored = &m_monitoring_hr;
            handle_for_service = &m_hr_handle;
            value_handle_for_service = &m_hr_value_handle;
        } else {
            return;
        }

        if (err != EOK) {
            *service_being_monitored = false;
            snprintf(temp, sizeof(temp), "Failure to connect to service=%s - err=%d", service, err);
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = err;
            m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
            return;
        }

        LOGD("Registering for notifications");

        rc = bt_gatt_reg_notifications(instance, notifications_cb);
        if (rc != EOK) {
            *service_being_monitored = false;
            snprintf(temp, sizeof(temp), "bt_gatt_reg_notifications - errno=%s", strerror(errno));
            LOGD("%s", temp);
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = temp;
            root[JSON_KEY_ERROR_CODE] = (int)errno;
            m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
            return;
        }

        LOGD("Allocated memory for notifications");

        int num_characteristics = bt_gatt_characteristics_count(instance);

        LOGD("# characteristics=%d", num_characteristics);

        if (num_characteristics > -1) {
            bt_gatt_characteristic_t *characteristic_list;

            characteristic_list = (bt_gatt_characteristic_t*) malloc(num_characteristics * sizeof(bt_gatt_characteristic_t));

            if (NULL == characteristic_list) {
                LOGD("GATT characteristics: Not enough memory");
                bt_gatt_disconnect_instance(instance);
                *service_being_monitored = false;
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "GATT characteristics: Not enough memory";
                m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
                return;
            }

            /* BEGIN WORKAROUND - Temporary fix to address race condition */
            int number = 0;
            do {
                number = bt_gatt_characteristics(instance, characteristic_list, num_characteristics);
            } while ((number == -1) && (errno== EBUSY));

            int characteristic_list_size = number;

            int i;
            for (i = 0; i < characteristic_list_size; i++) {

                LOGD("characteristic: uuid,handle,value_handle: %s, %d, %d", characteristic_list[i].uuid,
                        characteristic_list[i].handle, characteristic_list[i].value_handle);

                if (strcmpi(characteristic_list[i].uuid, characteristic_to_monitor.c_str()) == 0) {
                    LOGD("characteristic available");

                    *handle_for_service = characteristic_list[i].handle;
                    *value_handle_for_service = characteristic_list[i].value_handle;

                    LOGD("registering for notifications. uuid, handle, value_handle=%s, %d, %d",
                            characteristic_list[i].uuid, characteristic_list[i].handle, characteristic_list[i].value_handle);
                    errno = 0;
                    rc = bt_gatt_enable_notify(instance, &characteristic_list[i], 1);
                    if (rc != EOK) {
                        *service_being_monitored = false;
                        snprintf(temp, sizeof(temp), "bt_gatt_enable_notify - errno=%s", strerror(errno));
                        LOGD("%s", temp);
                        root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                        root[JSON_KEY_DESCRIPTION] = temp;
                        root[JSON_KEY_ERROR_CODE] = (int)errno;
                    } else {
                        LOGD("bt_gatt_enable_notify was presumably OK");
                        *service_being_monitored = true;
                    }
                }
            }

            if (characteristic_list != NULL) {
                free(characteristic_list);
                characteristic_list = NULL;
            }

            LOGD("done registering for notifications");

        } else {
            LOGD("GATT characteristics: Improper no of characteristics");
            bt_gatt_disconnect_instance(instance);
            *service_being_monitored = false;
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
            root[JSON_KEY_DESCRIPTION] = "GATT characteristics: Improper no of characteristics";
            m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
            return;
        }

        if (*service_being_monitored) {
            root[JSON_KEY_STATUS] = JSON_VALUE_OK;
            root[JSON_KEY_DESCRIPTION] = "Registered for notifications";
        } else {
            root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
        }

        m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
    }

    void SimpleBtLeHrPlugin_NDK::handleGattServiceDisconnected(const char *bt_addr, const char *service, int instance, int reason)
    {
        LOGD("handleGattServiceDisconnected()");
        Json::FastWriter writer;
        Json::Value root;
        char temp[500];
        int rc = 0;
        std::string callback_id;

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_EVENT] = JSON_VALUE_DISCONNECTION;
        root[JSON_KEY_STATUS] = JSON_VALUE_OK;

        if (strcmpi(service, HEART_RATE_SERVICE_UUID) == 0) {
            m_monitoring_hr = false;
            m_hr_handle = 0;
            m_hr_instance = 0;
            m_hr_value_handle = 0;
            callback_id = m_monitoring_hr_callback_id;
            root[JSON_KEY_DESCRIPTION] = "Heart Rate Service Disconnected";
        } else {
            return;
        }
        m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
    }

    void SimpleBtLeHrPlugin_NDK::handleGattServiceUpdated(const char *bt_addr, int instance, uint16_t conn_int, uint16_t latency, uint16_t super_timeout)
    {
        // Not really useful at this time just ignore for the moment
    }

    void SimpleBtLeHrPlugin_NDK::handleNotifications_cb(int instance, uint16_t handle, const uint8_t *val, uint16_t len)
    {
        LOGD("Bluetooth service notification call-back received");

        Json::FastWriter writer;
        Json::Value root;
        std::string callback_id;
        bool *service_being_monitored;

        bool hr_data_format_8 = false;
        bool sensor_contact_detected = false;
        bool sensor_contact_feature_supported = false;
        bool energy_expended_feature_supported = false;
        bool rr_interval_data_present = false;
        uint8_t hr_measurement = 0;
        uint16_t energy_expended = 0;
        uint8_t num_rr_intervals = 0;
        uint16_t* rr_intervals = NULL;

        root[JSON_KEY_SOURCE] = JSON_VALUE_THREAD;
        root[JSON_KEY_STATUS] = JSON_VALUE_OK;

        if (instance == m_hr_instance && handle == m_hr_value_handle) {
            callback_id = m_monitoring_hr_callback_id;
            service_being_monitored = &m_monitoring_hr;

            Json::Value hr;

            root[JSON_KEY_EVENT] = JSON_VALUE_HR_DATA;
            root[JSON_KEY_DESCRIPTION] = "Heart Rate Device data";

            int index = 0;
            uint8_t flags = val[index++];
            LOGD("FLAGS=%d\n", flags);

            hr_data_format_8 = ((flags & HEART_RATE_VALUE_FORMAT) != HEART_RATE_VALUE_FORMAT);
            sensor_contact_detected = ((flags & SENSOR_CONTACT_DETECTED) == SENSOR_CONTACT_DETECTED);
            sensor_contact_feature_supported = ((flags & SENSOR_CONTACT_FEATURE) == SENSOR_CONTACT_FEATURE);
            energy_expended_feature_supported = ((flags * ENERGY_EXPENDED_FEATURE) == ENERGY_EXPENDED_FEATURE);
            rr_interval_data_present = ((flags & RR_INTERVAL_DATA) == RR_INTERVAL_DATA);


            if (!hr_data_format_8) {
                LOGD("16 bit heart rate measurement data encountered - we only support 8 bit values. Please unstrap your HR monitor from that frightened gerbil immediately!\n");
                root[JSON_KEY_STATUS] = JSON_VALUE_ERROR;
                root[JSON_KEY_DESCRIPTION] = "16 bit heart rate measurement data encountered - we only support 8 bit values. Please unstrap your HR monitor from that frightened gerbil immediately!";
                m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
                return;
            }

            hr_measurement = val[index++];

            if (energy_expended_feature_supported) {
                energy_expended = val[index];
                index = index + 2;
            }

            if (rr_interval_data_present) {
                num_rr_intervals = (len - index) / sizeof(uint16_t);
                int num_bytes = num_rr_intervals * sizeof(uint16_t);
                rr_intervals = (uint16_t*) malloc(num_rr_intervals * sizeof(uint16_t*));
                memcpy(rr_intervals, (&val) + index, num_bytes);
            }

            LOGD("HR Rate=%d\n", hr_measurement);

            /*
             {
                 "hr_data_format_8": 1,
                 "sensor_contact_feature_supported": 1,
                 "energy_expended_feature_supported": 1,
                 "rr_interval_data_present": 1,
                 "heart_rate": 124,
                 "sensor_contact_detected": 1,      // optional
                 "energy_expended": 123,            // optional
                 "rr_intervals": [ 1, 2, 45, 12, 1] // optional - num_rr_intervals of uint16_t
             }
             */

            hr[JSON_KEY_HR_DATA_FLAGS] = flags;
            hr[JSON_KEY_HR_DATA_FORMAT_8] = hr_data_format_8 ? "true" : "false";
            hr[JSON_KEY_HR_DATA_SENSOR_CONTACT_FEATURE_SUPPORTED] = sensor_contact_feature_supported ? "true" : "false";
            hr[JSON_KEY_HR_DATA_SENSOR_ENERGY_EXPENDED_FEATURE_SUPPORTED] = energy_expended_feature_supported ? "true" : "false";
            hr[JSON_KEY_HR_DATA_SENSOR_RR_INTERVAL_DATA_PRESENT] = rr_interval_data_present ? "true" : "false";
            hr[JSON_KEY_HR_DATA_SENSOR_HEART_RATE] = hr_measurement;

            if (sensor_contact_feature_supported) {
                hr[JSON_KEY_HR_DATA_SENSOR_CONTACT_DETECTED] = sensor_contact_detected;
            }

            if (energy_expended_feature_supported) {
                hr[JSON_KEY_HR_DATA_SENSOR_ENERGY_EXPENDED] = energy_expended;
            }

            if (rr_interval_data_present) {
                Json::Value rr = Json::Value(Json::arrayValue);
                rr.clear();
                for (int i=0; i<num_rr_intervals; i++) {
                    rr.append((uint16_t)(*(rr_intervals + i*sizeof(uint16_t))));
                }
                hr[JSON_KEY_HR_DATA_SENSOR_RR_INTERVALS] = rr;
            }

            root[JSON_KEY_DATA] = hr;

            if (rr_intervals) {
                num_rr_intervals = 0;
                free(rr_intervals);
                rr_intervals = 0;
            }
        } else {
            return;
        }

        if (!(*service_being_monitored)) {
            LOGD("Not monitoring the service on which we've been notified");
            return;
        }

        m_pParent->NotifyEvent(callback_id + " " + writer.write(root));
    }

} /* namespace webworks */
