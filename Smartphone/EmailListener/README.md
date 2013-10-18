# Email Listener The Email Listener will allow a WebWorks application to receive an Message object when an email is received. Tons of use for listening for particular messages, from particular people, subject lines, etc.

**Authors:** [Rich Balsewich](https://github.com/rbalsewich), [Jeff Bentley](https://github.com/jbentley), [Lei Hu](https://github.com/lhu), [Rajeev Massand](https://github.com/rmassand)

## Tested On

* BlackBerry Bold 9900

**Requires BlackBerry WebWorks SDK v2.1.1 or higher and BlackBerry handheld code 6.0+**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.message.email**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\EmailListener**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\EmailListener**_ directory to your new _**ext\webworks.message.email**_ directory

6. Copy the downloaded _**Smartphone\EmailListener\src\blackberry**_ directory to your new _**ext\webworks.message.email\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.message.email**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.message.email" />

## Summary

    webworks.message.email.startListening(eventType:  String, onEvent: function);
    webworks.message.email.stopListening(eventType:  String, onEvent: function);
	
	
## Usage Information

When an application starts, start your listener and define what function to call when Calendar-Appointment event occurs
## Code Example

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >

				webworks.message.email.startListening("", displayMessage);

				function displayMessage(servicename, msgObject){		
					alert("Message recevied from listener for service: " + servicename);					
						document.getElementById("main_screen_container").innerHTML += "<br /> Message recevied from listener for service: " + servicename;
						document.getElementById("main_screen_container").innerHTML += "<br />msgObject.from = " +  msgObject.from;
						document.getElementById("main_screen_container").innerHTML += "<br />msgObject.subject = " +  msgObject.subject;
						//check out the API for more Message Object properties there are tons
				}	
		 
        </script>
      </head>
      <body >
    		<div id="main_screen_container"></div>
      </body>
    </html>

