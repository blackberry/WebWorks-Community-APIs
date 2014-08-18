/*
* Copyright (c) 2013 BlackBerry Limited
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
*/

var _self = {},
	_ID = "com.blackberry.community.facebookplugin",
	exec = cordova.require("cordova/exec");

var facebookLoginURL = 'https://www.facebook.com/dialog/oauth',
    facebookLogoutURL = 'https://www.facebook.com/logout.php',
    tokenStore = window.sessionStorage,
    fbAppId,
    urlContext = window.location.pathname.substring(0, window.location.pathname.indexOf("/",2)),
    
    /* May Have to Change this*/
    baseURL = location.protocol + '//' + location.hostname + (location.port ? ':' + location.port : '') + urlContext,
    oauth = baseURL + '/oauth.html',
    logout = baseURL + '/logout.html',

    //Since the OAuth process spans multiple functions.
    loginCallback,
    // Used in the exit event handler to identify if the login has already been processed elsewhere (in the oauthCallback function)
    loginProcessed;

    //Retrieves value from JSON Object
    function getValue(args, valueName){
        return JSON.parse(decodeURIComponent(args[valueName]));
    }

    //Converts the passed object into a Query String for Facebook    
    function toQueryString(obj) {
        var parts = [];
        for (var i in obj) {
            if (obj.hasOwnProperty(i)) {
                parts.push(encodeURIComponent(i) + "=" + encodeURIComponent(obj[i]));
            }
        }
        return parts.join("&");
    }

    function oauthCallback(url) {
        // Parse the OAuth data received from Facebook
        var queryString,
            obj;

        loginProcessed = true;
        if (url.indexOf("access_token=") > 0) 
        {
            queryString = url.substr(url.indexOf('#') + 1);
            obj = parseQueryString(queryString);
            tokenStore['fbtoken'] = obj['access_token'];
            
            if (loginCallback) 
                loginCallback({status: 'connected', authResponse: {token: obj['access_token']}});
        
        } else if (url.indexOf("error=") > 0) {
            
            queryString = url.substring(url.indexOf('?') + 1, url.indexOf('#'));
            obj = parseQueryString(queryString);
            
            if (loginCallback) 
                loginCallback({status: 'not_authorized', error: obj.error});
        } else {
            
            if (loginCallback) loginCallback({status: 'not_authorized'});
        }
    }

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

    /*
    Initialize the Facebook Plugin with the Applications's AppID
    Can also specify a custom tokenStore location in the jsonData 
    object if desired.
    */
	
    _self.init = function (jsonData) {

		if (getValue(jsonData, "appID")) {
		    fbAppId = getValue(jsonData, "appID");
        } 
	};

     /*
     Function to retrieve the login status, Client can pass a callback
     function if desired. 
     */

	_self.getLoginStatus = function (callback) {

	 var token = tokenStore['fbtoken'],
            loginStatus = {};
        if (token) {
            loginStatus.status = 'connected';
            loginStatus.authResponse = {token: token};

        } else {
            loginStatus.status = 'unknown';
        }

        if (callback) callback(loginStatus);
	};

    
    /*
    The Login Function, pass it the successful login callback and the scope 
    within an object
    */
	_self.login = function (callbackFunction, data) {

	 var loginWindow,
            startTime,
            scope = '';

        if (!fbAppId) {
            return callbackFunction({status: 'unknown', error: 'Facebook App Id not set.'});
        }

        function loginStartHandler(event) {  
            var url = event.url;
            if (url.indexOf("access_token=") > 0 || url.indexOf("error=") > 0) {

                var timeout = 600 - (new Date().getTime() - startTime);
                setTimeout(function () {
                    loginWindow.close();
                }, timeout > 0 ? timeout : 0);
                oauthCallback(url);
            }
        }

        function loginExitHandler() {           
            deferredLogin.reject({error: 'user_cancelled', error_description: 'User cancelled login process', error_reason: "user_cancelled"});
            loginWindow.removeEventListener('loadstop', loginStartHandler);
            loginWindow.removeEventListener('exit', loginExitHandler);
            loginWindow = null;
        }

        if(data && data.scope){
            scope = options.scope;
        }

        // Set the callback function, this is called from within ouathCallback function once login is processed.
        loginCallback = callbackFunction;
        loginProcessed = false;
        
        oauthRedirectURL = "https://www.facebook.com/connect/login_success.html";
        
        startTime = new Date().getTime();

        loginWindow = window.open(facebookLoginURL + '?client_id=' + fbAppId + '&redirect_uri=' + oauthRedirectURL +
            '&response_type=token&scope=' + scope, '_blank', 'location=no');

            loginWindow.addEventListener('loadstart', loginStartHandler);
            loginWindow.addEventListener('exit', loginExitHandler);
	};

    //TODO: Works for now, but maybe gigure out a better implementation for this, without having to access another html file..
    _self.logout = function(callback){
        var logoutWindow;
        var token = tokenStore['fbtoken'];
        
        tokenStore.removeItem('fbtoken');
        if (token) {
            logoutWindow = window.open(facebookLogoutURL + '?access_token=' + token + '&next=' + logout, '_blank', 'location=no');
            setTimeout(function() { logoutWindow.close(); }, 1000);
        }

        if (callback) {
            callback();
        }
    };
  
    /*Make a call to Facbeook's Graph API, pass it object with path,
      params and call backs
    */
    _self.api = function (obj) {

        var method = obj.method || 'GET',
            params = obj.params || {},
            xhr = new XMLHttpRequest(),
            url;

        params['access_token'] = tokenStore['fbtoken'];

        url = 'https://graph.facebook.com' + obj.path + '?' + toQueryString(params);

        xhr.onreadystatechange = function () {
            if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                    if (obj.success) obj.success(JSON.parse(xhr.responseText));
                } else {
                    var error = xhr.responseText ? JSON.parse(xhr.responseText).error : {message: 'An error has occurred'};
                    if (obj.error) obj.error(error);
                }
            }
        };

        xhr.open(method, url, true);
        xhr.send();
    };

module.exports = _self;