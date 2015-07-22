# AccountEmailID Cordova/Webworks Plugin

The purpose of this BlackBerry 10 Cordova/Webworks plugin is to allow a BlackBerry 10 Cordova app to retrieve 
the email address of the emails accounts setup on the device. This can be used to identify a user automatically or to 
auto-populate a user's email address in form fields. This extension cannot retrieve the account infomation(email address)
from the opposite space/perimeter. In other words, app using this extension cannot pull email addresses of the
work/enterprise space from the personal space, and vice versa. 

The sample code for this application is Open Source under the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

**Applies To**

* [BlackBerry Native SDK](http://developer.blackberry.com/native/)

**Author** 

* [Shikhir Singh](http://code.shikhir.com/)


**Release History**

* **V1.0.0** - Initial Release

**Dependencies**

1. Minimal requirement - BlackBerry 10.1
2. Minimal requirement for BlackBerry 10 Webworks is **2.0**


**How to install this extension**

* In command prompt while in your project directory, type: cordova plugin add cordova-plugin-deviceemails

Example
```
cordova plugin add cordova-plugin-deviceemails
```
* In the config.xml of your project, the following permission will be added for you. It is required to access the email accounts.

```
<rim:permissions>
	<rim:permit>access_pimdomain_messages</rim:permit>
</rim:permissions>
```

**Usage**


* community.deviceemails.getDefaultEmailAddress(); // this returns an object that contains the default email if one is set. 
Example:
```
 
var pluginDefaultEmail = community.deviceemails.getDefaultEmailAddress();

if(pluginDefaultEmail.success==true){
	console.log(pluginDefaultEmail.defaultemail); // result is here!
}
else if(pluginDefaultEmail.success==false){
	var error = "I could not find any default email address. Is this because there are no emails accounts setup on this device?";
	console.log(error);
}					
```

* community.deviceemails.getEmailAddresses(); // this returns an object which contains all email addresses in that space(Personal or work)

 
**Tested on**

1. Z30 running v10.2

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR 
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.