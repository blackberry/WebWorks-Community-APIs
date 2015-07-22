Gamepad Plugin
==============

This BlackBerry 10 WebWorks 2.0 plugin adds support for Gamepads, as a shim for the HMTL5 Gamepad API

### Tested On

BlackBerry Z10 10.2.1.2943

### Authors

[Peardox](http://supportforums.blackberry.com/t5/user/viewprofilepage/user-id/325249)

[Tim Windsor](https://github.com/timwindsor)

## Building and Testing the Sample

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-gamepad

or,
	
	webworks plugin add cordova-plugin-gamepad

Installation from source is the same but instead of the id ("cordova-plugin-gamepad"), use the file system path to the source plugin folder.

When the application runs the screen pictured below will appear. Joypad button presses and movements will be reflected in the demo.

* If a controller is connected or disconnected, the list will update.

![Screenshot](joypad.png)

## Structure of the Plugin and Sample

There are two parts to this project - the plugin and a sample:

The plugin is contained in the _plugin_ folder. This plugin can be imported into a PhoneGap or Cordova application using the command line tools.
The sample code is included in the _sample_ folder. The code here is meant to be dropped into a PhoneGap or Cordova default template project and it shows the API in use.


## Building and Testing the Sample

The included sample app is the _www_ directory of the test application created by the __webworks create__ command. Create a new WebWorks 2 project using the create command, and replace the _www_ folder with the one from this sample. Then add the plugin to your project with the command __webworks plugin add <path to the Gamepad folder>/plugin__. Then use __webworks run__ to build and run the application.

To use the plugin in another project, that's been created with WebWorks, run __webworks plugin add <path to the Gamepad folder>/plugin__

## The Extension API
The Gamepad Extension works as a shim for the [HTML5 Gamepad API](http://www.w3.org/TR/2014/WD-gamepad-20140225/).

You will need to wait for the "deviceready" event as it depends on Cordova being loaded, but after that, follow the standard for getting Gamepad data.

Listen to the window for "gamepadconnected" and "gamepaddisconnected" events:

```
window.addEventListener('gamepadconnected',function(event) {
	console.log("Gamepad was connected: " + event.gamepad.id);
	updateGamepadList();
});
window.addEventListener('gamepaddisconnected',function(event) {
	console.log("Gamepad was disconnected: " + event.gamepad.id);
	updateGamepadList();
});

```

Read controller values from the navigator.getGamepads() method. It's recommended that you do so on AnimationFrame updates like so:

```
function readGamepads() {
	window.requestAnimationFrame(readGamepads);
	if (!navigator.getGamepads) {
		return;
	}
	var gamepads = navigator.getGamepads();
	if (gamepads) {
		for (var i = 0; i<gamepads.length; i++) {
			readGamepad(gamepads[i]);
		}
	}
}
```

The Gamepad object follows the spec as close as possible:

* id: The descriptive name of the controller, or the raw id values if it's not a known controller
* index: The index of the controller in the list of connected controllers (up to 2 are supported)
* connected: true if the controller is still connected
* timestamp: an increasing counter updated every time a new update is received from the controller
* mapping: always the empty string as this extension sends the raw hardware mapping without doing any remapping
* buttons[]: an array of the button values. Each takes the format: { "pressed" : true/false, "value": 1.0/0.0 }
* axes[]: an array of all the axes the controller supports. Some Analog buttons can appear here. Format is a value from -1.0 -> 1.0

As this extension is a shim for the HTML5 spec, if you code your application to work based on this implementation, it should be highly portable and automatically work if at some point the Browser engine gets native support.

## The Gamepad Button and Axis Mapping

The arrays of buttons and axes that are returned in the Gamepad Object are will be remapped to the HTML5 "standard" mapping as long as the device has a known mapping. The __mapping__ attribute of the Gamepad object will be set to "standard" in this case. Otherwise the values will be returned in the raw mapping that the BlackBerry OS provides for the connected gamepad. In that case, the __mapping__ attribute will be set to the empty string.

The sample app shows the order of the supported buttons and axes, following the HTML5 standard mapping.

## Building the extension from source

Copy the extension folder to a location on your computer to start working with it.

### Momentics NDK setup

1. Open the Momentics IDE. Navigate to the workbench and from the program menu
select File -> Import and choose "Existing Projects into Workspace".
2. Choose "Select root directory: " and browse to the NDK_project directory where you copied the extension. Select the joypad project in the Projects list and uncheck "Copy projects into workspace". Click Finish.
3. Follow these next steps to build the extension to be sure the setup is working.

#### How to build your native Extension</a>

1. Right click your project and select the Clean Project option.
2. Right click your project again and select Build Configurations -> Build Selected... .
3. A window will appear that shows all the available build configurations
for the project. Select device and simulator and click ok.
4. You should see the shared libraries generated in the folders for each Build Configuration that you selected.



**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
