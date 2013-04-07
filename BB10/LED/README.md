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
3. Create a new directory **community.led** in the **ext** directory.
4. Copy **output/community.led/ledJnext.so** to **/path/to/sdk/ext/community.led/device/ledJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.led/**

## Required Feature ID and Permission
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="community.led" required="true" version="1.0.0.0" />

Add the following permission to your config.xml file as well:

	<rim:permissions>
		<rim:permit>access_led_control</rim:permit>
	</rim:permissions>
	
## Summary

var id = community.led.startLed([String: color], [String: blinkCount])

Parameters:  
color: hex string with with format "RRGGBB", default: "FF0000"  
blinkCount: String Integer >=0, default: "0" (0 means Blink indefinitely until stopLed command is issued)

Return:  
request id - used to reference the LED request

community.led.stopLed([String:id])  
Parameter:  
id: request id - given when startLed command was issued.
	
Example:

	<script type="text/javascript" >
		var id = community.led.startLed("FFFF00", "10");
		community.led.stopLed(id)
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.