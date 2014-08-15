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

#ifndef ???PROJECT_NAME???_NDK_HPP_
#define ???PROJECT_NAME???_NDK_HPP_

#include <string>
#include <pthread.h>

class ???PROJECT_NAME???_JS;

namespace webworks {

class ???PROJECT_NAME???_NDK {
public:
	explicit ???PROJECT_NAME???_NDK(???PROJECT_NAME???_JS *parent = NULL);
	virtual ~???PROJECT_NAME???_NDK();

	// The extension methods are defined here
	std::string ???PROJECT_FUNCTION_START???Test();

	std::string ???PROJECT_FUNCTION_START???Test(const std::string& inputString);

	std::string get???PROJECT_FUNCTION???Property();

	void set???PROJECT_FUNCTION???Property(const std::string& inputString);

	void ???PROJECT_FUNCTION_START???TestAsync(const std::string& callbackId, const std::string& inputString);

	std::string ???PROJECT_FUNCTION_START???StartThread(const std::string& callbackId);

	std::string ???PROJECT_FUNCTION_START???StopThread();

	bool isThreadHalt();

	void ???PROJECT_FUNCTION_START???ThreadCallback();

private:
	???PROJECT_NAME???_JS *m_pParent;
	int ???PROJECT_FUNCTION_START???Property;
	int ???PROJECT_FUNCTION_START???ThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

} // namespace webworks

#endif /* ???PROJECT_NAME???_NDK_HPP_ */
