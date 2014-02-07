Extension-Vibration
==================

BlackBerry 10 WebWorks Extensions create a bridge between JavaScript code in an HTML5 application and C/C++ and/or Qt code in the extension, allowing access to native libraries, which are not part of the HTML5 specification. The BlackBerry WebWorks APIs included with the WebWorks SDK are also Extensions and follow the same format as shown in this template.

Extensions and WebWorks APIs offer a JavaScript API which communicates through an interface to native methods. This interface can send and receive data through strings, or data that can be represented as a string such as JSON and Base64.

This vibration extension allows for more comprehensive control over device vibrations

1. A function to change both length and intensity of the vibration

## Including the feature in your application

This API can be installed from the source by copying the plugin folder to your webworks project, then add the plugin with the following commands:

	cordova plugin add plugin

or,
	
	webworks plugin add plugin

community.vibration folder should appear under the plugins folder of your project.

### Using the Extension in an Application</a>

With the extension copied into your WebWorks SDK as explained above, you can use it in your applications by referencing it as a feature in your config.xml file. 

```xml
<feature id="community.vibration" required="true" version="1.0.0.0" />
```

```javascript
var data = { "duration" : [int: duration], "intensity" : [int: intensity]}
community.vibration.vibration_request(data, [function: callback])
```

## Summary
var data = { "duration" : [int: duration], "intensity" : [int: intensity]}
community.vibration.vibration_request(data, [function: callback])

Parameters:  
duration : length of vibration ( 0 - 5000)
intensity : level of vibration ( 1 , 10 , 100 )

Example:

	<script type="text/javascript" >
		var data = { "duration" : 1000 , "intensity" : 100 };
		community.vibration.vibration_request(data, null);
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
