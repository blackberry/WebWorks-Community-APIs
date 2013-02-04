Extension-Vibration
==================

BlackBerry 10 WebWorks Extensions create a bridge between JavaScript code in an HTML5 application and C/C++ and/or Qt code in the extension, allowing access to native libraries, which are not part of the HTML5 specification. The BlackBerry WebWorks APIs included with the WebWorks SDK are also Extensions and follow the same format as shown in this template.

Extensions and WebWorks APIs offer a JavaScript API which communicates through an interface to native methods. This interface can send and receive data through strings, or data that can be represented as a string such as JSON and Base64.

This vibration extension allows for more comprehensive control over device vibrations

1. A function to change both length and intensity of the vibration

## Building and Testing the Sample

1. Deploy the extension into your BB10 WebWorks SDK by copying the community.vibration folder from the ext folder to the Framework/ext folder in your WebWorks SDK, or run the Ant build script in the /ext folder to do so.
2. Package the sample in the sample\_application folder. Use the [Ant Build Script](https://github.com/blackberry/BB10-WebWorks-Community-Samples/tree/master/Ant-Build-Script), or you can do this with the WebWorks packager command:
	__bbwp__ sample\_application -g _signingpassword_
3. Load it on your phone using the Ant Build Script, or with the _blackberry-deploy_ command found in the dependencies/tools folder of your WebWorks SDK.
4. When the application runs, a series of divs on the screen will appear with an orange background, and the extension methods will be called. When sucessful results or retrieved, the div backgrounds will change to blue as the screenshot below.

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
