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
#ifndef LocalTypes_H_
#define LocalTypes_H_

#include <string>
#include <errno.h>

#define PLUGIN_VERSION "1.1.0"

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
#define JSON_KEY_PLUGIN_VERSION "plugin_version"
#define JSON_KEY_DATA "data"
#define JSON_VALUE_ERROR "ERROR"
#define JSON_VALUE_OK "OK"
#define JSON_VALUE_THREAD "thread"
#define JSON_VALUE_FUNCTION "function"
#define JSON_VALUE_CONNECTION "CONNECTION"
#define JSON_VALUE_DISCONNECTION "DISCONNECTION"
#define JSON_VALUE_STARTED "STARTED"
#define JSON_VALUE_IBEACON "IBEACON"
#define JSON_KEY_IBEACON_UUID "uuid"
#define JSON_KEY_IBEACON_MAJOR "major"
#define JSON_KEY_IBEACON_MINOR "minor"
#define JSON_KEY_IBEACON_RSSI "rssi"
#define JSON_KEY_IBEACON_TXPOWER "txpower"
#define EMPTY_RETURN_STRING ""

typedef struct _bt_device_information_t {
    char address[DEVICE_INFO_ADDR_SIZE+1]; // '\0' terminated string
    char name[DEVICE_INFO_BUFF_SIZE+1];    // '\0' terminated string
} bt_device_information_t;

typedef struct _event_names_t {
    int id;
    const char *name;
} event_names_t;

#endif /* LocalTypes_H_ */
