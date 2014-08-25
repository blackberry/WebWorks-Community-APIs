Facebook Plugin
================

The facebook plugin for Webworks/Cordova (BB10) is completely written in JavaScript and does not interact with any native code as there's no offical SDK for Facebook for Blackberry. This plugin lets developers include the basic Facebook functionality in their webworks apps.

#Usage

## Getting Started

Create a Facebook app here: https://developers.facebook.com/apps. In the advanced settings, make sure you declare a "Valid OAuth redirect URI".Add https://www.facebook.com/connect/login_success.html as a Valid OAuth redirect URI for access from Webworks.

Copy the Facbeook App ID. You'll need this to instantiate the plugin.

##Available API functions
This plugin provides the developers with the following 5 functions. The plugin, as mentioned before, requires a Facebook App to be already setup along with the correct permissions. 

###Initialize App
To initialize the facebook plugin use the followig code.
```
community.facebookplugin.init({appID: APPIDFROMFACEBOOK})
```
###Get Facebook Login Status
```
community.facebookplugin.getLoginStatus(callBack);
```
###Login using Faceboo
```
community.facebookplugin.login(callback, {scope: email})
```

###Make a call to the Graph API
Example : getting the user's friend list
```
community.facebookplugin.api({path: '/me/friends', success: successHandler, error: errorHandler});
```

###Logout from Facebook
```
community.facebookplugin.logout(callBack)
```


##DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
