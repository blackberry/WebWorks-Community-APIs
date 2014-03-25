# community.emailSender Native Extension for applications built using BlackBerry 10 WebWorks

This extension provides additional APIs to send emails from an app.

**Applies To**

* [BlackBerry 10 WebWorks SDK](https://developer.blackberry.com/html5/download/sdk) 

**Author(s)** 

* [Alexandre Huot](https://github.com/alexhuot1/)

## Set up
From registry
Use the command:

    cordova plugin add com.blackberry.community.emailsenderplugin

or
    
    webworks plugin add com.blackberry.community.emailsenderplugin

From source

Copy the plugin folder to the destination of your choice.
Browse to you webworks project folder.
Use the command: 

    webworks plugin add pathToPluginFolder

## Use
```javascript
// Build email JSON
var emailJSON = {
	"To": "email@domain.com",
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body"
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```

The fields "To", "Cc" and "Bcc" can be filled with a json array. Here's an exemple with the field "To".

```javascript
// Build email JSON
var emailJSON = {
	"To": ["email_1@domain.com", "email_2@domain.com", "email_3@domain.com"],
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body"
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```

## Permissions

The plugin require the following permissions:
<pre>
<b>access_pimdomain_contacts</b>
<b>access_pimdomain_messages</b>
</pre>


## Contributing Changes

Please see the [README](https://github.com/blackberry/WebWorks-Community-APIs) of the WebWorks-Community-APIs repository for instructions on how to add new Samples or make modifications to existing Samples.


## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/WebWorks-Community-APIs//issues) for the Sample and send a message (via github messages) to the Sample Author(s) to let them know that you have filed an [Issue](https://github.com/blackberry/WebWorks-Community-APIs//issues).

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/DeviceInfo?pixel)](https://github.com/igrigorik/ga-beacon)
