Clipboard Extension
=============

This BlackBerry 10 Cordova Plugin provide simple clipboard access functions like getText and setText. 

**Tested On**

BlackBerry Dev Alpha C / 10.2.0.429

**Author**

[Merrick Zhang](https://github.com/anphorea)

[About Me](http://about.me/anpho)

## Including the feature in your application

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-clipboard

Installation from source is the same but instead of the id ("cordova-plugin-clipboard"), use the file system path to the source plugin folder.

## The Plugin API
The Clipboard plugin provides the following API:

```javascript
var str="something you want to copy to clipboard";
var result = community.clipboard.setText(str);
//result shows the bytes copied into clipboard, if result=="-1",then something is wrong.

var str = community.clipboard.getText;
// str is the content in the clipboard.
```

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.