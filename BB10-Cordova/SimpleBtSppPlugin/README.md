# Bluetooth SPP WebWorks Plugin #


This Bluetooth SPP WebWorks Plugin (**SimpleBtSppPlugin**) is intended to allow  WebWorks applications to connect to devices that uses the Bluetooth SPP profile.

There is a document in the **/documents** folder describing the plugin's API from JavaScript.

It comes with a sample WebWorks application in the form of a simple **Chat** application that uses SPP to communicate between two BlackBerry 10 devices.

###Applies To

* [Apache Cordova for BlackBerry 10](https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10)

###Author

* [John Murray](https://github.com/jcmurray)

###Release History

* **V1.1.0** - Support for bt\_spp\_open\_ex() and bt\_spp\_open\_server\_ex() in **10.3.0** and above allowing access to the RFCOMM API.
* **V1.0.1** - Minor bugs fixed in plugin
* **V1.0.0** - Initial release

###Known Issues

* None

###Dependencies

1. BlackBerry Device Software **10.2.1** and greater for the native component.
2. BlackBerry Device Software **10.3.0** and above is required for access to the RFCOMM API. 
1. BlackBerry WebWorks **2.1** and greater.

###Required Plugins

None.

## The Plugin ##

The plugin itself is located in this repo here:

	<repo>/SimpleBtSppPlugin/plugin

There is a sample application that uses thus plugin in this repo here:

	<repo>/SimpleBtSppPlugin/sample

Clone this repo to your local machine.

You have the choice of immediately using the plugin, which has been built against **10.2.1**, or, building the plugin yourself -- you may want to do this to enable access to the RFCOMM API additions that become available in **10.3.0**. In this case just import the project at this path:

	<repo>/SimpleBtSppPlugin/plugin/src/blackberry10/native

into Momentics and build it against the SDK of your choice (**10.2.1** or **10.3.0**) for **both** ***device*** and ***simulator*** targets.

The sample code for this plugin and application is Open Source under 
the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

## The Sample Application ##

If you want to run the sample Chat application here's what you need to do. You have two options.

### Install the pre-built and signed version ###

There is a pre-built, signed, release build of the application in the folder: 

	<repo>/SimpleBtSppPlugin/installable-bar-files

It's built using the plugin as built against 10.2.1. 

### Build it yourself ###

You'll need to re-create the project using the latest templates and plugins and then simply add the application specific code to your project. 

Ensure the [BlackBerry 10 WebWorks SDK 2.1](https://developer.blackberry.com/html5/download/sdk) is correctly installed. I used this version of WebWorks but you may be using a newer version when you build this project: 

	```
	> webworks -v
	v2.1.0.13
	```

Open a command prompt (windows) or terminal (mac) in a folder of your choosing and follow the following instructions:

- Create a new WebWorks project in a folder of your own choosing. I'll call it `<SAMPLE>` in what follows.
- `cd <SAMPLE>` and create a WebWorks project in this folder something like:

	```
	webworks create mytest com.sample.mytest "My Test"
	```

- `cd mytest` and replace the whole ***www*** folder at:

	```
	<SAMPLE>/mytest/www
	```
	
	with the one at:

	```
	<repo>/SimpleBtSppPlugin/sample/www
	```

- Add the Bluetooth plugin using this command: 

	```
	webworks plugin add <repo>/SimpleBtSppPlugin/plugin
	```	

- You can verify that the plugin is installed correctly

	```
	webworks plugin ls
	``` 

- Build and run the sample application on an attached device using:

	```
	webworks run
	```

- If you change the plugin you will need to remove it and add it again using:

	```
	webworks plugin remove com.blackberry.community.simplebtspplugin
	```
	```
	webworks plugin add <repo>/SimpleBtSppPlugin/plugin
	```	

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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.