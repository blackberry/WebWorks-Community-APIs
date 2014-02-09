LED
=============

This BlackBerry 10 Cordova Plugin flashes the RGB LED on a Blackberry phone. It can flash it using startLed for any integer number of times >= 0, where setting the value to 0 means that the LED flashes indefinitely. It can be flashed any possible hex color. It can be turned off with a stopLed function. 

** Tested On **

BlackBerry Q5 10.2.1.1927

Date: 2/9/2014

** Author **

[Deric Cheng](https://github.com/dericc)

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.led

or,
	
	webworks plugin add com.blackberry.community.led

Installation from source is the same but instead of the id ("com.blackberry.community.led"), use the file system path to the source plugin folder.


## Structure of the Plugin and Sample

There are two parts to this project - the plugin and a sample:

The plugin is contained in the _plugin_ folder. This plugin can be imported into a PhoneGap or Cordova application using the command line tools.
The sample code is included in the _sample_ folder. The code here is meant to be dropped into a PhoneGap or Cordova default template project and it shows the API in use.


## Building and Testing the Sample

The included sample app is the _www_ directory of the default Cordova Hello World application created by the __cordova create__ command. Create a new Cordova project using the create command, and copy the _www_ folder into it, overwriting the existing one. Make sure you've either added the BlackBerry 10 SDK to your PATH, or run bbndk-env.bat (Win) or bbndk-env.sh (Mac/Linux), then add the BlackBerry10 platform to your project using __cordova platform add blackberry10__. Then add the plugin to your project with the command __cordova plugin add <path to the Led folder>/plugin__. Your project is ready to run on your simulator simply by calling __cordova run__ in the project directory. The sample code in www has been altered to include a div in _index.html_ for displaying the test data, and a set of test functions in _js/index.js_ to excercise the Led API and display some results.

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the Led folder>/plugin__

Then you can call the methods with the namespace __community.led__, and that should appear in WebInspector as an Object so you can see what APIs are available.

## The Plugin API
The Led plugin provides the following API:

```javascript
var id = community.led.startLed(color, blinkCount); 
```
This function calls the LED on a Blackberry phone to blink "blinkCount" number of times with color "color". 
The variable "color" is defined as a 6-character hex string, for example "FF0000" is the equivalent hex string for a red light.
The variable "blinkCount" is an integer defined as the number of times the LED will blink before turning off. Setting blinkCount to 0 indicates that the light will blink until stopLed is called. 
The function startLed() returns the id of the LED process. 

```javascript
community.led.stopLed(id); 
```
This function stops the LED from blinking if it is currently blinking. 
The variable id is the ID of the LED process. 

## Building the plugin from source

Copy the plugin folder to a location on your computer to start working with it.

### Momentics NDK setup

You can either import the project from the Template folder, or use the New Project Wizard in Momentics to create a starter project.

#### Importing the Template

1. Open the Momentics IDE. Navigate to the workbench and from the program menu
select File -> Import and choose "Existing Projects into Workspace".
2. Choose "Select root directory: " and browse to the _/plugin/src/blackberry10/native_ directory where you copied the plugin. Select the Led project in the Projects list and uncheck "Copy projects into workspace". Click Finish.
3. Follow these next steps to build the plugin to be sure the setup is working.

#### How to build your native Plugin

1. Right click your project and select the Clean Project option.
2. Right click your project again and select Build Configurations -> Build Selected... .
3. A window will appear that shows all the available build configurations
for the project. Select __device__ and __simulator__ and click ok.
4. You should see the shared libraries (libLed.so files) generated in the folders for each Build Configuration that you selected.

### Using the Plugin in an Application

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the Led folder>/plugin__.
That will copy the plugin into the project, and update the www/config.xml file to include the feature as below:

```xml
<feature name="community.led" value="community.led" />
```

All the methods in the plugin will be prefixed by that feature name, so a method called _startLed()_ supplied in the _community.led_ plugin will be called in JavaScript like so:

community.led.startLed(); 

### Modifying the Plugin

See the examples in the [Template Extension](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/Template) for how to add additional features to this extension.

#### Rebuild 
When making changes, rebuild regularly so you don't make a really hard to find typo.

Follow the steps above to:
1. [Build the native portion](#how-to-build-your-native-plugin), and
2. [Use the plugin in your test app](#using-the-plugin-in-an-application).

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
