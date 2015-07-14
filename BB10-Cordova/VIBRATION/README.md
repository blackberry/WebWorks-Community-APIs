Extension-Vibration
==================

This vibration extension allows for more comprehensive control over device vibrations

1. A function to change both length and intensity of the vibration

## How To Install The Plugin

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-vibrate-intense

Installation from source is the same but instead of the id ("cordova-plugin-vibrate-intense"), use the file system path to the source plugin folder.

### Using the Extension in an Application</a>

```javascript
var data = { "duration" : [int: duration], "intensity" : [int: intensity]}
community.vibration.vibration_request(data, [function: callback])
```
Parameters:  
duration : length of vibration in ms ( 0 - 5000)
intensity : level of vibration ( 1 - 100 )

Example:

	<script type="text/javascript" >
		var data = { "duration" : 1000 , "intensity" : 100 };
		community.vibration.vibration_request(data, null);
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
