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
#include <math.h>
#include <json/writer.h>
#include <json/reader.h>
#include <pthread.h>
#include "template_ndk.hpp"
#include "template_js.hpp"

#include <stdlib.h>
#include "miniupnpc.h"
#include "miniwget.h"
#include "upnpcommands.h"

// Remove %HH from strings
char* urldecode(const char *str) {
	char *ns;
	int l;
	int i;
	int k = 0;
	unsigned long int hexcode;

	l = strlen(str);
	ns = (char*) malloc(sizeof(char) * l);
	memset(ns, 0, l);

	// Convert %hh encoded data
	for(i = 0; i < l; i++) {
		if((i <= (l - 3)) && (str[i] == '%') && ISXDIGIT(str[i + 1]) && ISXDIGIT(str[i + 2])) {
			char hexstr[3];
			char *ptr;
			hexstr[0] = str[i + 1];
			hexstr[1] = str[i + 2];
			hexstr[2] = 0;
			hexcode = strtoul(hexstr, &ptr, 16);
			hexcode = (hexcode & (unsigned long) 0xFF);
			if((hexcode != 34) && (i > 0) && (i < (l - 3))) { // Kill leading or trailing double quotes
				ns[k] = (unsigned char) hexcode;
				k++;
			}
			i += 2;
		} else {
			ns[k] = str[i];
			k++;
		}

	}

	return ns;
}



namespace webworks {

TemplateNDK::TemplateNDK(TemplateJS *parent) {
}

TemplateNDK::~TemplateNDK() {
}


// These methods are the true native code we intend to reach from WebWorks

std::string TemplateNDK::discover(const std::string& arg) {
	Json::FastWriter writer;
	Json::Value res;
	Json::Reader reader;
	Json::Value root;

    struct UPNPDev *devlist = NULL;
    int upnperr = 0;
	char* descXML;
	char* service;
	int descXMLsize = 0;
	int idx = 0;
	long int timeout = 2000; // 2 seconds default
	char *deviceList = NULL;

	bool parse = reader.parse(arg, root);
 	if (parse) {
   		  timeout = (int) strtoul(root["timeout"].asCString(), NULL, 10);
   		  deviceList = urldecode(root["devtype"].asCString());
 	}

	devlist = upnpDiscover(timeout, deviceList, NULL, NULL, 0, 0, &upnperr);
    {
    	struct UPNPDev * device;
    	struct UPNPUrls urls;
    	struct IGDdatas data;
    	if(devlist)
    	{
    		for(device = devlist; device; device = device->pNext)
    		{
        		descXML = (char *) miniwget(device->descURL, &descXMLsize, 0);
        		if(descXML)
        		{
        			service = (char*) malloc (descXMLsize + 1);
            		strncpy(service, descXML, descXMLsize);
            		res["device"][idx]["url"] = device->descURL;
            		res["device"][idx++]["xml"] = service;
        			free(descXML); descXML = NULL;
        			free(service);
       			}
    		}
    	}
    	else
    	{
    		fprintf(stdout, "{ error: %d }", upnperr);
    	}

    }

	res["success"] = true;
	res["devcount"] = idx;
	res["timeout"] = (float) (timeout);
	res["devtype"] = deviceList;

	free(deviceList);

 	return writer.write(res);
}


} /* namespace webworks */
