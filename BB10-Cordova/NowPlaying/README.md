# NowPlaying #

This BlackBerry 10 Cordova Plugin plays music in the background.

## Contents ##

* [Native Extension](plugin/src/blackberry10/native) - BlackBerry 10 Native SDK Project
* [Sample Application](sample) - WebWorks 2.0/Cordova Project

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.nowplaying

or,
	
	webworks plugin add com.blackberry.community.nowplaying

Installation from source is the same but instead of the id ("com.blackberry.community.nowplaying"), use the file system path to the source plugin folder.

## Javascript API ##

The api exports a global module as blackberry.community.nowplaying

### Object Methods ###

#### NowPlayingPlay() ####
Play music in the background. Make sure to call NowPlayingSwitchMusic(url-for-music) to set up the source. Otherwise, a default music will be played.

Example:
	com.blackberry.community.nowplaying.NowPlaying();

#### NowPlayingStop() ####
Stop the music if there is any music playing in the background. Has no effect otherwise.

Example:
	com.blackberry.community.nowplaying.NowPlayingStop();

#### NowPlayingPause() ####
Pause the music if there is any music playing in the background, and call NowPlayingPlay() to continue. Has no effect otherwise.

Example:
	com.blackberry.community.nowplaying.NowPlayingPause();	

#### NowPlayingGetSourceAddress() ####
Get the url for the source of music. If there's no music, return the address of the default music.

Example:
	com.blackberry.community.nowplaying.NowPlayingGetSourceAddress();

#### NowPlayingGetDuration() #### 
Get the duration of the current music. On default return the default music's duration.

Example:
	com.blackberry.community.nowplaying.NowPlayingGetDuration();

#### NowPlayingGetPosition() ####
Get the current position of the music player. If the music has not played already, return 0.

Example:
	com.blackberry.community.nowplaying.NowPlayingGetPosition();

#### NowPlayingSwitchMusic(url) ####
Stop the current music and set the music to the given url.

Example:
	com.blackberry.community.nowplaying.NowPlayingSwitchMusic('http://www.w3schools.com/html/horse.ogg');

## How to build your native Plugin

1.  Clone this repo to your local machine

2.  Ensure the [BlackBerry 10 Native SDK](https://developer.blackberry.com/native/download/sdk) is correctly installed

3.  Import the native extension project into momentics. It is located at `/plugin/src/blackberry10/native` within **this project**

4.  Build the native extension project for all architectures: Right click on the project and choose "Build Configurations" > "Build All"



## Using the Plugin in an Application
The following commands are from nowplaying_build.sh

rm -rf debug1
webworks create debug1
cd debug1
webworks plugin add ../plugin
rm -rf www/
cp -r ../sample/* ./
webworks run

Or, ./nowplaying_build.sh if using linux.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

