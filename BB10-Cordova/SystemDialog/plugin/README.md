# SystemDialog #

This BlackBerry 10 Cordova Plugin prompts a global notification dialog. The notification also triggers effects vibration and LED flashing. The effects are dependent on the notification settings of the call. 

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.sysdialog

or,
	
	webworks plugin add com.blackberry.community.sysdialog


## Javascript API ##

The api exports a global module as community.sysdialog

### Object Methods ###

#### show(message, buttons, settings, onOptionSelected, onFail) ####

A notification that will be shown to the user using a dialog box. 

parameters:

	message {String}
	The message Message to be displayed in the dialog.

	buttons {Object}
	The buttons Array of string choices that will be presented to the user in the form of buttons.

	settings {Object}
	Optional Object literal that allows the user to manipulate the title and notification effects (LED flashing and vibration) repeat.

		settings.title {String}
		Desired title of the dialog.

		settings.repeat {Boolean}
		The preference for repeating notification settings. Default is false.

	onOptionSelected {Function}
	Optional callback function that will be invoked when the user makes a selection. 
	Expected signature: function onOptionSelected(selectedButtonIndex).

	onFail {Function}
	Optional callback function that will be invoked if error occurs when create and show the dialog.
	Expected signature: function onFail(error).

Example:

	var message = "This is the body of the dialog box. It asks the user to confirm an action."
	var buttons = ["Maybe", "Cancel", "ok"];
	var settings = {title: "My Dialog Box Title", repeat : true}
	var onSuccess = function(index) {console.log( "success: button " + buttons[index] + " is selected."); };
	var onFail = function(error) {console.log( "fail: " + error); };
	community.sysdialog.show(msg, buttons, settings, onSuccess, onFail);


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

