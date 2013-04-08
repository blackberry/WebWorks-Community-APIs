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


