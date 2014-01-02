# ThumbNail API

The ThumNail API allows you to create thumbnail of images residing on the local filesystem.

**Tested On**

* BlackBerry 10.0.9.388

**Author(s)** 

* [Andy Wu](https://github.com/andywu89)            

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Extension

1. Clone the repo to your local machine
2. Locate your BlackBerry WebWorks SDK **C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK <version>\Framework\ext** (Windows) or **~/SDKs/BlackBerry 10 WebWorks SDK <version>/Framework/ext** (Mac)
3. Create a new directory **community.thumbnail** in the **ext** directory.
4. Copy **output/community.thumbnail/device/thumbnailJnext.so** to **/path/to/sdk/ext/community.thumbnail/device/thumbnailJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.thumbnail/**


## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

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
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/Thumbnail?pixel)](https://github.com/igrigorik/ga-beacon)