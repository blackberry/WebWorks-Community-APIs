# SystemDialog #

This BlackBerry 10 Cordova Plugin prompts a global notification dialog. The notification also triggers effects vibration and LED flashing. The effects are dependent on the notification settings of the call. 

## Contents ##

* [Native Extension](plugin/src/blackberry10/native) - BlackBerry 10 Native SDK Project
* [Sample Application](sample) - WebWorks 2.0/Cordova Project

## Including the feature in your application

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-bb-sysdialog

Installation from source is the same but instead of the id ("cordova-plugin-bb-sysdialog"), use the file system path to the source plugin folder.

## Javascript API ##

The api exports a global module as blackberry.community.sysdialog

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

See the [Sample Application](sample) for an full example of how to use the API.


## How to build your native Plugin

1.  Clone this repo to your local machine

2.  Ensure the [BlackBerry 10 Native SDK](https://developer.blackberry.com/native/download/sdk) is correctly installed

3.  Import the native extension project into momentics. It is located at `/plugin/src/blackberry10/native` within **this project**

4.  Build the native extension project for all architectures: Right click on the project and choose "Build Configurations" > "Build All"

Note: When build on the Mac/Linux platform, you will have the build directory contents as shown below. The "libSysDialog.so" is a symlink to the actual shared object "libSysDialog.so.1.0.0" in order to manage build version. Thus copying your build to the Windows platform and adding the plugin will NOT work. You need to rebuild on Windows, or remove the symlinks and rename you shared object "libSysDialog.so.1.0.0" to "libSysDialog.so".

	lrwxr-xr-x   libSysDialog.so@ -> libSysDialog.so.1.0.0
	lrwxr-xr-x   libSysDialog.so.1@ -> libSysDialog.so.1.0.0
	lrwxr-xr-x   libSysDialog.so.1.0@ -> libSysDialog.so.1.0.0
	-rwxr-xr-x   libSysDialog.so.1.0.0*


## Using the Plugin in an Application

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the SysDialog folder>/plugin__.

All the methods in the plugin will be prefixed by that feature name, so a method called _show()_ supplied in the _community.sysdialog_ plugin will be called in JavaScript like so:

community.sysdialog.show();

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

