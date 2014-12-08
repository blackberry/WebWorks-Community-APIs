LED
=============

This BlackBerry 10 Cordova Plugin flashes the RGB LED on a Blackberry phone. It can flash it using startLed for any integer number of times >= 0, where setting the value to 0 means that the LED flashes indefinitely. It can be flashed any possible hex color. It can be turned off with a stopLed function. 

** Tested On **

BlackBerry Q5 10.2.1.1927
BlackBerry Passport 10.3.1.1055

** Author **

[Andy Wu](https://github.com/andywu89)
[Deric Cheng](https://github.com/dericc)

## Version History

	1.0.0 Initial Release
	1.0.1 Includes LED permission in plugin.xml

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.led

or,
	
	webworks plugin add com.blackberry.community.led

Installation from source is the same but instead of the id ("com.blackberry.community.led"), use the file system path to the source plugin folder.


## The Plugin API
The Led plugin provides the following API:

```javascript
var id = community.led.startLed(color, blinkCount); 
```
This function calls the LED on a Blackberry phone to blink "blinkCount" number of times with color "color". 
The variable "color" is defined as a 6-character hex string, for example "FF0000" is the equivalent hex string for a red light.
The variable "blinkCount" is an integer defined as the number of times the LED will blink before turning off. Setting blinkCount to 0 indicates that the light will blink until stopLed is called. 
The function startLed() returns the id of the LED process. 

```javascript
community.led.stopLed(id); 
```
This function stops the LED from blinking if it is currently blinking. 
The variable id is the ID of the LED process. 

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
