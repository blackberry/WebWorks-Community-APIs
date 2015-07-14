# SMS Plugin for BB10 Cordova / Webworks

The purpose of this Cordova/Webworks plugin is to allow developers to send an SMS from within a Cordova/Webworks project for BlackBerry 10. 
This plugin differs from [invoke](https://developer.blackberry.com/html5/documentation/beta/text_messages.html) 
because the Cordova app itself can send the SMS, where as the invoke feature will launch the SMS app. 

The sample code for this application is Open Source under the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).

**Author** 

* [Shikhir Singh](http://code.shikhir.com/)
* [Martin Woolley](https://github.com/mdwoolley)


**Release History**

* **V1.0.0** - Initial release

**Dependencies**

1. Minimal requirement for BlackBerry 10 Device Software is **10.0.9**
2. Minimal requirement for BlackBerry 10 Native SDK is **10.0.9**
3. Minimal requirement for BlackBerry 10 Webworks is **2.0**


**How to install this extension**

In command prompt while in your project directory, type: cordova plugin add <path to the /plugin/ directory>

**Permissions**

In the config.xml of your project the following permissions will be added on installation of the plugin.

```xml
	<rim:permissions>
	    <rim:permit>access_pimdomain_messages</rim:permit>
	    <rim:permit>access_sms_mms</rim:permit>
	    <rim:permit>access_phone</rim:permit>
	</rim:permissions>
```

**Usage**

Sync Example:
```javascript
	var param = {
		"destination": "2124537535",  
		"message": "What's up Barack?",
		"personName": "Barack Obama" // can be any string 
	};
	
	var result = community.SMSPlugin.udSMS_Sync(param);
	
	if(result===false){
		console.log('SMS Plugin is not installed. You need to run the command : cordova plugin add ../plugin');
	}
	else{
		if(result.success==false){
		// did you forget a parameter?
			console.log(result.result);
		}
		else{
			console.log('SMS has been sent!');
		}
	}
```

Async Example:
```javascript
	var param = {
		"destination": "2124537535",  
		"message": "What's up Barack?",
		"personName": "Barack Obama" // can be any string 
	};

	var callback = function(result) {
		if(result===false){
			console.log('SMS Plugin is not installed. You need to run the command : cordova plugin add ../plugin');
		}
		else{
			if(result.success==false){
			// did you forget a parameter?
				console.log(result.result);
			}
			else{
				console.log('SMS has been sent!');
			}
		}

	}

	community.SMSPlugin.udSMS_Async(param, callback);
```
 
**How to Build the SMS Plugin for BB10 Cordova**

Assumption: You have installed the BlackBerry Native SDK 2.0+

- Simply import the plugin/src/blackberry10/native directory into a workspace in your NDK. Build the project.


**How to test the SMS Plugin using the demo app provided**

Assumption: You have installed the BlackBerry WebWorks 2.0+ or Cordova 3.3+. If you are using WebWorks instead of Cordova, simply use the command webworks instead of cordova below.

1. Create a Cordova Project : 
```
$ cordova create SMSPlugin-DemoApp community.SMSPlugin.demo SMSPlugin-DemoApp
```

2. If you are using cordova, type you will need to add the BlackBerry platform. If you are using WebWorks 2.0+, ignore this step. 
```
$ cd SMSPlugin-DemoApp
$ cordova platform add blackberry10
```


3. Go to the SMSPlugin-DemoApp directory which was just created, then add the plugin using the following command: 
```
$ cordova plugin add /path/To/plugin/folder
```

4. Delete the folder SMSPlugin-DemoApp/www and replace it with the www folder provided

5. Now you can build the project using: 
```
$ cordova build --release --keystorepass YOUR_PASSWORD_GOES_HERE -buildId 1 
```

**How can I know that this plugin actually works without building it all myself!**

If you don't want to build this sample application yourself we've included a pre-built and signed BAR file. 
You can find it in the folder /build folder. 


**License**

Apache 2.0

**Tested on**

1. Z30 running v10.2
2. Q10 running v10.1


**Known Issues**

None! 

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR 
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.