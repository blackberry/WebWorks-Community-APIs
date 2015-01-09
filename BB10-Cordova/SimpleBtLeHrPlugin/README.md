# Bluetooth Heart Rate Profile WebWorks Plugin #


This Bluetooth Smart heart Rate Profile WebWorks Plugin (**SimpleBtLeHrPlugin**) is intended to allow  WebWorks applications to connect to devices that uses the Bluetooth Smart Heart Rate Monitor profile.

There is a document in the **/documents** folder describing the plugin's API from JavaScript.

It comes with a sample WebWorks application in the form of a simple **Heart Rate Monitor** application that uses the Bluetooth Smart Heart Rate Plugin to communicate with a suitable Heart Rate monitor.

###Applies To

* [Apache Cordova for BlackBerry 10](https://github.com/blackberry/cordova-blackberry/tree/master/blackberry10)

###Author

* [John Murray](https://github.com/jcmurray)

###Release History

* **V1.0.0** - Initial release

###Known Issues

* None

###Dependencies

1. BlackBerry Device Software **10.2.1** and greater for the native component.

###Required Plugins

None.

## The Plugin ##

The plugin itself is located in this repo here:

	<repo>/SimpleBtLeHrPlugin/plugin

There is a sample application that uses thus plugin in this repo here:

	<repo>/SimpleBtLeHrPlugin/sample

Clone this repo to your local machine.

You have the choice of immediately using the plugin, which has been built against **10.2.1**, or, building the plugin yourself -- you may want to do this to enable access to features that become available in **10.3.0** and above. In this case just import the project at this path:

	<repo>/SimpleBtBtHrPlugin/plugin/src/blackberry10/native

into Momentics and build it against the SDK of your choice (**10.2.1**, **10.3.0** or **10.3.1**) for **both** ***device*** and ***simulator*** targets.

The sample code for this plugin and application is Open Source under 
the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

## The Sample Application ##

If you want to run the sample Heart Rate Monitor application here's what you need to do.

You'll need to re-create the project using the latest templates and plugins and then simply add the application specific code to your project. 

Ensure the [BlackBerry 10 WebWorks SDK 2.2](https://developer.blackberry.com/html5/download/sdk) is correctly installed. I used this version of WebWorks but you may be using a newer version when you build this project: 

	```
	> webworks -v
	v2.2.0.15
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
	<repo>/SimpleBtLeHrPlugin/sample/www
	```

- Add the Bluetooth plugin using this command: 

	```
	webworks plugin add <repo>/SimpleBtLeHrPluginPlugin/plugin
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
	webworks plugin remove com.blackberry.community.simplebtlehrplugin
	```
	```
	webworks plugin add <repo>/SimpleBtLeHrPlugin/plugin
	```	

### Notes on the Sample Application

The sample application depends on **Bacon.js** licensed under the [MIT License](http://opensource.org/licenses/MIT) and **jQuery** licensed under the [jQuery License](https://github.com/jquery/jquery/blob/master/LICENSE.txt).

The **SimpleBtLeHrPlugin** itself has **NO** dependency on either of these frameworks.

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
