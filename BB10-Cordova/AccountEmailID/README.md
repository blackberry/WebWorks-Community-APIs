# Device Emails Cordova/Webworks Plugin

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

* In command prompt while in your project directory, type: webworks plugin add <path to the /plugin/ directory>

Example
```
webworks plugin add ../plugin
```
* In the config.xml of your project add the following permissions (see the config.xml in /DeviceEmails-WW2-AppDemo/www for an example)

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

 
**How to Build Device Emails for BB10 Webworks 2.0 Beta**

Note: It is not necessary to build this project if you do not intend to modify the extension. The plugin folder already has a built copy of the
extension, simply install it. 

Assumption: You have installed the BlackBerry Native SDK 2.0+, and BlackBerry Webworks 2.0+

1. Simply import the plugin/src/blackberry10/native directory into a workspace into Momentics. Build the project. 
2. Go to the DeviceEmails-DemoApp directory, then add the plugin using the following command: cordova plugin add ../plugin/
3. Now you can build the project using : webworks build --release  

**How can I know that this plugin actually works without building it all myself!**

If you don't want to build this sample application yourself we've included a pre-built and signed BAR file. 
You can find it in the folder /build folder. 


**Tested on**

1. Z30 running v10.2


**Known Issues**
Cordova 3.4 now keeps it config.xml in a different location than the previous version(3.3).
WebWorks 2.0 Beta still keeps it's config.xml like version 3.3 of Cordova in the /www
folder. The sample project DeviceEmails-AppDemo has it's config.xml in the /www folder so it will
work with WebWorks 2.0 Beta. The plugin will work with Cordova 3+ or WebWorks 2.0+ but 
to package the sample project the config.xml will need to be moved for it to work with Cordova 3.4. 
This does not effect the plugin, and is only in reference to the sample project.  


## Contributing Changes

Please see the [README](https://github.com/blackberry/BB10-WebWorks-Community-Samples) of the BB10-WebWorks-Community-Samples repository for instructions on how to add new Samples or make modifications to existing Samples.



## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR 
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.