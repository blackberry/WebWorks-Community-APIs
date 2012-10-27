# ThumbNail API

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
<<<<<<< HEAD
4. Copy **output/community.thumbnail/device/thumbnailJnext.so** to **/path/to/sdk/ext/community.thumbnail/device/thumbnailJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.thumbnail/**
=======
4. Copy **output/community.led/ledJnext.so** to **/path/to/sdk/ext/community.led/device/ledJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.led/**
>>>>>>> upstream/master

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

<<<<<<< HEAD
    <feature id="community.thumbnail" required="true" version="1.0.0.0" />

## Summary

var image = community.thumbnail.createThumbNail(path, {width: width, height: height, quality:quality});

Parameters:  
path: path to image on local file system 
JSON: width - width of resize image - optional  
	  height - height of resize image - optional
	  quality - quality of the resized image (default: 100) (0-100 ?? c++ api reference did not specify the quality range) - optional  
Note: must contain either width or height or both as parameter. Providing only width or height will scale to proportion.   

Return:  
Image DOM object  

Example:

	<script type="text/javascript" >
         var image = community.thumbnail.createThumbNail(path, {width:480});
		 document.getElementById("image").innerHTML = "";
		 document.getElementById("image").appendChild(image);
=======
    <feature id="community.led" required="true" version="1.0.0.0" />

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
>>>>>>> upstream/master
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.