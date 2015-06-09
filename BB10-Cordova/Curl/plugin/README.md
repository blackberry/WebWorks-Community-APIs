Curl Extension
=============

This BlackBerry 10 Cordova Plugin providea simple HTTP-GET function using libcurl.

** Tested On **

BlackBerry Dev Alpha C / 10.2.0.429

** Author **

[Merrick Zhang](https://github.com/anphorea)

[About Me](http://about.me/anpho)

## Version History

	1.0.0 First release
	1.0.1 Asynchronous operation

## Including the feature in your application

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-curl

Installation from source is the same but instead of the id ("cordova-plugin-curl"), use the file system path to the source plugin folder.

## The Plugin API
The Curl plugin provides the following API:

```javascript
var str="http://google.com";
var result = community.curl.get(str);
//result shows the content of the URL,timeout is 3 seconds.
```

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.