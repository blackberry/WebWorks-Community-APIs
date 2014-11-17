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
	1.0.3 Supports automatic adding of vcf attachment or signature to email

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
	"Type": "html", //for plain text replace with "txt"
	"From": "12345678", //id of an email account on the device
	"To": "email@domain.com",
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body",
	"signature": document.getElementById('Signature').value,
	"attachment": "file:///accounts/1000/shared/documents/file.ext" //will also accept file path as "/accounts/1000/shared/...",
																	"/accounts/1000/removable/sdcard/...", or "./app/native/..."
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```

The fields "To", "Cc" and "Bcc" can be filled with a json array. Here's an exemple with the "To" field.

```javascript
// Build email JSON
var emailJSON = {
	"Type": "html",
	"From": "12345678", //id of an email account on the device
	"To": ["email_1@domain.com", "email_2@domain.com", "email_3@domain.com"],
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body",
	"signature": document.getElementById('Signature').value,
	"attachment": "/accounts/1000/shared/documents/file.ext"
};
var status = community.emailsenderplugin.sendEmail(emailJSON);
```
To use the default account use "-1" as the account id:
```javascript
// Build email JSON
var emailJSON = {
	"Type": "txt",
	"From": "-1", //use the default email
	"To": ["email_1@domain.com", "email_2@domain.com", "email_3@domain.com"],
	"Cc": "email_CC@domain.com",
	"Bcc": "email_BCC@domain.com",
	"subject" : "Email Subject",
	"body": "Email body",
	"signature": document.getElementById('Signature').value,
	"attachment": "/accounts/1000/removable/sdcard/My Files/file.dat"
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
**Automatically add email signature:**
For adding an automatic email signature to the message check the add signature checkbox in the index.html
A vCard file will be checked for first. If not found, it will look for a text file and parse the data into the email body.
The signature files must be saved in the following directory:
```javascript
    /www/res/signature/
```
Associated email account:
```javascript
    "From": "12345678" //id of an email account associated to testaccount@gmail.com
```
The .vcf or .txt file must be in the format of the sender email address that the message is been sent from.
```javascript
	testaccount.vcf
	    or
    testaccount.txt
```

## Permissions

The plugin require the following permissions:
<pre>
<b>access_pimdomain_contacts</b>
<b>access_pimdomain_messages</b>
<b>access_shared</b>
</pre>

## Known Issues
As of 09-Oct-2014, the plugin can't send email from a Microsoft account(hotmail, live, outlook, etc).
17-Nov-2014 - When sending emails as html, multiple lines are formatted into one continuous line.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.