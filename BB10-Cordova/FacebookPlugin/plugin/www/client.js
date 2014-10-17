/**
* Copyright (c) 2014 BlackBerry Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @author: Justin Carvalho
*         jcarvalho.ca, github.com/jstncarvalho
*
* Notice: This code is based off of Christophe Coenraets' OpenFB library
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* OpenFB License:
* Copyright (c) 2014 CHRISTOPHE COENRAETS
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

var _self = {},
	_ID = "com.blackberry.community.facebookplugin",

    FB_LOGIN_URL = 'https://www.facebook.com/dialog/oauth',
    FB_LOGOUT_URL = 'https://www.facebook.com/logout.php',

    // By default we store access_token in sessionStorage. This can be overridden in init()
    tokenStore = window.sessionStorage,

    fbAppId,

    context = window.location.pathname.substring(0, window.location.pathname.indexOf("/",2)),

    baseURL = location.protocol + '//' + location.hostname + (location.port ? ':' + location.port : '') + context,

    oauthRedirectURL = baseURL + '/oauthcallback.html',

    logoutRedirectURL = baseURL + '/logoutcallback.html',

    // Because the OAuth login spans multiple processes, we need to keep the login callback function as a variable
    // inside the module instead of keeping it local within the login function.
    loginCallback,

    // Indicates if the app is running inside Cordova
    runningInCordova,

    // Used in the exit event handler to identify if the login has already been processed elsewhere (in the oauthCallback function)
    loginProcessed;


document.addEventListener("deviceready", function () {
    runningInCordova = true;
}, false);

/**
 * Initialize the OpenFB module. You must use this function and initialize the module with an appId before you can
 * use any other function.
 * @param params - init paramters
 *  appId: The id of the Facebook app,
 *  tokenStore: The store used to save the Facebook token. Optional. If not provided, we use sessionStorage.
 */
_self.init = function(params) {
    if (params.appId) {
        fbAppId = params.appId;
    } else {
        throw 'appId parameter not set in init()';
    }

    if (params.tokenStore) {
        tokenStore = params.tokenStore;
    }
};

/**
 * Checks if the user has logged in with openFB and currently has a session api token.
 * @param callback the function that receives the loginstatus
 */
_self.getLoginStatus = function(callback) {
    var token = tokenStore['access_token'],
        loginStatus = {};
    if (token) {
        loginStatus.status = 'connected';
        loginStatus.authResponse = {token: token};
    } else {
        loginStatus.status = 'unknown';
    }
    if (callback) callback(loginStatus);
};

/**
 * Login to Facebook using OAuth.
 *
 * @param callback - Callback function to invoke when the login process succeeds
 * @param options - options.scope: The set of Facebook permissions requested
 * @returns {*}
 */
_self.login = function(callback, options) {

    var loginWindow,
        startTime,
        scope = '';

    if (!fbAppId) {
        return callback({status: 'unknown', error: 'Facebook App Id not set.'});
    }

    if (options && options.scope) {
        scope = options.scope;
    }

    loginCallback = callback;
    loginProcessed = false;

    oauthRedirectURL = "https://www.facebook.com/connect/login_success.html";

    startTime = new Date().getTime();
    redirectUri = FB_LOGIN_URL + '?client_id=' + fbAppId + '&redirect_uri=' + oauthRedirectURL +
        '&response_type=token&scope=' + scope;
    loginWindow = window.open(FB_LOGIN_URL + '?client_id=' + fbAppId + '&redirect_uri=' + oauthRedirectURL +
        '&response_type=token&scope=' + scope, '_blank');
    console.log(loginWindow);

    var inter = setInterval(function() {
        var currentURL = loginWindow.window.location.href;
        var callbackURL = redirectUri;
        var inCallback = currentURL.indexOf("access_token=");

        // location has changed to our callback url
        if (inCallback != -1) {

            // stop the interval
            clearInterval(inter);

            // parse the access token
            var code = currentURL;
            code = code.split('access_token=');
            code = code[1];
            code = code.split('&expires_in=');
            code = code[0];
            tokenStore.access_token = code;

            // close the loginWindow
            loginWindow.window.close();
        }
    }, 1000);

};

/**
 * Logout from Facebook, and remove the token.
 * IMPORTANT: For the Facebook logout to work, the logoutRedirectURL must be on the domain specified in "Site URL" in your Facebook App Settings
 *
 */
_self.logout = function(callback) {
    var logoutWindow,
        token = tokenStore['access_token'];

    if (token) {
        tokenStore.removeItem('access_token');
        logoutWindow = window.open(FB_LOGOUT_URL + '?access_token=' + token + '&next=' + logoutRedirectURL, '_blank', 'location=no');
        if (runningInCordova) {
            setTimeout(function() {
                logoutWindow.close();
            }, 1000);
        }
    }

    if (callback) {
        callback();
    }

};

/**
 * Lets you make any Facebook Graph API request.
 * @param obj - Request configuration object. Can include:
 *  method:  HTTP method: GET, POST, etc. Optional - Default is 'GET'
 *  path:    path in the Facebook graph: /me, /me.friends, etc. - Required
 *  params:  queryString parameters as a map - Optional
 *  success: callback function when operation succeeds - Optional
 *  error:   callback function when operation fails - Optional
 */
_self.api = function (obj) {

    var method = obj.method || 'GET',
        params = obj.params || {},
        xhr = new XMLHttpRequest(),
        url;

    params['access_token'] = tokenStore.access_token;

    url = 'https://graph.facebook.com' + obj.path + '?' + toQueryString(params);

    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4) {
            if (xhr.status === 200) {
                if (obj.success) obj.success(JSON.parse(xhr.responseText));
            } else {
                var error = xhr.responseText ? JSON.parse(xhr.responseText).error : {message: 'An error has occurred'};
                if (obj.error) {
                    obj.error(error);
                    console.log(obj);
                }
            }
        }
    };

    xhr.open(method, url, true);
    xhr.send();
};

/**
 * De-authorizes the app
 * @param success
 * @param error
 * @returns {*}
 */
_self.revokePermissions = function (success, error) {
    return _self.api({method: 'DELETE',
        path: '/me/permissions',
        success: function () {
            tokenStore['access_token'] = undefined;
            success();
        },
        error: error});
};

function parseQueryString(queryString) {
    var qs = decodeURIComponent(queryString),
        obj = {},
        params = qs.split('&');
    params.forEach(function (param) {
        var splitter = param.split('=');
        obj[splitter[0]] = splitter[1];
    });
    return obj;
}

function toQueryString(obj) {
    var parts = [];
    for (var i in obj) {
        if (obj.hasOwnProperty(i)) {
            parts.push(encodeURIComponent(i) + "=" + encodeURIComponent(obj[i]));
        }
    }
    return parts.join("&");
}

module.exports = _self;