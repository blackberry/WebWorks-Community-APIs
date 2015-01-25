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
#include "Util.hpp"

void Util::flushFileDescriptor(int fd)
{
    for (;;) {
        char ch;
        if (::read(fd, &ch, 1) == -1) {
            if (errno == EAGAIN) {
                break;
            }
        }
    }
}

const char *Util::btEventName(const int id)
{
    const event_names_t descriptions[] = {
            { BT_EVT_ACCESS_CHANGED, "BT_EVT_ACCESS_CHANGED" },
            { BT_EVT_RADIO_SHUTDOWN, "BT_EVT_RADIO_SHUTDOWN" },
            { BT_EVT_RADIO_INIT, "BT_EVT_RADIO_INIT" },
            { BT_EVT_CONFIRM_NUMERIC_REQUEST, "BT_EVT_CONFIRM_NUMERIC_REQUEST" },
            { BT_EVT_PAIRING_COMPLETE, "BT_EVT_PAIRING_COMPLETE" },
            { BT_EVT_DEVICE_ADDED, "BT_EVT_DEVICE_ADDED" },
            { BT_EVT_DEVICE_DELETED, "BT_EVT_DEVICE_DELETED" },
            { BT_EVT_SERVICE_CONNECTED, "BT_EVT_SERVICE_CONNECTED" },
            { BT_EVT_SERVICE_DISCONNECTED, "BT_EVT_SERVICE_DISCONNECTED" },
            { BT_EVT_FAULT, "BT_EVT_FAULT" },
            { BT_EVT_DEVICE_CONNECTED, "BT_EVT_DEVICE_CONNECTED" },
            { BT_EVT_DEVICE_DISCONNECTED, "BT_EVT_DEVICE_DISCONNECTED" },
            { BT_EVT_NAME_UPDATED, "BT_EVT_NAME_UPDATED" },
            { BT_EVT_LE_DEVICE_CONNECTED, "BT_EVT_LE_DEVICE_CONNECTED" },
            { BT_EVT_LE_DEVICE_DISCONNECTED, "BT_EVT_LE_DEVICE_DISCONNECTED" },
            { BT_EVT_LE_NAME_UPDATED, "BT_EVT_LE_NAME_UPDATED" },
            { BT_EVT_SERVICES_UPDATED, "BT_EVT_SERVICES_UPDATED" },
            { BT_EVT_GATT_SERVICES_UPDATED, "BT_EVT_GATT_SERVICES_UPDATED" },
            { BT_EVT_LE_GATT_SERVICES_UPDATED, "BT_EVT_LE_GATT_SERVICES_UPDATED" },
            { BT_EVT_PAIRING_DELETED, "BT_EVT_PAIRING_DELETED" },
#if BBNDK_VERSION_AT_LEAST(10,3,0)
            { BT_EVT_PAIRING_STARTED, "BT_EVT_PAIRING_STARTED" },
            { BT_EVT_PAIRING_FAILED, "BT_EVT_PAIRING_FAILED" },
#endif
            { BT_EVT_UNDEFINED_EVENT, "BT_EVT_UNDEFINED_EVENT" }
    };

    for (int i = 0; i < (int)(sizeof(descriptions)/sizeof(event_names_t)); i++) {
        if (descriptions[i].id == id) {
            return descriptions[i].name;
        }
    }
    return "Unknown Event";
}

float Util::convertToFloat(uint32_t value) {
     int32_t mantissa = value & 0x00FFFFFF;
     int32_t exponent = ((int32_t) value) >> 24;
     if (mantissa & 0x00800000) {
         mantissa -= FLOAT_MAX_MANTISSA;
     }
     return ((float)mantissa * powf(10.0, (float)exponent));
}

