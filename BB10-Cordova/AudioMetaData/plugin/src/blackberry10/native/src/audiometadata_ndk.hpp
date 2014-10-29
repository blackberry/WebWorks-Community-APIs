/*
* Copyright (c) 2013 BlackBerry Limited
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

#ifndef AudioMetaData_NDK_HPP_
#define AudioMetaData_NDK_HPP_

#include <json/reader.h>
#include <json/writer.h>
#include <string>
#include <pthread.h>

class AudioMetaData_JS;

namespace webworks {

class AudioMetaData_NDK {
public:
	explicit AudioMetaData_NDK(AudioMetaData_JS *parent = NULL);
	virtual ~AudioMetaData_NDK();

	// The extension methods are defined here
	void audioMetaDataGetMetaData(const std::string& callbackId, const std::string& inputString);

private:
	Json::Value parseMp3ForMetaData(const char* path);
	std::string getProperString(char* strArray, int size, char encoding);

	AudioMetaData_JS *m_pParent;
	std::string threadCallbackId;
};

} // namespace webworks

#endif /* AudioMetaData_NDK_HPP_ */
