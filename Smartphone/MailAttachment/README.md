# Mail Attachment
This extension will allow the user to send an email with an attachment.

**Author:** [Brent Thornton](https://github.com/bthornton32) - Special thanks to plepleus for posting some of the code here: http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/Email-with-Attachment-from-Webworks-app/m-p/1760425#M24580

## Tested On

* BlackBerry Bold 9790 v7.1.0.221
* BlackBerry Bold 9900 v7.1.0.251
* BlackBerry Bold 9900 v7.0.0.540

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/bthornton32)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.mail.attachment**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\MailAttachment**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\MailAttachment**_ directory to your new _**ext\webworks.mail.attachment**_ directory

6. Copy the downloaded _**Smartphone\MailAttachment\webworks**_ directory to your new _**ext\webworks.mail.attachment\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.mail.attachment**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.mail.attachment" />

## Summary

	var result = webworks.mail.attachment.send(emailAddress, subject, body, filePath);

   
## Code Example
	<html>
		<head>
		<meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
		<script type="text/javascript" >
		function doClick(){
			var email = "test@test.com";
			var subject = "See attachment";
			var body = "Some text goes here";

			var file = "file:///myimage.png";

		    var result = webworks.mail.attachment.send(email, subject, body, file);
		    if (result === true) alert("Email sent.");
		    else alert("Error sending email.");
		}
		</script>
		</head>
		<body >
		<h1>Hello World</h1>
		<button onclick="doClick();">Send Email</button> 
		<div id="debug"></div>
		</body>
	</html>
    

## Usage Information
It is best to use this with the filePicker extension to ensure a true file path. Check your Messages after sending the email. It should appear in sent items.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


