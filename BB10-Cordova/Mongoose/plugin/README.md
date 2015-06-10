Mongoose
=============

This BlackBerry 10 Cordova Plugin creates a default Mongoose embedded web server that can be started or stopped with commands from JavaScript. 

** Tested On **

BlackBerry Q5 10.2.1.1927

Date: 4/17/2014

** Author **

Extension - Peardox

Mongoose - Valenok

Port - Deric Cheng (https://github.com/dericc)

## Including the feature in your application

This API can be installed from source or from NPM. Installation from NPM is done through the following:

	cordova plugin add cordova-plugin-mongoose

Installation from source is the same but instead of the id ("cordova-plugin-mongoose"), use the file system path to the source plugin folder.

## The Plugin API
The Mongoose plugin provides the following API:

```javascript
var result = community.mongoose.start(options); // Start the server using the defined options (see below)

/*
Result is a javascript object with the following properties - status and error are always returned the rest depend on circumstances

status,          // Did the server start - true or false
error,           // false or an error message
command_errors   // Number of passed options that were invalid
listening_ports, // Port the web server is running on
document_root,   // Base path documents are being served from
mongoose_version // Version of Mongoose in this release

*/
```

```javascript
community.mongoose.stop();  // Stop the server
```

This function stops the Mongoose server. It returns a Javascript object with the status set to true or false, depending on whether the stop() function was successfully executed.

```
## Options passed to community.mongoose.start();

"cgi_pattern"               "**.cgi$|**.pl$|**.php$"
"cgi_environment"           NULL
"put_delete_auth_file"      NULL
"cgi_interpreter"           NULL
"protect_uri"               NULL
"authentication_domain"     "mydomain.com"
"ssi_pattern"               "**.shtml$|**.shtm$"
"throttle"                  NULL
"access_log_file"           NULL
"enable_directory_listing"  "yes"
"error_log_file"            NULL
"global_auth_file"          NULL
"index_files"               "index.html,index.htm,index.cgi,index.shtml,index.php"
"enable_keep_alive"         "no"
"access_control_list"       NULL
"extra_mime_types"          NULL
"listening_ports"           "8080"
"document_root"             "."
"ssl_certificate"           NULL
"num_threads"               "20"
"run_as_user"               NULL
"url_rewrite_patterns"      NULL
"hide_files_patterns"       NULL
"request_timeout_ms"        "30000"

Note that the extension overrides the default for document_root and enable_directory_listing with $HOME/data and no respectively

```

Detailed descriptions of what all options do is available at ...

https://github.com/valenok/mongoose/blob/master/UserManual.md

These options should be passed as a JSON object with document_root being the VITAL one to set

e.g.

var options = { 'document_root' : 'shared/images' };

If supplied with an empty object the extension defaults to {home}/data

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
