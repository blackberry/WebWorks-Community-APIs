Curl Extension
=============

This BlackBerry 10 Cordova Plugin providea simple HTTP-GET function using libcurl.

** Tested On **

BlackBerry Dev Alpha C / 10.2.0.429

** Author **

[Merrick Zhang](https://github.com/anphorea)

[About Me](http://about.me/anpho)

## why I built this ?

There's something called Cross-Domain limit. I can use this to bypass the limit.

The server doesn't refuse client side http request mostly.

I'm really not good at C++, therefore I hope someone can make this plugin better.

## Version History

	1.0.0 First release
	1.0.1 Asynchronous operation

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.curl

or,
	
	webworks plugin add com.blackberry.community.curl

Installation from source is the same but instead of the id ("com.blackberry.community.curl"), use the file system path to the source plugin folder.


## Structure of the Plugin and Sample

There are two parts to this project - the plugin and a sample:

The plugin is contained in the _plugin_ folder. This plugin can be imported into a WebWorks 2.0 application using the command line tools.
The sample code is included in the _sample_ folder. The code here is meant to be dropped into a WebWorks 2.0 default template project and it shows the API in use.


## Building and Testing the Sample

The included sample app is the _www_ directory of the default WebWorks Hello World application created by the __webworks create__ command. Create a new WebWorks project using the create command, and copy the _www_ folder into it, overwriting the existing one. Then add the plugin to your project with the command __WebWorks plugin add <path to the plugin folder>__. Your project is ready to run on your simulator simply by calling __webworks run__ in the project directory. The sample code in www has been altered to include a textarea in _index.html_ for displaying the test data, and a set of test functions in _js/index.js_ to excercise the Curl  API and display some results.

To use the plugin in another project, that's been created with WebWorks, run __webworks plugin add <path to the plugin folder>__

Then you can call the methods with the namespace __community.curl__, and that should appear in WebInspector as an Object so you can see what APIs are available.

## The Plugin API
The Curl plugin provides the following API:

```javascript
var str="http://google.com";
var result = community.curl.get(str);
//result shows the content of the URL,timeout is 3 seconds.
```

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/Curl?pixel)](https://github.com/igrigorik/ga-beacon)