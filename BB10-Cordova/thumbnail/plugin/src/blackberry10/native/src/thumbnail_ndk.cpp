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
#include "thumbnail_ndk.hpp"
#include "thumbnail_js.hpp"
#include <img/img.h>
#include <stdio.h>
#include <cstdlib>

namespace webworks {

ThumbnailNDK::ThumbnailNDK(ThumbnailJS *parent) :
	m_pParent(parent)
{
}

ThumbnailNDK::~ThumbnailNDK() {
	delete m_pParent;
}

std::string ThumbnailNDK::createThumbNail(std::string path, int width, int height, unsigned int quality, int flag) {

	img_lib_t ilib = NULL;
	int rc;
	if ((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
	                std::fprintf(stderr, "img_lib_attach() failed: %d\n", rc);
	                return "";
	}

	img_t img;
	img.w = width;
	img.h = height;
	img.flags = flag | IMG_QUALITY;
	img.quality = quality;
	int resizeResult = img_load_resize_file( ilib, path.c_str(), NULL, &img );

	std::string tempFileName = convertLongToString(clock()) + path.substr(path.find_last_of("."), path.length());
	std::string tempFilePath = std::string(getcwd(NULL, 0)) + "/" + TMP_PATH + tempFileName;
	int writeResult = img_write_file( ilib, tempFilePath.c_str(), NULL, &img );
	img_lib_detach(ilib);


	return tempFilePath;
}
std::string ThumbnailNDK::convertLongToString(long l) {
	std::stringstream ss;
	ss << l;
	return ss.str();
}

} /* namespace webworks */
