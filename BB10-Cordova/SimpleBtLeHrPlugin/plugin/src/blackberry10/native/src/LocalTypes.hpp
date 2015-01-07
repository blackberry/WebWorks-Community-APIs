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
#ifndef LocalTypes_H_
#define LocalTypes_H_

#include <string.h>
#include <errno.h>

#define PLUGIN_VERSION "1.0.0"

#define DEVICE_INFO_BUFF_SIZE (256)
#define DEVICE_INFO_ADDR_SIZE (18)

#define JSON_KEY_STATUS "status"
#define JSON_KEY_DESCRIPTION "desc"
#define JSON_KEY_ERROR_CODE "error_code"
#define JSON_KEY_SOURCE "source"
#define JSON_KEY_EVENT "event"
#define JSON_KEY_ADDRESS "address"
#define JSON_KEY_NAME "name"
#define JSON_KEY_DEVICES "devices"
#define JSON_KEY_DATA "data"
#define JSON_VALUE_ERROR "ERROR"
#define JSON_VALUE_OK "OK"
#define JSON_VALUE_THREAD "thread"
#define JSON_VALUE_FUNCTION "function"
#define JSON_VALUE_HR_DATA "HRDATA"
#define JSON_VALUE_CONNECTION "CONNECTION"
#define JSON_VALUE_DISCONNECTION "DISCONNECTION"

#define JSON_KEY_HR_DATA_FLAGS "flags"
#define JSON_KEY_HR_DATA_FORMAT_8 "hr_data_format_8"
#define JSON_KEY_HR_DATA_SENSOR_CONTACT_FEATURE_SUPPORTED "sensor_contact_feature_supported"
#define JSON_KEY_HR_DATA_SENSOR_ENERGY_EXPENDED_FEATURE_SUPPORTED "energy_expended_feature_supported"
#define JSON_KEY_HR_DATA_SENSOR_RR_INTERVAL_DATA_PRESENT "rr_interval_data_present"
#define JSON_KEY_HR_DATA_SENSOR_HEART_RATE "heart_rate"
#define JSON_KEY_HR_DATA_SENSOR_CONTACT_DETECTED "sensor_contact_detected"
#define JSON_KEY_HR_DATA_SENSOR_ENERGY_EXPENDED "energy_expended"
#define JSON_KEY_HR_DATA_SENSOR_RR_INTERVALS "rr_intervals"

#define HEART_RATE_SERVICE_UUID "0x180D"
#define HEART_RATE_MEASUREMENT "0x2A37"

#define HEART_RATE_VALUE_FORMAT (1)
#define SENSOR_CONTACT_DETECTED (2)
#define SENSOR_CONTACT_FEATURE (4)
#define ENERGY_EXPENDED_FEATURE (8)
#define RR_INTERVAL_DATA (16)

typedef struct _bt_device_information_t {
    char address[DEVICE_INFO_ADDR_SIZE+1]; // '\0' terminated string
    char name[DEVICE_INFO_BUFF_SIZE+1];    // '\0' terminated string
} bt_device_information_t;

typedef struct _event_names_t {
    int id;
    const char *name;
} event_names_t;

#endif /* LocalTypes_H_ */
