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

#ifndef QRCODEREADERNDK_HPP_
#define QRCODEREADERNDK_HPP_

#include <camera/camera_api.h>

class QRCodeReaderJS;

namespace webworks {

std::string getBMPHeader(const unsigned int width, const unsigned int height);

class QRCodeReaderNDK {
public:
    explicit QRCodeReaderNDK(QRCodeReaderJS *parent = NULL);
    virtual ~QRCodeReaderNDK();
    int startQRCodeRead(const std::string& inputString);
    static int stopQRCodeRead();
    std::string templateStartThread();
    std::string templateStopThread();

    static QRCodeReaderJS *m_pParent;

private:
    static camera_handle_t mCameraHandle;

};

} // namespace webworks

#endif /* QRCODEREADERNDK_H_ */
