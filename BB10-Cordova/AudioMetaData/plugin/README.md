#AudioMetaData Cordova/Webworks Plugin

This plugin is designed to be able to retrive the ID3 tag of a MP3 file. This tag contains information for the file such as title, genre, artist, year of production, track number on the album etc. For more info visit <a href="http://en.wikipedia.org/wiki/ID3">here</a> for more information.

## Version History

	1.0.0 Initial Release

##Loading the plugin

Simply open up a terminal and perform the following commands while in your application path:

````
webworks plugin add com.blackberry.community.audiometadata
```

##Calling The Extraction Functions

Currently the only function to call in the plugin is

````
community.audiometadata.audioMetaDataGetMetaData(path, callBack);
````
where:
````
path: The path to your mp3 file
callBack: The callback function to handle the data returned. eg: retrievedData(data)
````

###Tested on

Z10 running v10.2
Passport running 10.3.1

Note: Some error occurring in reading some mp3 files, this could be the problem with the library it self. This needs more investigation.


##Final Notes

The same code used for the test application falls under the <a href="http://www.apache.org/licenses/LICENSE-2.0.html">Apache 2.0 License</a>.

Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
