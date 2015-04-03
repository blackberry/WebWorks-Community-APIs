# MediaKeys Plugin #

This BlackBerry 10 Cordova Plugin allows developers to utilize the native signal and slots Media Keys API for BlackBerry 10.

## Inclusion of plugin into application

This API can be installed through the following commands:

Registry:

	cordova plugin add com.blackberry.community.mediakeys

Source:

	cordova plugin add path-to-plugin/MediaKeys/plugin/

## JavaScript API

The api exports a global module as community.mediakeys

### Object Methods ###

#### bind(mediaKeys, generalFailure) ####

parameters:

	mediaKeys {Object}
	The array of media keys or a single media key that is to be binded. 

		mediaKeys.mediakey {String}
		The media key that is to be binded with.
			Available options: [fastForward, pause, play, playPause, rewind, stop, volumeDown, volumeUp, previous, next]

		mediaKeys.keylength {String}
		The media key threshold to trigger the media key. 
			Available options: [short, long]

		mediaKeys.onPressed {function}
		The callback function that is called upon the key being pressed.

	generalFailure {Function}
	A general failure function that will be invoked whenever a error occurs in the process of binding with media keys.

## Examples

Binding One Media Key:

	var mediaKeyObj = {
		mediakey: "volumeUp",
		keylength: "short",
		onPressed: function() { app.writeOut("bind volume up (short) succeeded"); }
	};

	var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };
	community.mediakeys.bind(mediaKeyObj, generalFailure);

Binding Multiple Media Keys:

	var mediaKeyUpShort = {
		mediakey: "volumeUp",
		keylength: "short",
		onPressed: function() { app.writeOut("bind volume up (short) succeeded"); }
	};
	var mediaKeyDownShort = {
		mediakey: "volumeDown",
		keylength: "short",
		onPressed: function() { app.writeOut("bind volume down (short) succeeded"); }
	};
	var mediaKeyUpLong = {
		mediakey: "volumeUp",
		keylength: "long",
		onPressed: function() { app.writeOut("bind volume up (long) succeeded"); }
	};

	var generalFailure = function (error) { app.writeOut("unable to bind media keys: "+error); };
	var keyBindings = [mediaKeyUpShort, mediaKeyDownShort,mediaKeyUpLong];
	community.mediakeys.bind(keyBindings, generalFailure);

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.