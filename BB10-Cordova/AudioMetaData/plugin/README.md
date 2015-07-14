#AudioMetaData Cordova/Webworks Plugin

This plugin is designed to be able to retrive the ID3 tag of a MP3 file. This tag contains information for the file such as title, genre, artist, year of production, track number on the album etc. For more info visit <a href="http://en.wikipedia.org/wiki/ID3">here</a> for more information.

## Version History

	1.0.0 Initial Release
	1.1.0 Ability to set Metadata tags

##Loading the plugin

Simply open up a terminal and perform the following commands while in your application path:

````
cordova plugin add cordova-plugin-audiometadata
```

##Calling The Extraction Functions

````
audioMetaDataGetMetaData(path, callBack);
````
where:
````
path: The path to your mp3 file
callBack: The callback function to handle the data returned. eg: retrievedData(data)
````

````
audioMetaDataSetTagData(input);
````
where:
````
input: a json object containing the following information
  "path": path to the file
  "title": changing title
  "genre": changing genre
  "track": changing track number
  "year": changing year of the production of the song
  "album": changing album of the song
  "artist": changing the artist of the song

Follow is an example to change the songs title and artist

var data = { "path":"path/to/my/file.mp3", "title":"very nice", "artist":"yours truly" }
````

````
audioMetaDataRemoveTag(path);
````
where:
````
path: The path to your mp3 file
````

###Tested on

	Z10 running v10.2
	Passport running 10.3.1/10.3.2

Note: Some error occurring in reading some mp3 files, this could be the problem with the library it self. This needs more investigation.


##Final Notes

The same code used for the test application falls under the <a href="http://www.apache.org/licenses/LICENSE-2.0.html">Apache 2.0 License</a>.

Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
