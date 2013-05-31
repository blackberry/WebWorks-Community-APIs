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
#include <pthread.h>
#include "template_ndk.hpp"
#include "template_js.hpp"

#include <bb/device/DisplayInfo>

#define MMPERINCH 25.4;

namespace webworks {

TemplateNDK::TemplateNDK(TemplateJS *parent) {
}

TemplateNDK::~TemplateNDK() {
}


// These methods are the true native code we intend to reach from WebWorks

std::string TemplateNDK::SDgetSize() {
	bb::device::DisplayInfo display;
	Json::FastWriter writer;
	Json::Value root;

	double physx = display.physicalSize().width();
	double physy = display.physicalSize().height();
	int pixx = display.pixelSize().width();
	int pixy = display.pixelSize().height();
	double ppmm = 0;
	double ppmmx = 0;
	double ppmmy = 0;
	double pshape = 0;

	double physdiag = 0; // Diagonal metrics
	double pixdiag = 0;

	if((pixx > 0) && (pixy > 0) && (physx > 0) && (physy > 0)) {
		ppmmx = pixx / physx;
		ppmmy = pixy / physy;
		pshape = (ppmmx / ppmmy);
		physdiag = sqrt((physx * physx) + (physy * physy));
		pixdiag = sqrt((pixx * pixx) + (pixy * pixy));
		ppmm = pixdiag / physdiag;
	}

	root["physicalWidth"] = physx;
	root["physicalHeight"] = physy;
	root["pixelWidth"] = pixx;
	root["pixelHeight"] = pixy;
	root["ppmm"] = ppmm;
	root["ppmmX"] = ppmmx;
	root["ppmmY"] = ppmmy;
	root["ppi"] = ppmm * MMPERINCH;
	root["ppiX"] = ppmmx * MMPERINCH;
	root["ppiY"] = ppmmy * MMPERINCH;
	root["pixelShape"] = pshape;

 	return writer.write(root);
}


} /* namespace webworks */
