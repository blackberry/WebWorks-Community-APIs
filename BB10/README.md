# Contributing A BlackBerry 10 Extension

Start with the [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/Template) sample and copy it to a new location, with a name change to reflect it's capabilities. Follow the instructions in the Template [readme](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10/Template/README.md) to adapt it to your needs.

# Community extensions available

1. [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/Template)
	1. test() - returns a string from native.
	2. testInput(string) - supplies input and returns it as part of another string.
	3. templateProperty - numeric property that can be set and retrieved.
	4. testAsync(callback) - asynchronous call that fires the callback when complete.
	5. testAsyncJSON(callback) - asynchronous call that fires a callback which receives a JSON object.
	6. testAsyncJSONio(json, callback) - asynchronous call that takes JSON data and sends a modified JSON object on the callback.
	7. startThread(callback) - starts a thread that fires events to a callback.
	8. stopThread(callback) - ends the thread that is firing the callback.
2. [Prevent Sleep](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/PreventSleep)
	1.  setPreventSleep(boolean) - true prevents the screen from sleeping, false allows it to.
	2.  isSleepPrevented - true if sleep is prevented, false otherwise.
3. [LED](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/LED)
	1. var id = community.led.startLed("FFFF00", "10") - start the LED blinking with the given colour, for the number of times requested. 0 is indefinite.
    2. community.led.stopLed(id) - stop the LED blink request with the given id.
4. [thumbnail](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/thumbnail)
	1. createThumbNail(path, {width: width, height: height, quality:quality}) - Create a thumbnail of the image at "path" with the given options.
5. [UNZIP](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/UNZIP)
	1. unzipPackage("shared/documents/bbtest.zip", "shared/documents/") - unzip the given file to the directory given.
6. [VIBRATION](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/VIBRATION)
	1. vibration_request({ "duration" : [int: duration], "intensity" : [int: intensity]}, [function: callback]) - vibrate phone with given intensity and duration, with optional callback.
7. [BarcodeScanner](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/BarcodeScanner)
	1.  startRead(codeFound, errorFound, "myCanvas", onStartRead) - Scan for barcodes and paint viewfinder on myCanvas
	2.  stopRead(onStopRead, errorFound) - stop the reading process
8. [ScreenDisplay](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/ScreenDisplay)
	1. var result = community.screendisplay.sdgetsize();

	Result is a javascript object with the following properties

	```javascript

		pixelWidth,      // Pixel Size - width
		pixelHeight,     // Pixel Size - height
		physicalWidth,   // Physical Size mm - width
		physicalHeight,  // Physical Size mm - height
		ppmm,            // Pixels Per mm
		ppmmX,           // Pixels Per mm - X
		ppmmY,           // Pixels Per mm - Y
		ppi,             // Pixels Per Inch
		ppiX,            // Pixels Per Inch - X
		ppiY,            // Pixels Per Inch - Y
		pixelShape;      // Physical Shape
	```

9. [deviceInfo](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/deviceInfo)
	1. getModelNumber() - returns model number such as 'Z10', 'Q10', 'Q5'
	2. getRoamingStatus() - return Roaming Status of the device as 'true' or 'false'
	3. getMCC() - returns Mobile Country Code
	4. getMNC() - returns Mobile Network Code, that identifies the carrier
	5. getNetwork() - returns a JSON object with information about the current default connection.
	6. hasPhysicalKeyboard() - returns 1 if device has a hardware keyboard, or 0 otherwise.

10. [gseCrypto](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/gseCrypto)
	1. hash( params ) - Hash using any SHA or AES CBC algorithm
	2. generateKey( params ) - generate a key using AES CBC
	3. encrypt( params ) - encrypt the input using AES CBC
	4. decrypt( params ) - decrypt using AES CBC
	5. random( size: int ) - return a random data string of the given length
	
11. [clipboard](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/clipboard)
	1. community.clipboard.getText() - Get text from clipboard.
	2. community.clipboard.setText( string ); - Set string to clipboard.



