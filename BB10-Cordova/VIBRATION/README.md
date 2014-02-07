Extension-Vibration
==================

This vibration extension allows for more comprehensive control over device vibrations

1. A function to change both length and intensity of the vibration

## Including the feature in your application

This API can be installed from the source by copying the plugin folder to your webworks project, then add the plugin with the following commands:

	cordova plugin add plugin

or,
	
	webworks plugin add plugin

community.vibration folder should appear under the plugins folder of your project.

### Using the Extension in an Application</a>

```javascript
var data = { "duration" : [int: duration], "intensity" : [int: intensity]}
community.vibration.vibration_request(data, [function: callback])
```
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
