# Calendar-Appointment Listener The Calendar (BlackBerry lingo)/ Appointment (Outlook lingo) will allow a WebWorks application to receive an Appointment object when any insert, update, delete event happens.  Since the Calendar is in the Event Category, this extension can EASILY be tweaked to include other Events such as:  Memos, ToDos, Tasks, and more

**Authors:** [Rich Balsewich](https://github.com/rbalsewich), [Brent Thornton](https://github.com/bthornton32)

## Tested On

* BlackBerry Bold 9900

**Requires BlackBerry WebWorks SDK v2.1.1 or higher and BlackBerry handheld code 6.0+**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.pim.appointment**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\CalendarListener**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\CalendarListener**_ directory to your new _**ext\webworks.pim.appointment**_ directory

6. Copy the downloaded _**Smartphone\CalendarListener\src\blackberry**_ directory to your new _**ext\webworks.pim.appointment\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.pim.appointment**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.pim.appointment" />

## Summary

    webworks.pim.appointment.startListening(eventType:  String, onEvent: function);
    webworks.pim.appointment.stopListening(eventType:  String, onEvent: function);
	
	
## Usage Information

When an application starts, start your listener and define what function to call when Calendar-Appointment event occurs
## Code Example
		/*
		* Copyright 2010-2012 Research In Motion Limited.
		*
		* Licensed under the Apache License, Version 2.0 (the "License");
		* you may not use this file except in compliance with the License.
		* You may obtain a copy of the License at
		*
		* http://www.apache.org/licenses/LICENSE-2.0
		*
		* Unless required by applicable law or agreed to in writing, software
		* distributed under the License is distributed on an "AS IS" BASIS,
		* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		* See the License for the specific language governing permissions and
		* limitations under the License.
		*/

    <html>
      <head>
        <meta name="viewport" id="viewport" content="height=device-height,width=device-width,user-scalable=no" />
        <script type="text/javascript" >

				webworks.pim.appointment.startListening("", displayMessage);

				function displayMessage(servicename, aptObject){		
					alert("Message recevied from listener for service: " + servicename);					
					document.getElementById("main_screen_container").innerHTML += "<br /> Message recevied from listener for service: " + servicename;
					document.getElementById("main_screen_container").innerHTML += "<br />aptObject.summary = " +  aptObject.summary;
					document.getElementById("main_screen_container").innerHTML += "<br />aptObject.start = " +  aptObject.start;
				}	
		 
        </script>
      </head>
      <body >
    		<div id="main_screen_container"></div>
      </body>
    </html>

