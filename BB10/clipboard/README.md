Clipboard Extension for BlackBerry 10 WebWorks
================================================

This BlackBerry 10 WebWorks extension allows you to programmatically set and retrieve the device's clipboard.

## How to use

1. copy the `community.clipboard` folder under`ext` folder to your webworks installation directory's `/framework/ext` folder, so that it looks like this:

	`C:\Program Files (x86)\Research In Motion\BlackBerry 10 WebWorks SDK 1.0.4.11\Framework\ext\community.clipboard`
	
	`+  client.js`
	
	`+  index.js`
	
	`+  manifest.json`
	
	`+-device/libclipboard.so`
	
	`\-simulator/libclipboard.so`


2. in your `config.xml` file, add the following line feature:


	`<feature id="community.clipboard" required="true" version="1.0.0.0"/>`


3. in your app, use the following code to call the extension:

	`community.clipboard.getText();`
	
	`community.clipboard.setText("String");`

** Author **

[Merrick Zhang](https://github.com/anphorea)

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/Clipboard?pixel)](https://github.com/igrigorik/ga-beacon)