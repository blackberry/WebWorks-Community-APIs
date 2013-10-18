webworks-bb10-screenshot
========================

A screenshot extension for BlackBerry 10 Webworks. Features:

* save to file, or return a data-URL
* control rectangle to capture
* control JPEG compression quality
* capture as JPEG, PNG
* set file permissions

Tested on:
* OS 10.0.10.738
* OS 10.2.0.1767
* WebWorks 1.0.4.11
* Z10, Dev Alpha C

Author: [Jon Webb] (http://www.jonwebb.net)
Support forums thread: TBA

Installing the extension
------------------------

Install the extension into your BB10 WebWorks SDK by copying the `net.jonwebb.screenshot` folder from the `ext` folder to the `Framework/ext` folder in your WebWorks SDK (On Windows normally `C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK (version#)\Framework\Ext`).

Adding the extension to your application
----------------------------------------

Edit the `config.xml` in your project and add the following feature tag:

```xml
  <feature id="net.jonwebb.screenshot" />
```

Taking a screenshot
-------------------

To take a screenshot and put it in an `<img>` tag using JQuery:

```html
	<img id="myimage" />
```
```javascript
	var options = {dest:'data:', mime:'image/png'};
	var screenshot = net.jonwebb.screenshot.execute(options);
	if(screenshot.substr(0,5)=="data:")
		$('#myimage').attr('src', screenshot)
	else
		alert(screenshot);
```	

This configures the screenshot to return a data-URL containing a PNG image. Then it performs the screenshot and checks the result. If the result starts with a `data:` prefix, it is loaded into the image tag. Otherwise, it is an error, which we display.

Options
-------

The `execute` function takes an `options` object which allows you to customize how it works. The options object may contain:

<dl>
	<dt>dest: string</dt>
	<dd>either `data:`, which returns the image as a data-URL (default)
	**or** a complete file path with extension, which saves the image to the file.</dd>
 
	<dt>rect: {x: int, y:int, w: int, h: int}</dt>
	<dd>the rectangle to capture (optional, defaults to full screen).</dd>
 
	<dt>quality: int</dt>
	<dd>jpeg/png quality factor (1-100, default 90).</dd>
 
	<dt>mime: string</dt>
	<dd>for a **data-URL** destination: defines the mime type to encode (defaults to `image/jpeg`).</dd>

	<dt>chmod: int</dt>
	<dd>for a **file path** destination: which `chmod` should be performed.</dd>
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
