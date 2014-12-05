Facebook Plugin
================

Cordova plugin that handles Facebook integration for BB10 Apps. The plugin makes use of the OpenFB Library (which leverages Facebook OAuth), and is written completely in javascript.

# Prerequisites
1. Facebook Account
2. Facebook App ID (from the developer portal)

##### Configuring your Facebook App for use with the plugin:
Register your app with Facebook in the developer portal: https://developers.facebook.com/apps.
* In the *Advanced Settings*, add https://www.facebook.com/connect/login_success.html as a Valid OAuth redirect URI.
* Under *Status & Review*, you can request more app permissions. By default, you will be able to retrieve the primary email address, public profile, and a list of friends that are also using your app.

#Usage
This plugin provides the developers with the following 6 functions. The plugin requires a Facebook App to be already registered with the Facebook Developer portal along with the neccessary permissions granted.



##Initialize Plugin
Initialize the plugin. You must use this function and initialize the plugin with an appId before you can use any other function.
```javascript
community.facebookplugin.init({appId: APPIDFROMFACEBOOK})
```
You may also specify a custom storage location the plugin will use for the access token.
```javascript
community.facebookplugin.init({appId: APPIDFROMFACEBOOK, tokenStore: LOCATION})
```
Failing to include appId, will cause an error to be thrown. All other functions assume the plugin to have already been initalized.


##Get Facebook Login Status
Checks if the user has logged in with openFB and currently has a session api token.
```javascript
community.facebookplugin.getLoginStatus(callback);
```
The function will call callback with an object: loginStatus. In the case where an access token is found, `loginStatus.status` will be `'connected'`. In all other cases, the value will be `'unknown'`.


##Login to Facebook
Login to Facebook using OAuth, and store the access token. Will overwrite a token if it already exists.
```javascript
community.facebookplugin.login(callback, {scope: email})
```
User Perspective: When this function runs, a browser window is opened with the facebook login page. Upon successful login, they are asked to grant permissions to the app (if they haven't yet). The window returns an access token from Facebook, and closes.

##Make a call to the Graph API
Make a Facebook Graph API request.
params:
* obj:  Request configuration object. Can include:
* method:  HTTP method: GET, POST, etc. Optional - Default is 'GET'
* path:    path in the Facebook graph: /me, /me.friends, etc. - Required
* params:  queryString parameters as a map - Optional
* success: callback function when operation succeeds - Optional
* error:   callback function when operation fails - Optional

Example : getting a list of friends (relative to the access token) that use the app
```javascript
community.facebookplugin.api({path: '/me/friends', success: successHandler, error: errorHandler});
```
For all possible options, visit the Facebook Docs [here](https://developers.facebook.com/docs/graph-api/reference/v2.1/). The plugin is compatabible with their JavaScript SDK.

##Logout from Facebook
Logout from Facebook, and remove the access token.
```javascript
community.facebookplugin.logout(callback)
```
User Perspective: When this function runs, a browser window is with the facebook logout page. It automatically closes.

##Revoke user granted permissions
De-authorizes the app
```javascript
community.facebookplugin.revokePermissions(successHandler, errorHandler);
```
Helper function that makes a delete request to the Facebook Graph API. Makes use of self.api. Revoking permissions automatically logs a user out.


#DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This plugin is based off of Christophe Coenraets OpenFB library, which is licensed under MIT. You can find it [https://github.com/ccoenraets/OpenFB](here).
