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

#define MAX_OPTIONS 53
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include "json/reader.h"
#include "json/writer.h"
#include "mongooseintf.h"
#include "mongoose.h"
#include "Mongoose_ndk.hpp"
#include "Mongoose_js.hpp"


namespace webworks {

int is_running = false;

MongooseNDK::MongooseNDK(MongooseJS *parent):m_pParent(parent) {
		m_pParent->getLog()->info("Mongoose Created");
}

MongooseNDK::~MongooseNDK() {
	MGstop();
}


// These methods are the true native code we intend to reach from WebWorks

std::string MongooseNDK::MGstart(const std::string& arg) {
	int res = 0;
	char *options[MAX_OPTIONS];
	Json::Reader reader;
	Json::FastWriter writer;
	Json::Value root;
	Json::Value rval;


	m_pParent->getLog()->debug("MgStartRunning");

	char ptemp[MAXPATHLEN]; // Build default document root in case missing
	getcwd(ptemp, MAXPATHLEN);
	strcat(ptemp,"/data");


	bool parse = reader.parse(arg, root);
	for(unsigned int i=0; i<MAX_OPTIONS; i++) {
		options[i] = NULL;
	}
	if(is_running) {
		rval["status"] = false;
		rval["error"] = "Already Running";
	} else {

 	if (parse) {
 		int xops = 0;

  		if(!root.isMember("document_root")) {
   		  options[xops++] = sdup("document_root");
   		  options[xops++] = sdup(ptemp);
 		}

  		if(!root.isMember("enable_directory_listing")) {
  			options[xops++] = sdup("enable_directory_listing");
     		options[xops++] = sdup("no");
 		}
 		Json::Value::Members memberNames = root.getMemberNames();
 		int ecount = 0;

  		for(unsigned int i=0; i<memberNames.size(); i++) {
  			std::string memberName = memberNames[i];
			options[xops++] = sdup(memberName.c_str());
			options[xops++] = urldecode(root[memberName].asCString());
  		}

  		m_pParent->getLog()->info("Starting Mongoose");
		ecount = start_mongoose(options);

	    for (int idx = 0; options[idx] != NULL; idx++) {
 		  free(options[idx]);
 		  }

 		if(ecount >= 0) {
 			rval["status"] = true;
 			rval["error"] = false;
 			rval["command_errors"] = ecount;

 	 	 	rval["document_root"] = mongoose_get_option("document_root");
 	 	 	rval["listening_ports"] = mongoose_get_option("listening_ports");

 	 	 	is_running = true;

 		} else {
 			ecount = 0 - (ecount + 1);
 			rval["status"] = false;
 			rval["error"] = "Unable to start server";
 			rval["command_errors"] = ecount;
 		}

	} else {
		rval["status"] = false;
		rval["error"] = "Unable to parse JSON";
	}

	}

	return writer.write(rval);
}

std::string MongooseNDK::MGstop() {
	Json::FastWriter writer;
	Json::Value rval;

	if(is_running) {
	  m_pParent->getLog()->info("Mg was stopped.");
	  stop_mongoose();
	}
	else {
		m_pParent->getLog()->info("Mg was not stopped.");
	}

	is_running = false;
	rval["status"] = true;
	return writer.write(rval);
}


} /* namespace webworks */
