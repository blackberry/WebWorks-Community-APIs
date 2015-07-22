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

**How to install this extension**

In command prompt while in your project directory, type: cordova plugin add cordova-plugin-bb-sms

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
		console.log('SMS Plugin is not installed. You need to run the command : cordova plugin add cordova-plugin-bb-sms');
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
			console.log('SMS Plugin is not installed. You need to run the command : cordova plugin add cordova-plugin-bb-sms');
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