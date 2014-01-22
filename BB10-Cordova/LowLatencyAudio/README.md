## PGLowLatencyAudio Plugin for BlackBerry10, using Cordova. 

Plays audio .wav files using openAL, ALUT, Qdir, and QtCore libraries.

For a sample application demonstrating the use of this API, please see the [LowLatencySequencer](https://github.com/blackberry/Cordova-Samples/tree/master/LowLatencySequencer)

The plugin is currently under the name of PGLowLatencyAudio, and can be installed using the following command in your Cordova project location:

    cordova/plugin add path-to-plugin/PGLowLatencyAudio

To start off, in the directory PGLowLatencyAudio/www contains the API. Every function is called using PGLowLatencyAudio, and we have the follow functions: 

    preloadAudio: function (data, path, voices, success, fail) {
        exec(success, fail, service, "preloadAudio", { data: data, path: path, voices: voices });
    },
    unload: function (data, success, fail) {
        exec(success, fail, service, "unload", { data: data });
    },
    play: function (data, success, fail) {
        exec(success, fail, service, "play", { data: data });
    },
    stop: function (data, success, fail) {
        exec(success, fail, service, "stop", { data: data });
    },
    getDuration: function (data, success, fail) {
        exec(success, fail, service, "getDuration", { data: data });
    },
    loop: function (data, success, fail) {
        exec(success, fail, service, "loop", { data: data });
    }

Note: In the exampled below we use the success call back to display messages for debugging purposes. To remove these messages, simply remove 'alert(echoValue)' from the function call, however if you do run into troubles, these call backs should provide useful information to debug. If you wanted you could include a fail function to each of these, for example your fail function could be function(){alert("FAIL")};

## Examples: 
If in our Cordova Applications www application folder, we have the path assets/sounds, to our sound location we can do the following:

----------------------------------------------------------------------------------

### preloadAudio
Preloads an audio file into a buffer so it can be played automatically without loading. Takes in three parameters, the file name, the path to the sounds, and how many unique voices it can have. Voices are how many simultaneous sounds that can be played at once.

Example:

	PGLowLatencyAudio.preloadAudio("bounce.wav", "assets/sounds/", 1, 	function(echoValue) {
		alert(echoValue);},
	});

----------------------------------------------------------------------------------

### unload
Unloads an audio file from the current buffer it holds up. Used to free and allocate space in openAL. Takes in one parameter, file name.






Example:
  
	PGLowLatencyAudio.unload("bounce.wav", function(echoValue) {
		alert(echoValue);
	});

----------------------------------------------------------------------------------

### play
Plays an audio file using openAL. Takes in one parameter, file name.

Example:
 
	PGLowLatencyAudio.play("bounce.wav", function(echoValue) {
		alert(echoValue);
	});   

----------------------------------------------------------------------------------

### stop
Stops an audio file from playing or looping. Takes one parameter, file name.

Example: 

	PGLowLatencyAudio.stop("bounce.wav", function(echoValue) {
		alert(echoValue); 
	});
----------------------------------------------------------------------------------

### getDuration
Get the duration of a file. Takes one parameter, file name.

Example: 

	PGLowLatencyAudio.getDuration("bounce.wav", function(duration) {
		if (duration > 6.0) {
			alert(“Greater than 6”);
		} else {
			alert(“Less than 6”);
		} 
	});

----------------------------------------------------------------------------------

### loop

Loops an audio file indefinitely until either unloaded or stopped. Takes in one parameter, file name.

Example: 

	PGLowLatencyAudio.loop("background.wav", function(echoValue) {
		alert(echoValue);
	});       

----------------------------------------------------------------------------------

The Native portion of the plugin is called using JNEXT.invoke, and inputting either of the following strings, 'load', 'unload', 'play', 'stop', 'loop'.


For more information about how to run and build Cordova applications, refer to:
https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/LowLatencyAudio?pixel)](https://github.com/igrigorik/ga-beacon)