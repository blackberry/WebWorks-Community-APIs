# Thumbnail API

The Thumbnail API allows you to create thumbnail of images residing on the local filesystem.

**Tested On**

* BlackBerry 10.2.1.1927

**Author(s)** 

* [Andy Wu](https://github.com/andywu89)            
* [Morgan Parlee](https://github.com/mkparlee) 

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Plugin

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.thumbnail

or,
	
	webworks plugin add com.blackberry.community.thumbnail

Installation from source is the same but instead of the id ("com.blackberry.community.thumbnail"), use the file system path to the source plugin folder.
	
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
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/Thumbnail?pixel)](https://github.com/igrigorik/ga-beacon)