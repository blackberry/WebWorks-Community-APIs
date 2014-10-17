Facebook Plugin
================

The facebook plugin for Webworks/Cordova (BB10) is completely written in JavaScript and does not interact with any native code as there's no offical SDK for Facebook for Blackberry. This plugin lets developers include the basic Facebook functionality in their webworks apps.

#Usage

## Getting Started

Create a Facebook app here: https://developers.facebook.com/apps.
-In the advanced settings, make sure you declare a "Valid OAuth redirect URI".Add https://www.facebook.com/connect/login_success.html as a Valid OAuth redirect URI for access from Webworks.

NOTE: All callbacks/handlers are optional

##Available API functions
This plugin provides the developers with the following 6 functions. The plugin requires a Facebook App to be already setup along with the correct permissions.

###Initialize App
Initialize the OpenFB module. You must use this function and initialize the module with an appId before you can use any other function.
```javascript
community.facebookplugin.init({appID: APPIDFROMFACEBOOK})
```
###Get Facebook Login Status
Checks if the user has logged in with openFB and currently has a session api token.
```javascript
community.facebookplugin.getLoginStatus(callback);
```
###Login to Facebook
Login to Facebook using OAuth, and store the access token
```javascript
community.facebookplugin.login(callback, {scope: email})
```

###Make a call to the Graph API
Lets you make any Facebook Graph API request.
params:
* obj:  Request configuration object. Can include:
* method:  HTTP method: GET, POST, etc. Optional - Default is 'GET'
* path:    path in the Facebook graph: /me, /me.friends, etc. - Required
* params:  queryString parameters as a map - Optional
* success: callback function when operation succeeds - Optional
* error:   callback function when operation fails - Optional
Example : getting the user's friend list
```javascript
community.facebookplugin.api({path: '/me/friends', success: successHandler, error: errorHandler});
```

###Logout from Facebook
Logout from Facebook, and remove the access token.
```javascript
community.facebookplugin.logout(callback)
```

###Revoke user granted permissions
De-authorizes the app
```javascript
community.facebookplugin.revokePermissions(successHandler, errorHandler);
```


#DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This plugin is based off of Christophe Coenraets OpenFB library, which is licensed under MIT. You can find it [https://github.com/ccoenraets/OpenFB](here).
