webworks-bb10-screenshot
========================

__Note: This plugin has been incorporated into the [core SDK](https://developer.blackberry.com/html5/apis/v2_2/blackberry.screenshot.html) now.__

A screenshot extension for BlackBerry 10 Webworks. Features:

* save to file, or return a data-URL
* control rectangle to capture
* control JPEG compression quality
* capture as JPEG, PNG
* set file permissions

Tested on:
* OS 10.2.1.1927
* WebWorks 2.0.0.54
* Dev Alpha C

Authors:
* [Jon Webb](http://www.jonwebb.net)          
* [Morgan Parlee](https://github.com/mkparlee) 

Support forums thread: http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/WebWorks-BB10-screenshot-extension/td-p/2631541

How To Install The Plugin
-------------------------

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.screenshot

or,
	
	webworks plugin add com.blackberry.community.screenshot

Installation from source is the same but instead of the id ("com.blackberry.community.screenshot"), use the file system path to the source plugin folder.


Taking a screenshot
-------------------

To take a screenshot and put it in an `<img>` tag using JQuery:

HTML:
```html
	<img id="myimage" />
```
JS:
```javascript
	// configure options
	var options = {dest:'data:', mime:'image/png'};
	// perform screenshot
	var screenshot = community.screenshot.execute(options);
	// check result
	if(screenshot.substr(0,5)=="data:")
		$('#myimage').attr('src', screenshot)
	else
		alert(screenshot);
```	

This configures the screenshot to return a data-URL containing a PNG image. Then it performs the screenshot and checks the result. If the result starts with a `data:` prefix, it is loaded into the image tag. Otherwise, it is an error, which we display.

See the sample application for more details.

Options
-------

The `execute` function takes an `options` object which allows you to customize how it works. The options object may contain:

<dl>
	<dt>dest: string</dt>
	<dd>either "data:", which returns the image as a data-URL (default)
	or a complete file path with extension, which saves the image to the file.</dd>
 
	<dt>rect: {x: int, y:int, w: int, h: int}</dt>
	<dd>the rectangle to capture (optional, defaults to full screen).</dd>
 
	<dt>quality: int</dt>
	<dd>jpeg/png quality factor (1-100, default 90).</dd>
 
	<dt>mime: string</dt>
	<dd>for a data-URL destination: defines the mime type to encode (defaults to "image/jpeg").</dd>

	<dt>chmod: int</dt>
	<dd>for a file path destination: which chmod should be performed.</dd>
</dl>

Result
------

The `execute()` function returns a string:

* the data-URL (including `data:` prefix)
* or `error:` followed by an error message
* or the full path of the destination file.

Notes
-----
* The `chmod` option may be needed if you wish to pass the screenshot to other parts of the system, such as a Window Cover. It should be specified as an octal value, e.g. `parseInt(644,8)`.
* Refer to the [Template Extension](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10/Template) for info on how to build extensions from source
* Toggle the `TRACE` flag in `screenshot_js.hpp` to enable or disable tracing. The trace output can be viewed in the Web Inspector console of the Bootstrap webkit instance.

Disclaimer
----------
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.