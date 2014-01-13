Prevent Sleep
=============

This BlackBerry 10 Cordova Plugin keeps the screen from dimming while the application is in the foreground. It can be turned on or off and a property is set that is _true_ when sleep is being prevented, and _false_ otherwise.

** Tested On **

BlackBerry Q5 10.2.0.1157

** Author **

[Tim Windsor](https://github.com/timwindsor)

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.preventsleep

or,
	
	webworks plugin add com.blackberry.community.preventsleep

Installation from source is the same but instead of the id ("com.blackberry.community.preventsleep"), use the file system path to the source plugin folder.


## Structure of the Plugin and Sample

There are two parts to this project - the plugin and a sample:

The plugin is contained in the _plugin_ folder. This plugin can be imported into a PhoneGap or Cordova application using the command line tools.
The sample code is included in the _sample_ folder. The code here is meant to be dropped into a PhoneGap or Cordova default template project and it shows the API in use.


## Building and Testing the Sample

The included sample app is the _www_ directory of the default Cordova Hello World application created by the __cordova create__ command. Create a new Cordova project using the create command, and copy the _www_ folder into it, overwriting the existing one. Make sure you've either added the BlackBerry 10 SDK to your PATH, or run bbndk-env.bat (Win) or bbndk-env.sh (Mac/Linux), then add the BlackBerry10 platform to your project using __cordova platform add blackberry10__. Then add the plugin to your project with the command __cordova plugin add <path to the PreventSleep folder>/plugin__. Your project is ready to run on your simulator simply by calling __cordova run__ in the project directory. The sample code in www has been altered to include a div in _index.html_ for displaying the test data, and a set of test functions in _js/index.js_ to excercise the Prevent Sleep API and display some results.

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the PreventSleep folder>/plugin__

Then you can call the methods with the namespace __community.preventsleep__, and that should appear in WebInspector as an Object so you can see what APIs are available.

This is a screenshot of the test data being displayed in the Hello World sample app:
![Screenshot](screenshot.png)

## The Plugin API
The Prevent Sleep plugin provides the following API:

```javascript
var setting = true; // boolean
var result = community.preventsleep.setPreventSleep(setting);
// Result is a string value for debugging

var currentSetting = community.preventsleep.isSleepPrevented;
// boolean value for the state of the screen
```

## Building the plugin from source

Copy the plugin folder to a location on your computer to start working with it.

### Momentics NDK setup

You can either import the project from the Template folder, or use the New Project Wizard in Momentics to create a starter project.

#### Importing the Template

1. Open the Momentics IDE. Navigate to the workbench and from the program menu
select File -> Import and choose "Existing Projects into Workspace".
2. Choose "Select root directory: " and browse to the _/plugin/src/blackberry10/native_ directory where you copied the plugin. Select the PreventSleep project in the Projects list and uncheck "Copy projects into workspace". Click Finish.
3. Follow these next steps to build the plugin to be sure the setup is working.

#### How to build your native Plugin

1. Right click your project and select the Clean Project option.
2. Right click your project again and select Build Configurations -> Build Selected... .
3. A window will appear that shows all the available build configurations
for the project. Select __device__ and __simulator__ and click ok.
4. You should see the shared libraries (libPreventSleep.so files) generated in the folders for each Build Configuration that you selected.

### Using the Plugin in an Application

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the PreventSleep folder>/plugin__.
That will copy the plugin into the project, and update the www/config.xml file to include the feature as below:

```xml
<feature name="community.preventsleep" value="community.preventsleep" />
```

All the methods in the plugin will be prefixed by that feature name, so a method called _test()_ supplied in the _community.templateplugin_ plugin will be called in JavaScript like so:

```javascript
community.templateplugin.setPreventSleep(true);
```

### Modifying the Plugin

See the examples in the [Template Extension](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/Template) for how to add additional features to this extension.

#### Rebuild 
When making changes, rebuild regularly so you don't make a really hard to find typo.

Follow the steps above to:
1. [Build the native portion](#how-to-build-your-native-plugin), and
2. [Use the plugin in your test app](#using-the-plugin-in-an-application).

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/PreventSleep?pixel)](https://github.com/igrigorik/ga-beacon)