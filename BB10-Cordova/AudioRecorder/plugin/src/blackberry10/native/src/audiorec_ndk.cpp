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

#include <string>
#include <sstream>
#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "audiorec_ndk.hpp"
#include "audiorec_js.hpp"


namespace webworks {

AudiorecNDK::AudiorecNDK(AudiorecJS *parent) {
	m_pParent = parent;

}

AudiorecNDK::~AudiorecNDK() {
}

// These methods are the true native code we intend to reach from WebWorks

//start the audio recorder
void AudiorecNDK::record(){
	recorder.record();
}

//set the file path for the recording
void AudiorecNDK::setOutput(const std::string& inputString){
	QString file =  QString::fromStdString(inputString);
	recorder.setOutputUrl(QUrl(file));
}

//pause the recording
void AudiorecNDK::pause(){
	recorder.pause();
}

//reset/stop the recording
void AudiorecNDK::reset(){
	recorder.reset();
}

//returns the state of the recorder
std::string AudiorecNDK::mediaState(){
	stringstream ss;
	ss << recorder.mediaState();
	string state = ss.str();
	return state;
}

//returns the current duration of the recording
std::string AudiorecNDK::duration(){
	stringstream ss;
	ss << recorder.duration();
	string dur = ss.str();
	return dur;
}

} /* namespace webworks */
