# Contributing A BlackBerry 10 Plugin for PhoneGap, Cordova, and WebWorks 2.0

Start with the [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Template) sample and copy it to a new location, with a name change to reflect it's capabilities. Follow the instructions in the Template [readme](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/Template/README.md) to adapt it to your needs.

# Community extensions available

## [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Template)
	
This is a template to start your PhoneGap, Cordova, or WebWorks 2.0 plugin development.

	1. test() - returns a string from native.
	2. testInput(string) - supplies input and returns it as part of another string.
	3. templateProperty - numeric property that can be set and retrieved.
	4. testAsync(input, callback) - asynchronous call that takes JSON data and sends a modified JSON object on the callback.
	5. startThread(callback) - starts a thread that fires events to a callback.
	6. stopThread(callback) - ends the thread that is firing the callback.

## [Prevent Sleep](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/PreventSleep)
	
This plugin allows you turn off screen dimming in your application.

	1.  setPreventSleep(boolean) - true prevents the screen from sleeping, false allows it to.
	2.  isSleepPrevented - true if sleep is prevented, false otherwise.

## [Low Latency Audio](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/LowLatencyAudio)

This plugin plays audio .wav files using openAL, ALUT, Qdir, and QtCore libraries.

	1. preloadAudio(data, path, voices, success, fail) 

Preloads an audio file into a buffer so it can be played automatically without loading. Takes in three parameters, the file name, the path to the sounds, and how many unique voices it can have. Voices are how many simultaneous sounds that can be played at once.

    2. unload(data, success, fail)
    
Unloads an audio file from the current buffer it holds up. Used to free and allocate space in openAL. Takes in one parameter, file name.
    
    3. play(data, success, fail)
    
Plays an audio file using openAL. Takes in one parameter, file name.
    
    4. stop(data, success, fail)
    
Stops an audio file from playing or looping. Takes one parameter, file name.
    
    5. getDuration(data, success, fail)
    
Get the duration of a file. Takes one parameter, file name.
    
    6. loop(data, success, fail)
    
Loops an audio file indefinitely until either unloaded or stopped. Takes in one parameter, file name.

## [Message Box](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/MessageBox)
	
This plugin maps the cross platform MessageBox API to the standard BlackBerry 10 Dialog API.

	1.  window.plugins.messageBox.alert(options, callback)
	
Displays an alert message defined according to options: 

* okButtonTitle: Default value 'OK'.
* title : A string describing the title of the alert box.
* message : A string with the contents of the confirm box. 

callback is a function that gets called as soon as the native code has done its work. Expected signature: callback(button), where button is a string that describes the selected option. Default value for button is 'ok'.
 
	2. window.plugins.messageBox.confirm(options, callback)
 	
Displays a confirm message defined according to options:

* yesButtonTitle: Default value 'Yes'.
* noButtonTitle: Default value 'No'.
* title : A string describing the title of the confirm box.
* message : A string with the contents of the confirm box.

callback is a function that gets called as soon as the native code has done its work. Expected signature:  callback(button), where button is a string that describes the selected option. Possible values for button are 'yes' or 'no'.

	3. window.plugins.messageBox.prompt(options, callback)
	
Displays a password prompt defined according to options:

* okButtonTitle: Default value 'OK'.
* cancelButtonTitle: Default value 'Cancel'.
* title : A string describing the title of the prompt box.
* message : A string with the contents of the prompt box.

callback is a function that gets called as soon as the native code has done its work. Expected signature: callback(button, value), where button is a string that describes the selected option (possible values: 'ok' or 'cancel') and value is a string containing the user's input. 
 

	
## [Clipboard](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Clipboard)
	
This plugin allows you access the text data in your clipboard.

	1.  setText(string) - Set <string> to clipboard,string length returned, or "-1" if error.
	2.  getText() - get the clipboard text.

## [Curl](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Curl)

This plugin provides simple HTTP-GET function using libcurl.

	1. get(url) - Returns the contents of the given URL

## [ExtractZipFile](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/ExtractZipFile)

This plugin unzips Zip files.

	1. extract(options, callback)

Options uses the following format:

	zip: 'folder1/zipFilename',
	destination: 'folder2/destination',
	overwriteFiles: true,
	tarBombProtection: false,
	callbackToken: ''


