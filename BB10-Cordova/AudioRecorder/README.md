Audio Recorder Plugin for Webworks 2.0
==================

Author: James Grisdale

This plugin allows use of the native audio recorder features
It includes all of the features necessary to make an audio recording, although there is further work that can be done to this plugin to add more functionality.

## How To Install The Plugin

This API can be installed for the webworks 2.0 platform through the following method:
	
	webworks plugin add plugin

## Functions Involved in This Plugin (currently)

record() - Prepares the recorder and begins recording audio.

pause() - Pauses the recorder

reset() - Stops the recording

setOuput(String filename) - Takes a file path for the recording to be saved. Must be called before calling record the first time.

mediaState() - Returns the state of the recorder

	- 0 - Unprepared - All media resources have not been acquired.
	- 1 - Prepared - All media resources have been acquired. 
	- 2 - Started - Media is actively progressing. 
	- 3 - Paused - Media progress is interrupted and will resume from its current position. 
	- 4 - Stopped - Media progress is stopped. 
	- Values are returned as a string.
	
duration() - The current duration of the recording (returns 0 if in an unprepared or stopped state). values are returned as a string

## Notes on Compiling Native Source and Adding to a Webworks Project

When compiling the source, to use the audiorecorder api

	#include <bb/multimedia/AudioRecorder>
	#include <QtCore>
	
are required to be added to the audiorec_ndk.hpp

	#include <QtCore> could cause some errors, to fix this follow the steps below:

- Right click on your project in project explorer/choose properties
- Expand C/C++ General, then click on Paths and Symbols
- Change Configuration to All configurations
- Then click Includes, select GNU C in languages
- Click Add, and add the following two lines:
- ${QNX_TARGET}/usr/include/qt4
- ${QNX_TARGET}/usr/include/qt4/QtCore
- Click OK and that should fix the errors.


When attempting to add the AudioRecorder plugin to your webworks project two things need to be added to your config.xml file.
	
	<rim:permissions>
		<rim:permit>record_audio</rim:permit>
		<rim:permit>access_shared</rim:permit>
	</rim:permissions>

These two permissions are important so the program can access the microphone and so that it can save the file for the recording.


### Using the Extension in an Application

After the above has been completed the functions for the plugin are pretty basic.

Before attempting to record a file path for the recording must be set, this is easy:
```javascript
var file = "file:///accounts/1000/shared/voice/recording.m4a";
ommunity.audiorecorder.setOutput(file);
```
Parameter: file - The file path where you want to save the recording

To prepare the recorder and begin recording:
```javascript
community.audiorecorder.record();
```

If you wish to pause the recording, use:
```javascript
community.audiorecorder.pause();
```

To end and save your recording and free up the recorder resources
```javascript
community.audiorecorder.reset();
```

mediaState returns the state of the recording, for example so you know when it is recording or paused.
```javascript
if (community.audiorecorder.mediaState() == "2")
	community.audiorecorder.pause();//pause only if currently recording
```

duration can be called while the recording is being made so keep track of how long the recording is in milliseconds.
```javascript
document.write(community.audiorecorder.duration());//returns as a string
```


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.