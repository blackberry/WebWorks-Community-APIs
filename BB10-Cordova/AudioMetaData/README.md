#AudioMetaData Cordova/Webworks Plugin

This plugin is designed to be able to retrive the ID3 tag of a MP3 file. This tag contains information for the file such as title, genre, artist, year of production, track number on the album etc. For more info visit <a href="http://en.wikipedia.org/wiki/ID3">here</a> for more information.

##Setting Up

####Building the Plugin
```
Open Momentics and import the plugin/src/blackberry10/native directory.
Build the project.
```

The .cproject file should be automatically picked up by Momentics with regards to linking the id3v2lib static library. However, if not, then follow the steps below to link it manually.

```
Right click on the imported project and choose properties.
Drop down C/C++ Build and choose Settings
Navigate to Tool Settings tab and choose Libraries under QCC Linker
 1. press add in Library Path(the file with gree + icon) and navigate to plugin/src/blackberry10/native/extlib
 2. press add in Libraries(Same icon as above) and type "id3v2" without the quotes.
```

####Loading the plugin

Simply open up a terminal and perform the following commands while in your application path:

````
cordova plugin add path/to/AudioMetaData plugin/plugin
```

#####However the following permissions must be applied to config.xml of you application

```
<rim:permissions>
 <rim:permit>access_shared</rim:permit>
</rim:permissions>
```

####Running the TestApplication

Connect your BlackBerry 10 to your computer and with Development Mode enabled, then input the following commands.

````
cd path/to/your/application
cordova platform add blackberry10
cordova plugin add path/to/AudioMetaData plugin/plugin
cordova plugin add cordova-plugin-bb-card
cordova run
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
