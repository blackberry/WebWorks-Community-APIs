Clipboard Extension
=============

This BlackBerry 10 Cordova Plugin provide simple clipboard access functions like getText and setText. 

** Tested On **

BlackBerry Dev Alpha C / 10.2.0.429

** Author **

[Merrick Zhang](https://github.com/anphorea)

[About Me](http://about.me/anpho)

## Structure of the Plugin and Sample

There are two parts to this project - the plugin and a sample:

The plugin is contained in the _plugin_ folder. This plugin can be imported into a WebWorks 2.0 application using the command line tools.
The sample code is included in the _sample_ folder. The code here is meant to be dropped into a WebWorks 2.0 default template project and it shows the API in use.


## Building and Testing the Sample

The included sample app is the _www_ directory of the default WebWorks Hello World application created by the __webworks create__ command. Create a new WebWorks project using the create command, and copy the _www_ folder into it, overwriting the existing one. Then add the plugin to your project with the command __WebWorks plugin add <path to the plugin folder>__. Your project is ready to run on your simulator simply by calling __webworks run__ in the project directory. The sample code in www has been altered to include a textarea in _index.html_ for displaying the test data, and a set of test functions in _js/index.js_ to excercise the Clipboard API and display some results.

To use the plugin in another project, that's been created with WebWorks, run __webworks plugin add <path to the plugin folder>__

Then you can call the methods with the namespace __community.clipboard__, and that should appear in WebInspector as an Object so you can see what APIs are available.

## The Plugin API
The Clipboard plugin provides the following API:

```javascript
var str="something you want to copy to clipboard";
var result = community.clipboard.setText(str);
//result shows the bytes copied into clipboard, if result=="-1",then something is wrong.

var str = community.clipboard.getText;
// str is the content in the clipboard.
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
4. You should see the shared libraries (libclipboard3.so files) generated in the folders for each Build Configuration that you selected.

### Using the Plugin in an Application

To use the plugin in another project, that's been created with webworks, run __webworks plugin add <plugin path>__.
That will copy the plugin into the project, and update the www/config.xml file to include the feature as below:

```xml
<feature name="community.clipboard" value="community.clipboard" />
```

All the methods in the plugin will be prefixed by that feature name, so a method called _getText()_ supplied in the _community.clipboard_ plugin will be called in JavaScript like so:

```javascript
community.clipboard.getText();
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
