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

#ifndef BARCODESCANNERNDK_HPP_
#define BARCODESCANNERNDK_HPP_

#include <camera/camera_api.h>

class BarcodeScannerJS;

namespace webworks {

std::string YUV_NV12_TO_RGB(const unsigned char* yuv, int width, int height);
std::string getBitmapHeader(const unsigned int width, const unsigned int height);

class BarcodeScannerNDK {
public:
    explicit BarcodeScannerNDK(BarcodeScannerJS *parent = NULL);
    virtual ~BarcodeScannerNDK();

    int stopRead();
    int startRead();



private:
    BarcodeScannerJS *m_pParent;
    camera_handle_t mCameraHandle;
    int filecounter;
    #define TMP_PATH "tmp/"
    uint32_t rotation;
//    img_lib_t ilib = NULL;

};

} // namespace webworks

#endif /* BARCODESCANNERNDK_H_ */
