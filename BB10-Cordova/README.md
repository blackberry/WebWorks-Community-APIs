# Contributing A BlackBerry 10 Plugin for PhoneGap, Cordova, and WebWorks 2.0

Start with the [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Template) sample and copy it to a new location, with a name change to reflect it's capabilities. Follow the instructions in the Template [readme](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/Template/README.md) to adapt it to your needs.

# Community extensions available

1. [Template](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Template)
	
	This is a template to start your PhoneGap, Cordova, or WebWorks 2.0 plugin development.

	1. test() - returns a string from native.
	2. testInput(string) - supplies input and returns it as part of another string.
	3. templateProperty - numeric property that can be set and retrieved.
	4. testAsync(input, callback) - asynchronous call that takes JSON data and sends a modified JSON object on the callback.
	5. startThread(callback) - starts a thread that fires events to a callback.
	6. stopThread(callback) - ends the thread that is firing the callback.

2. [Prevent Sleep](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/PreventSleep)
	
	This plugin allows you turn off screen dimming in your application.

	1.  setPreventSleep(boolean) - true prevents the screen from sleeping, false allows it to.
	2.  isSleepPrevented - true if sleep is prevented, false otherwise.

3. [Low Latency Audio](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/LowLatencyAudio)

	This plugin plays audio .wav files using openAL, ALUT, Qdir, and QtCore libraries.

	1. preloadAudio(data, path, voices, success, fail) - Preloads an audio file into a buffer so it can be played automatically without loading. Takes in three parameters, the file name, the path to the sounds, and how many unique voices it can have. Voices are how many simultaneous sounds that can be played at once.
    2. unload(data, success, fail) - Unloads an audio file from the current buffer it holds up. Used to free and allocate space in openAL. Takes in one parameter, file name.
    3. play(data, success, fail) - Plays an audio file using openAL. Takes in one parameter, file name.
    4. stop(data, success, fail) - Stops an audio file from playing or looping. Takes one parameter, file name.
    5. getDuration(data, success, fail) - Get the duration of a file. Takes one parameter, file name.
    6. loop(data, success, fail) - Loops an audio file indefinitely until either unloaded or stopped. Takes in one parameter, file name.
