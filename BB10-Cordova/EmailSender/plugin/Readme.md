# community.emailSender Native Extension for applications built using BlackBerry 10 WebWorks

This extension provides additional APIs to send emails from an app.

**Applies To**

* [BlackBerry 10 WebWorks SDK](https://developer.blackberry.com/html5/download/sdk) 

**Author(s)** 

* [Alexandre Huot](https://github.com/alexhuot1/)
* [Stefan Schielke](https://github.com/stefanschielke)

## Version History

	1.0.0 Initial Release
	1.0.1 Adds method for getting Email Accounts
	1.0.2 Now supports HTML emails in addition to plaintext

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
**Here's the usage for the sendEmail() method:**
```javascript
// Build email JSON
var emailJSON = {
	"From": "12345678", //id of an email account on the device
	"To": "email@domain.com",
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body"
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```

The fields "To", "Cc" and "Bcc" can be filled with a json array. Here's an exemple with the "To" field.

```javascript
// Build email JSON
var emailJSON = {
	"From": "12345678", //id of an email account on the device
	"To": ["email_1@domain.com", "email_2@domain.com", "email_3@domain.com"],
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body"
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```
To use the default account use "-1" as the account id:
```javascript
// Build email JSON
var emailJSON = {
	"From": "-1", //use the default email
	"To": ["email_1@domain.com", "email_2@domain.com", "email_3@domain.com"],
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body"
};
```
**Here's the usage for the getEmailAccounts() method:**
```javascript
// Build email JSON, dont' forget to parse it!
var emailAccounts = JSON.parse(community.emailsenderplugin.getEmailAccounts());
```

**Here's the return format:**
```javascript
{
	"account1Id":"email_1@domain.com",
	"account2Id":"email_2@domain.com",
	"account3Id":"email_3@domain.com",
	"account4Id":"email_4@domain.com"
}
```
## Permissions

The plugin require the following permissions:
<pre>
<b>access_pimdomain_contacts</b>
<b>access_pimdomain_messages</b>
</pre>

## Known Issues
As i'm writing those lines, the plugin can't send email from a microsoft account(hotmail, live, outlook, etc) and Yahoo! accounts.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.