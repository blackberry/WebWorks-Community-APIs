# Compass API

The Compass API allows you to read the leading of the compass in your device.

___Note that this extension is superceded by the Sensors API in WebWorks as of SDK Version 1.0.3.8. [Sample available](https://github.com/blackberry/BB10-WebWorks-Samples/tree/master/Sensors). It will remain here as another example of a BlackBerry 10 WebWorks Community Extension.___

**Tested On**

* BlackBerry 10.0.9.388

**Author(s)**

* [Arman Fallah](https://github.com/starpax)

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Extension

1. Clone the repo to your local machine
2. Locate your BlackBerry WebWorks SDK **C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK <version>\Framework\ext** (Windows) or **~/SDKs/BlackBerry 10 WebWorks SDK <version>/Framework/ext** (Mac)
3. Create a new directory **community.compass** in the **ext** directory
4. Copy **output/community.compass/compassJnext.so** to **/path/to/sdk/ext/community.compass/device/compassJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.compass/**

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

	<feature id="community.compass" required="true" version="1.0.0.0" />
	
## Summary

**community.compass.readCompass()**

Return:
number - degrees from Magnetic North

Example:

	<script type="text/javascript">
		var leading = community.compass.readCompass();
	</script>

**community.compass.startMonitoringCompass([Function:fun])**

Parameter:
fun: a one parameter method to call each second, that parameter stores the compass leading      

Return:
[String] - status message of monitoring the compass

Example:

	<script type="text/javascript">                        
	    function compassCallback(compassLeading) 
	    {
	        document.getElementById("leading").innerHTML = "Degrees from Magnetic North: " + compassLeading;
	    }
	    
	    function startMonitoringCompass() 
	    {
	        var ret = community.compass.startMonitoringCompass(compassCallback);
	        alert(ret);
	    }
	</script>   

**community.compass.stopMonitoringCompass()**

Return:
[String] - status message of stopping the monitoring the compass

Example:

	<script type="text/javascript">   
		function stopMonitoringCompass() 
		{
	    	var ret = community.compass.stopMonitoringCompass();
	        alert(ret);
	    }
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.