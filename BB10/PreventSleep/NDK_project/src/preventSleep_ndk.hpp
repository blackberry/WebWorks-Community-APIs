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

#ifndef PreventSleepNDK_HPP_
#define PreventSleepNDK_HPP_

#include <string>

class PreventSleepJS;

namespace webworks {

class PreventSleepNDK {
public:
	explicit PreventSleepNDK(PreventSleepJS *parent = NULL);
	virtual ~PreventSleepNDK();

	// The extension methods are defined here
	std::string setPreventSleep(const std::string& inputString);

	std::string isPreventSleep();


private:
	PreventSleepJS *m_pParent;
	bool preventSleepIsOn;
};

} // namespace webworks

#endif /* PreventSleepNDK_H_ */
