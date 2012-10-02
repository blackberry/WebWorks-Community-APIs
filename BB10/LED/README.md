# LED API

The LED API allows you to control the LED.

**Tested On**

* BlackBerry 10.0.9.388

**Author(s)** 

* [Andy Wu](https://github.com/andywu89)            

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Extension

1. Clone the repo to your local machine
2. Locate your BlackBerry WebWorks SDK **C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK <version>\Framework\ext** (Windows) or **~/SDKs/BlackBerry 10 WebWorks SDK <version>/Framework/ext** (Mac)
3. Create a new directory **webworks.bbalert.led** in the **ext** directory.
4. Copy **output/webworks.bbalert.led/ledJnext.so** to **/path/to/sdk/ext/webworks.bbalert.led/device/ledJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/webworks.bbalert.led/**

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.bbalert.led" required="true" version="1.0.0.0" />

## Summary

var id = webworks.bbalert.led.startLed([String: color], [String: blinkCount])

Parameters:

color: hex string with with format "RRGGBB", default: "FF0000"
blinkCount: String Integer >=0, default: "0" (0 means Blink indefinitely until stopLed command is issued)

Return:

request id - used to reference the LED request

webworks.bbalert.led.stopLed([String:id])

Parameter:

id: request id - given when startLed command was issued.
	
Example:

	<script type="text/javascript" >
		var id = webworks.bbalert.led.startLed("FFFF00", "10");
		webworks.bbalert.led.stopLed(id)
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.