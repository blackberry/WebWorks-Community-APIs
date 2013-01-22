# Vibration API

The Vibration API allows you to control the Vibration.

**Tested On**

* BlackBerry 10.9.9.339

**Author(s)** 

* [Alex Kwan](https://github.com/greentea0)            

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Extension

1. Clone the repo to your local machine
2. Locate your BlackBerry WebWorks SDK **C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK <version>\Framework\ext** (Windows) or **~/SDKs/BlackBerry 10 WebWorks SDK <version>/Framework/ext** (Mac)
3. Create a new directory **community.vibration** in the **ext** directory.
4. Copy **output/community.vibration/libVibration.so** to **/path/to/sdk/ext/community.vibration/device/libVibration.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.vibration/**

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="community.vibration" required="true" version="1.0.0.0" />

## Summary
var data = { "duration" : [int: duration], "intensity" : [int: intensity]}
var id = community.vibration.requestVibration(data, [function: callback])

Parameters:  
duration : length of vibration ( 0 - 5000)
intensity : level of vibration ( 1 , 10 , 100 )

Return:  
request id - used to reference the Vibration request

community.vibration.requestVibration([String:id])  
Parameter:  
id: request id - given when requestVibration command was issued.
	
Example:

	<script type="text/javascript" >
		var data = { "duration" : 1000 , "intensity" : 100 };
		var id = community.vibration.requestVibration(data, null);
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.