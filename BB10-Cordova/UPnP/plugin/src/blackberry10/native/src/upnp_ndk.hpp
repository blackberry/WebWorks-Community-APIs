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

#ifndef UPnP_NDK_HPP_
#define UPnP_NDK_HPP_

#include <string>
#include <pthread.h>

#define ISXDIGIT(x) (isxdigit((int) ((unsigned char) x)))

class UPnP_JS;

namespace webworks {

class UPnP_NDK {
public:
	explicit UPnP_NDK(UPnP_JS *parent = NULL);
	virtual ~UPnP_NDK();

	// The extension methods are defined here

	std::string discover(const std::string& inputString);

private:
	UPnP_JS *m_pParent;
	int uPnPProperty;
	int uPnPThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

} // namespace webworks

#endif /* UPnP_NDK_HPP_ */
