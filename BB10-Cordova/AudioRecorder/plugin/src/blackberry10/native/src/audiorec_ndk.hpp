/*
* Copyright 2013-2014 Research In Motion Limited.
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

#ifndef AUDIOREC_HPP_
#define AUDIOREC_HPP_

#include <string>
#include <pthread.h>
#include <bb/multimedia/AudioRecorder>
#include <QtCore>

class AudiorecJS;

namespace webworks {

class AudiorecNDK {
public:
	explicit AudiorecNDK(AudiorecJS *parent = NULL);
	virtual ~AudiorecNDK();

	// The extension methods are defined here


	void record();

	void pause();

	void reset();

	void setOutput(const std::string& inputString);

	std::string mediaState();

	std::string duration();

private:
	AudiorecJS *m_pParent;
	bb::multimedia::AudioRecorder recorder;
};

} // namespace webworks

#endif /* AUDIORECNDK_H_ */
