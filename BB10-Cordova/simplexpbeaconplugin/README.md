# Bluetooth iBeacon Cross Platform Cordova Plugin #

This Bluetooth Smart iBeacon WebWorks Cordova Cross Platform Plugin (**SimpleXpBeaconPlugin**) is intended to allow  WebWorks applications to monitor and detect devices using the Apple de-facto iBeacon standard. It supports all the following platforms:

- **BlackBerry 10**
- **Android**
- **iOS**

in this release.

There is a document in the **/documents** folder describing the plugin's API from JavaScript.

It comes with a sample WebWorks application in the form of a simple **iBeacon Monitor** application that uses the Bluetooth iBeacon Plugin to monitor for iBeacons.

This plugin supersedes the older verion: [SimpleBeaconPlugin](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/simplebeaconplugin). I had made some design choices in that version that I did not want to propagate to this cross-platfrom version. If you have made use of the older version ([SimpleBeaconPlugin](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/simplebeaconplugin)) and want to move to this cross-platform version then please review the API documentation.

### Applies To

* [Apache Cordova for BlackBerry 10](https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10)

### Author

* [John Murray](https://github.com/jcmurray)

### Release History

* **V1.0.0** - Initial release
* **V1.1.0** - Added iOS support

### Known Issues

* None

### Dependencies

#### BlackBerry 10

1. BlackBerry Device Software **10.2.1** and greater for the BlackBerry 10 native component.
2. The native component of the BlackBerry 10 plugin in GitHub has been built against BlackBerry Device Software **10.3.1** as this was the most recent version of BlackBerry 10 at the time of posting to GitHub.
3. If you want to run it on an earlier version of BlackBerry 10 Device Software you should follow the instructions below to rebuild it yourself.

#### Android

1. Android **API Level 18** is necessary for this plugin to operate correctly since the plugin makes use of Bluetooth Low Enery APIs that became available in this level.
2. The Android device must support the **PackageManager.FEATURE_BLUETOOTH_LE** Bluetooth Low Energy feature to allow the plugin to listen to iBeacon Advertisements.

#### iOS

1. I've tested the sample application and plugin on iOS **8.1** and **8.4** where it builds and executes correctly.
2. The iOS device must suport Bluetooth Low Energy ( determined via **CBCentralManager** state).

# Required Plugins

None.

## The Plugin ##

The plugin itself is located in this repo here:

	<repo>/SimpleXpBeaconPlugin/plugin

There is a sample application that uses thus plugin in this repo here:

	<repo>/SimpleXpBeaconPlugin/sample

Clone this repo to your local machine.

#### BlackBerry 10

You have the choice of immediately using the plugin, which has been built against **10.3.1**, or, building the plugin yourself -- you may want to do this to enable it to run on **10.2.1** and earlier. In this case just import the project at this path:

	<repo>/SimpleXpBeaconPlugin/plugin/src/blackberry10/native

into Momentics and build it against the SDK of your choice ( **10.2.1**, **10.3.0** or **10.3.1** ) for **both** ***device*** and ***simulator*** targets.

#### Android

Ensure that you have the [Android SDK](https://developer.android.com/sdk/index.html) installed. For convenience I also find it useful to have the following directories added to the command search path (%PATH% on Windows(TM) or $PATH on Unix(TM), or Mac(TM) variants) where `<ANDROID-SDKS>` is the installation for the Android SDK on your system:

```
<ANDROID-SDKS>/tools
<ANDROID-SDKS>/platform-tools
<ANDROID-SDKS>/build-tools/xx.y.z
```
(In my case I used **xx.y.z** = **22.0.1**).

The sample code for this plugin and application is Open Source under 
the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

#### iOS

Ensure that you have a suitable version of **XCode* and **Command Line Tools** installed on your Mac. I built this and ran tests using **XCode 6.4** on **Yosemite 10.10.4**.

You will need to open the Xcode project (*.xcodeproj* file) that **WebWorks (Cordova)** generates from the sample application and make some configuration changes before building the sample app itself (see below).

## The Sample Application ##

Before using the sample application pelase take some time to review the plugin documentation. There is a difference in behaviour between iOS and both BlackBerry 10 and Android in terms of how iBeacon events are genered. iOS **mandates** that you specify a set of iBeacon characteristics that you want to match such as UUID, Major or Minor numbers and **no** iBeacon events will be reported unless you specify this. Android and BlackBerry 10 will on the other hand report **all** iBeacon events unless you specify a specific set of filters to restrict this behaviour.

The sample application is set up by default to monitor for iBeacons wuth a UUID of **"8AEFB031-6C32-486F-825B-E26FA193487D"** simply because that's the UUID of the iBeacon I used to test the application and plugin. You should review this and decide how you want the applictaion to behave in your own testing since I'm sure you won't have an iBeacon with the same UUID as mine.

So, having noted that, if you want to run the sample iBeacon Monitor application here's what you need to do.

You'll need to re-create the project using the latest templates and plugins and then simply add the application specific code to your project. 

Ensure the [BlackBerry 10 WebWorks SDK 2.2](https://developer.blackberry.com/html5/download/sdk) is correctly installed. I used this version of WebWorks but you may be using a newer version when you build this project: 

```
$ webworks -v
v2.2.0.15
```

Open a command prompt (windows) or terminal (mac) in a folder of your choosing and follow the following instructions:

- Create a new WebWorks project in a folder of your own choosing. I'll call it `<SAMPLE>` in what follows.
- `cd <SAMPLE>` and create a WebWorks project in this folder something like:

	```
	$ webworks create mytest com.sample.mytest "My Test"
	```

- `cd mytest` and replace the whole ***www*** folder at:

	```
	<SAMPLE>/mytest/www
	```
	
	with the one at:

	```
	<repo>/SimpleXpBeaconPlugin/sample/www
	```

- Select the platform(s) that you want to use - by default WebWorks will add BlackBerry 10 as a platform. If you want to use Android add it as a platform like this:

	```
	$ webworks platform add android
	```
	
	You can check what platforms have been added like this:
	
	```
	$ webworks platform list
	Installed platforms: android 3.5.0, blackberry10 3.6.0
	Available platforms: amazon-fireos, firefoxos, ios
	```

- Add the Bluetooth plugin using this command: 

	```
	$ webworks plugin add <repo>/SimpleXpBeaconPlugin/plugin
	```	

- You can verify that the plugin is installed correctly

	```
	$ webworks plugin ls
	``` 

- Build and run the sample application on an attached device.
	- For BlackBerry 10 use:

		```
		$ webworks build
		$ webworks run	
		```
	
	- For Android use:
	
		```
		$ webworks build android
		$ webworks run android
		```	
	
	- For iOS you need to add a couple of extra steps:
	
		```
		$ webworks build ios
		```	
	
		Then open the XCode project that was build by this step:

		```
		$ open ./platforms/ios/My\ Test.xcodeproj
		```	

		You then need to make the following changes to the project:
		
		- Add the **CoreBluetooth.framework** to the project to allow access to Bluetooth Low Enery APIs.
		- Add the following **Capabilities** to the project:
			- **Uses Bluetooth LE Accessories**
			- **Location Updates**
		- Add the following keys and values to the **My Test-Info.plist** file:

			```
			...
    		<key>NSLocationAlwaysUsageDescription</key>
    		<string>Testing iBeacon service</string>
    		<key>NSLocationWhenInUseUsageDescription</key>
    		<string>Testing iBeacon service</string>
    		...
    		```
		You can change the strings: **"Testing iBeacon service"** to something more suitable since these represent the prompts that the user wull see when the application is installed regarding these specific authorisations. It's always a good idea to explain to the user precisely why specific authorisations are needed.

- If you change the plugin you will need to remove it and add it again using:

	```
	$ webworks plugin remove cordova-plugin-beacon
	$ webworks plugin add <repo>/SimpleXpBeaconPlugin/plugin
	```	

### Notes on the Sample Application

The sample application depends on **Bacon.js** licensed under the [MIT License](http://opensource.org/licenses/MIT) and **jQuery** licensed under the [jQuery License](https://github.com/jquery/jquery/blob/master/LICENSE.txt).

The **SimpleXpBeaconPlugin** itself has **NO** dependency on either of these frameworks.

### More Info

* [BlackBerry HTML5 WebWorks](https://bdsc.webapps.blackberry.com/html5/) - Downloads, Getting Started guides, samples, code signing keys.
* [BlackBerry WebWorks Development Guides](https://bdsc.webapps.blackberry.com/html5/documentation)
* [BlackBerry WebWorks Community Forums](http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/bd-p/browser_dev)
* [BlackBerry Open Source WebWorks Contributions Forums](http://supportforums.blackberry.com/t5/BlackBerry-WebWorks/bd-p/ww_con)
 
**To contribute code to this repository you must be [signed up as an 
official contributor](http://blackberry.github.com/howToContribute.html).**

## Contributing Changes

Please see the [README](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/README.md) of the WebWorks-Community-APIs repository for instructions on how to add new Samples or make modifications to existing Samples.

## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) for the Sample and send a message (via github messages) to the Sample Author(s) to let them know that you have filed an [Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues).


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.c