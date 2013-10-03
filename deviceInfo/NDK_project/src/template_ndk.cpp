/*
 * Copyright 2013 Research In Motion Limited.
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
#include <bps/bps.h>
#include <bps/deviceinfo.h>
#include <bb/device/SimCardInfo>
#include <bb/device/HardwareInfo>
#include <bb/device/CellularNetworkInfo>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "template_ndk.hpp"
#include "template_js.hpp"

namespace webworks {

DeviceInfo::DeviceInfo(TemplateJS *parent) {
	m_pParent = parent;
	bps_initialize();
}

std::string DeviceInfo::getModelNumber() {
	bb::device::HardwareInfo hwInfo;
	std::string str = hwInfo.modelName().toLocal8Bit().data();
	return str;
}

std::string DeviceInfo::getMCC() {
	bb::device::SimCardInfo simInfo;
	return simInfo.mobileCountryCode().toLocal8Bit().data();
}

std::string DeviceInfo::getRoamingStatus() {
	bb::device::CellularNetworkInfo netInfo;
	return netInfo.isRoaming() ? "true" : "false";
}

std::string DeviceInfo::getMNC() {
	bb::device::SimCardInfo simInfo;
	return simInfo.mobileNetworkCode().toLocal8Bit().data();
}

std::string DeviceInfo::isSimulator() {
    deviceinfo_details_t *details;

    if (BPS_SUCCESS == deviceinfo_get_details(&details)) {
        if(deviceinfo_details_is_simulator(details)) {
        	return "true";
        } else {
        	return "false";
        }
        deviceinfo_free_details(&details);
    }
	return "null";
}

DeviceInfo::~DeviceInfo() {
	bps_shutdown();
}


} /* namespace webworks */
