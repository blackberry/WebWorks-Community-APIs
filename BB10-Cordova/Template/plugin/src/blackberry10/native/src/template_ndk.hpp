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

#ifndef TEMPLATENDK_HPP_
#define TEMPLATENDK_HPP_

#include <string>
#include <pthread.h>

class TemplateJS;

namespace webworks {

class TemplateNDK {
public:
	explicit TemplateNDK(TemplateJS *parent = NULL);
	virtual ~TemplateNDK();

	// The extension methods are defined here
	std::string templateTestString();

	std::string templateTestString(const std::string& inputString);

	std::string getTemplateProperty();

	void setTemplateProperty(const std::string& inputString);

	void templateTestAsync(const std::string& callbackId, const std::string& inputString);

	std::string templateStartThread(const std::string& callbackId);

	std::string templateStopThread();

	bool isThreadHalt();

	void templateThreadCallback();

private:
	TemplateJS *m_pParent;
	int templateProperty;
	int templateThreadCount;
	bool threadHalt;
	std::string threadCallbackId;
	pthread_t m_thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

} // namespace webworks

#endif /* TEMPLATENDK_H_ */
