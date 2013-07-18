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

#define MAX_OPTIONS 53
#define MAX_CONF_FILE_LINE_SIZE (8 * 1024)

#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "template_ndk.hpp"
#include "template_js.hpp"
#include "mongooseintf.h"
#include "mongoose.h"

namespace webworks {

int is_running = false;

TemplateNDK::TemplateNDK(TemplateJS *parent) {
}

TemplateNDK::~TemplateNDK() {
	MGstop();
}


// These methods are the true native code we intend to reach from WebWorks

std::string TemplateNDK::MGstart(const std::string& arg) {
	int res = 0;
	char *options[MAX_OPTIONS];
	Json::Reader reader;
	Json::FastWriter writer;
	Json::Value root;
	Json::Value rval;

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
  		if(!root.isMember("document_root")) {
 		    root["document_root"] = ptemp;
 		}

  		if(!root.isMember("enable_directory_listing")) {
 		    root["enable_directory_listing"] = "no";
 		}

 		Json::Value::Members memberNames = root.getMemberNames();
 		int ecount = 0;


  		for(unsigned int i=0; i<memberNames.size(); i++) {
 		  std::string memberName = memberNames[i];
 		  options[i++] = sdup(memberName.c_str());
 		  options[i] = urldecode(root[memberName].asCString());
 		  }

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

std::string TemplateNDK::MGstop() {
	Json::FastWriter writer;
	Json::Value rval;

	if(is_running) {
	  stop_mongoose();
	}

	is_running = false;
	rval["status"] = true;
	return writer.write(rval);
}


} /* namespace webworks */


