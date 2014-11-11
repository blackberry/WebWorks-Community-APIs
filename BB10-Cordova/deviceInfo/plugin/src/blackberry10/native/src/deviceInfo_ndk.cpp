/*
 * Copyright (c) 2013 - 2014 BlackBerry Limited
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
#include <bps/netstatus.h>
#include <bb/device/SimCardInfo>
#include <bb/device/HardwareInfo>
#include <bb/device/CellularNetworkInfo>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "deviceInfo_ndk.hpp"
#include "deviceInfo_js.hpp"

namespace webworks {


DeviceinfoNDK::DeviceinfoNDK(DeviceinfoJS *parent):
	m_pParent(parent){
    bps_initialize();
}

DeviceinfoNDK::~DeviceinfoNDK() {
    bps_shutdown();
}

std::string DeviceinfoNDK::getModelNumber() {
	bb::device::HardwareInfo hwInfo;
	std::string str = hwInfo.modelName().toLocal8Bit().data();
	return str;
}

std::string DeviceinfoNDK::getMCC() {
	bb::device::SimCardInfo simInfo;
	return simInfo.mobileCountryCode().toLocal8Bit().data();
}

std::string DeviceinfoNDK::getRoamingStatus() {
	bb::device::CellularNetworkInfo netInfo;
	return netInfo.isRoaming() ? "true" : "false";
}

std::string DeviceinfoNDK::getMNC() {
	bb::device::SimCardInfo simInfo;
	return simInfo.mobileNetworkCode().toLocal8Bit().data();
}

std::string DeviceinfoNDK::isSimulator() {
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

std::string DeviceinfoNDK::getNetwork() {
	Json::FastWriter writer;
	Json::Value res;
	netstatus_interface_details_t *details;

	if(BPS_SUCCESS == netstatus_get_interface_details(NULL, &details)) {
		netstatus_ip_status_t nstatus = netstatus_interface_get_ip_status(details);
		netstatus_interface_type_t ntype = netstatus_interface_get_type(details);
		bool nisconnected = netstatus_interface_is_connected(details);
		bool nisup = netstatus_interface_is_up(details);

		int nipcnt = netstatus_interface_get_num_ip_addresses(details);

		res["status"] = true;
		res["ifname"] = std::string(netstatus_interface_get_name(details));
		res["ifup"] = nisup;
		res["ifconnected"] = nisconnected;
		res["iftype"] = ntype;
		res["ifstatus"] = nstatus;
		res["ifipcount"] = nipcnt;

		for(int i=0; i<nipcnt; i++) {
			res["iplist"][i]["ifaddress"] = std::string(netstatus_interface_get_ip_address(details, i));
			res["iplist"][i]["ifnetmask"] = std::string(netstatus_interface_get_ip_address_netmask(details, i));
		}

		netstatus_free_interface_details(&details);
	} else {
		res["status"] = false;
	}

	return writer.write(res);
}

std::string DeviceinfoNDK::hasPhysicalKeyboard() {
	bb::device::HardwareInfo hwInfo;

	return hwInfo.isPhysicalKeyboardDevice() ? "1" : "0";
}


} /* namespace webworks */
