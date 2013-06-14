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

#ifndef VIBRATIONNDK_HPP_
#define VIBRATIONNDK_HPP_

#include <string>
#include <pthread.h>

class VibrationJS;

namespace webworks {

class VibrationNDK {
public:
	explicit VibrationNDK(VibrationJS *parent = NULL);

	virtual ~VibrationNDK();

	void vibrationRequest(const std::string& inputString);

private:
	VibrationJS *m_pParent;
};

} // namespace webworks

#endif /* VIBRATIONNDK_H_ */
