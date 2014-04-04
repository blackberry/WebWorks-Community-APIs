# Unzip API

The Unzip API allows you to extract a Zip package to a folder of your choice on your device.

**Tested On**

* BlackBerry 10.0.9.388

**Author(s)**

* [Arman Fallah](https://github.com/starpax)

**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## How To Install The Extension

1. Clone the repo to your local machine
2. Locate your BlackBerry WebWorks SDK **C:\Program Files\Research In Motion\BlackBerry 10 WebWorks SDK <version>\Framework\ext** (Windows) or **~/SDKs/BlackBerry 10 WebWorks SDK <version>/Framework/ext** (Mac)
3. Create a new directory **community.compass** in the **ext** directory
4. Copy **output/community.unzip/unzipJnext.so** to **/path/to/sdk/ext/community.unzip/device/unzipJnext.so**
5. Copy everything in **javascript_src** to **/path/to/sdk/ext/community.unzip/**

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

	<feature id="community.unzip" required="true" version="1.0.0.0" />
	
## Summary

**community.compass.unzipPackage([filepath], [unzipToPath])**

Parameter:
filepath: path of the .zip package to extract from
unzipToPath: directory to extract the package contents to

Return:
[String] - Zip package directory structure (use eval on this returned string to turn it into a JSON object)

Example:

	<script type="text/javascript">
		var txt = community.unzip.unzipPackage("shared/documents/bbtest.zip", "shared/documents/");
		var JSONobject = eval(txt);
	</script>

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/Unzip?pixel)](https://github.com/igrigorik/ga-beacon)