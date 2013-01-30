Extension-Template
==================

BlackBerry 10 WebWorks Extensions create a bridge between JavaScript code in an HTML5 application and C/C++ and/or Qt code in the extension, allowing access to native libraries, which are not part of the HTML5 specification. The BlackBerry WebWorks APIs included with the WebWorks SDK are also Extensions and follow the same format as shown in this template.

Extensions and WebWorks APIs offer a JavaScript API which communicates through an interface to native methods. This interface can send and receive data through strings, or data that can be represented as a string such as JSON and Base64.

This template includes examples for communicating in several ways:

1. A function that returns a string from a native method.
2. A function that sends a string to, and gets another back, from a native method.
3. A function that calls a native method and provides a callback function, which is fired by a native method.
4. A function that does the same as #3 but gets a JSON object on the callback function.
5. A function that sends a JSON object into a native method with a callback function to receive the result.
6. Defining a property that is linked to native methods when retrieved or set.
7. Starting and Stopping a native thread, with a callback function that receives events from the native thread.

## Building and Testing the Sample

1. Deploy the extension into your BB10 WebWorks SDK by copying the community.templateExt folder from the ext folder to the Framework/ext folder in your WebWorks SDK, or run the Ant build script in the /ext folder to do so.
2. Package the sample in the sample\_application folder. Use the [Ant Build Script](https://github.com/blackberry/BB10-WebWorks-Community-Samples/tree/master/Ant-Build-Script), or you can do this with the WebWorks packager command:
	__bbwp__ sample\_application -g _signingpassword_
3. Load it on your phone using the Ant Build Script, or with the _blackberry-deploy_ command found in the dependencies/tools folder of your WebWorks SDK.
4. When the application runs, a series of divs on the screen will appear with an orange background, and the extension methods will be called. When sucessful results or retrieved, the div backgrounds will change to blue as the screenshot below.

![Screenshot](/WebWorks-Community-APIs/raw/master/BB10/Template/screenshot.png)

## Starting an Extension from the Template

Copy the Template folder to a location on your computer to start working with it.

### Momentics NDK setup

1. Open the Momentics IDE. Navigate to the workbench and from the program menu
select File -> Import and choose "Existing Projects into Workspace".
2. Choose "Select root directory: " and browse to the /NDK_project directory where you copied the Template. Select the Template project in the Projects list and uncheck "Copy projects into workspace". Click Finish.
3. Follow these next steps to build the template extension to be sure the setup is working.

#### How to build your native Extension</a>

1. Right click your project and select the Clean Project option.
2. Right click your project again and select Build Configurations -> Build Selected... .
3. A window will appear that shows all the available build configurations
for the project. Select Device-Release and Simulator-Debug and click ok.
4. You should see the shared libraries generated in the folders for each Build Configuration that you selected.
5. Copy the .so file(s) to the device and simulator folders under the ext/community.templateExt directory. Or you can use the Ant build script to do so.

### Copying the Extension to the SDK</a>

1. Under the ext folder in this project should be the community.templateExt folder with the following file structure underneath it:

![Screenshot](/WebWorks-Community-APIs/raw/master/BB10/Template/templatefiles.png)

If you wish to do the copying manually, continue with these steps:

2. The device and simulator folders should contain the libTemplate.so files built by the NDK.
3. Copy the community.templateExt folder into your WebWorks SDK, placing it in the Framework/ext directory.

If you have Ant installed and would like to use the build script provided, edit the script as follows (you will find it in the /ext directory, called [build.xml](/WebWorks-Community-APIs/tree/master/BB10/Template/ext/build.xml) ).

1. Change the project name to match the extension name in manifest.json.
2. Change the bbwp.native.dir value to match the SDK you want to deploy to.
3. Type ant at a command prompt while in the /ext directory of the Extension. 

The build script will clean any old extension from your SDK, copy the .so files build by the NDK into the correct locations, and finally copy the whole extension into the SDK.

### Using the Extension in an Application</a>

With the extension copied into your WebWorks SDK as explained above, you can use it in your applications by referencing it as a feature in your config.xml file. 

```xml
<feature id="community.templateExt" />
```

All the methods in the extension will be prefixed by that feature id, so a method called test() supplied in the community.templateExt extension will be called in JavaScript like so:

```javascript
community.templateExt.test();
```

Depending on the extension, these methods can also have return values, take arguments, or supply callback methods.

### Modifying the Template Extension

#### Namespace
Choose a amespace for your application that is concise and descriptive. Use community.extensionName as the general format. Set this value in manifest.json in the "namespace" property, and rename the directory of the extension to match. Also use the namespace in the event names for Asynchronous methods called by JNEXT.

#### JNEXT plugin files
templateJNEXT.js and the template_js.cpp/hpp files are linked by the name of the library and the name of the class in the library. Change the Class Name from TemplateJS to something that matches the new extension's role. Change the project name in the NDK as well. Then you will need to update the references in templateJNEXT.js to match the new values:

Find self.init = function () {}, and change __libTemplate__ to the new project name, but keep the __lib__ prefix. Then change the reference to __TemplateJS__ to use the new Class Name.
```javascript
self.init = function () {
	if (!JNEXT.require("libTemplate")) {
		return false;
	}

	self.m_id = JNEXT.createObject("libTemplate.TemplateJS");

	...
};
```
#### Update .js files
There are many other places where "template" is used in file names and methods in the JavaScript files. These can be changed to better reflect the new extension API. Note carefully where index.js and templateJNEXT.js refer to each other so that you don't break the chain:

1. [index.js calls require on the .template property of templateJNEXT](/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/index.js#L17).
2. [The Template object is defined in templateJNEXT.js](/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/templateJNEXT.js#L24).
3. [the Template object is returned at the end of templateJNEXT.js](/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/templateJNEXT.js#L120-123).

#### Change NDK files
In the files template_ndk.hpp and template_ndk.cpp, there are plenty of uses of "Template" and "template" which can be changed to match the new extension.

#### Rebuild 
When making changes, rebuild regularly so you don't make a really hard to find typo.

Follow the steps above to:
1. [Build the native portion](#how-to-build-your-native-extension),
2. [Copy the extension to your SDK](#copying-the-extension-to-the-sdk), and
3. [Use the extension in your test app](#using-the-extension-in-an-application).

## Architecture of an Extension

Extensions are organized into several files, each with a specific role.

1. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/manifest.json">manifest.json</a> - defines the namespace of the extension and any dependencies it has on other extensions.
2. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/client.js">client.js</a> - the client file defines the API that can be called by an application. It calls to functions in index.js using the WebWorks SDK. It also connects callback functions to the events that fire them.
3. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/index.js">index.js</a> - This is the main controller of the extension. It receives calls from the client.js through WebWorks, and calls the appropriate methods in templateJNEXT.js.
4. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/ext/community.templateExt/templateJNEXT.js">templateJNEXT.js</a> - This file handles communication with the native code through the JNEXT plugin. It is kept in a separate file and set of methods to reduce the dependency on JNEXT.
5. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/NDK_project/src/template_js.hpp">template_js.hpp</a> - C++ header for the JNEXT code. You should rarely have to edit this aside from renaming.
6. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/NDK_project/src/template_js.cpp">template_js.cpp</a> - C++ code for the JNEXT plugin. Besides renaming, the primary editing of this will be to edit the InvokeMethod function to call the appropriate methods in template_ndk.cpp, given the command sent in from templateJNEXT.js.
7. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/NDK_project/src/template_ndk.hpp">template_ndk.hpp</a> - C++ header for the native code. Where you will need to list method signatures for the extenion methods called from the template_js.cpp file, and any private methods or variables.
8. <a href="/WebWorks-Community-APIs/tree/master/BB10/Template/NDK_project/src/template_ndk.cpp">template_ndk.cpp</a> - C++ native code. All the main native code will go here typically. 

## Communication end to end

Extension methods can be synchronous or asynchronous, send arguments, receive return values, and provide callback methods.

#### Synchronous Methods

Synchronous methods call:
```javascript
window.webworks.execSync(_ID, "methodname", args);
```
in client.js, where args is a JSON object defining the various arguments. This method calls
```javascript
methodname(success, fail, args)
```
in index.js, where the success and fail callbacks are provided by Webworks, and args is an object passed in to the execSync method, but it has been converted by JSON.stringify() and encoded as a URIcomponent, so you must call 
```javascript
JSON.parse(decodeURIComponent(args["input"]));
```
to access a value that you sent in through client.js with 
```javascript
window.webworks.execSync(_ID, "methodname", {"input": arg });
```

Inside index.js, a synchronous function will typically fire the success callback, and pass in the result of the appropriate JNEXT function, looking like the following:

```javascript
testInput: function (success, fail, args) {
	args = JSON.parse(decodeURIComponent(args["input"]));
	success(template.getInstance().testInput(args));
}
```
In this method, template.getInstance() returns the singleton copy of the template JNEXT object. The testInput method looks like the following as defined in templateJNEXT.js:
```javascript
self.testInput = function (input) {
    return JNEXT.invoke(self.m_id, "testStringInput " + input);
};
```
If input is a JSON object, we need to convert it to a string first with JSON.stringify(input).
Now the communication passes into the native layer. JNEXT.invoke eventually calls into:
```cpp
string TemplateJS::InvokeMethod(const string& command) {
	int index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());
	...
	} else if (strCommand == "testStringInput") {
		return m_pTemplateController->templateTestString(arg);
	}
	...
}
```
The method name is stripped off the front of the command sent in, and the rest is sent in as an argument for the appropriate method. Note the use of the pointer to the TemplateNDK object, so we separate our regular native code from the JNEXT code, reducing the dependency again, as done in the JavaScript code.

Finally, we call the native method that contains the functionality we are providing to our WebWorks application, and in this example, it takes in the arguments, appends them to a string and returns the result to WebWorks.
```cpp
// Take in input and return a value
std::string TemplateNDK::templateTestString(const std::string& inputString) {
	return "Template Test Function, got: " + inputString;
}
```
If the input is a JSON object, we need to parse it first, as shown here:
```cpp
// Parse the arg string as JSON
Json::Reader reader;
Json::Value root;
bool parse = reader.parse(inputString, root);
```
To return JSON, use 
```cpp
Json::FastWriter writer;
Json::Value root;
root["result"] = "result value";
return writer.write(root);
```

#### Asynchronous Methods

An Asynchronous method will typically provide a callback function to receive the results of a method, but this is not necessarily the case for all situations. The sequence is much the same as that of Synchronous Methods with these differences:

Asynchronous methods call:
```javascript
window.webworks.execAsync(_ID, "methodname", args);
```
in client.js, and will usually register a callback function for a one-time result with the code:
```javascript
window.webworks.event.once(_ID, "extension.namespace.callbackEventName", callback);
```
The call into index.js functions as above, but then the JNEXT function will be called separately from success(), which is fired right afterwards.
```javascript
// Asynchronous function calls into the plugin and returns
testAsync: function (success, fail) {
	template.getInstance().testAsync();
	success();
}
```
Now the code proceeds as for Synchronous events, until the template_ndk.cpp method, where instead of returning the result directly, the NotifyEvent method is used instead:
```cpp
// Asynchronous callback
void TemplateNDK::templateCallback() {
	std::string event = "community.templateExt.aSyncCallback";
	m_pParent->NotifyEvent(event);
}
```
Data can also be sent by appending it to the string, with a space separator. This event passes through the JNEXT plugin and is received on the onEvent(strData) method of templateJNEXT.js:
```javascript
// Fired by the Event framework (used by asynchronous callbacks)
self.onEvent = function (strData) {
	var arData = strData.split(" "),
		strEventDesc = arData[0],
		jsonData;
	// Event names are set in native code when fired,
	// and must be checked here.
	if (strEventDesc === "community.templateExt.aSyncCallback") {
		_event.trigger("community.templateExt.aSyncCallback");
	} else if (strEventDesc === "community.templateExt.aSyncJSONCallback") {
		// Slice off the event name and the rest of the data is our JSON
		jsonData = arData.slice(1, arData.length).join(" ");
		_event.trigger("community.templateExt.aSyncJSONCallback", JSON.parse(jsonData));
	}
	...
};
```
Note that the event names match those sent from within the native code, but the events triggered here do not necessarily have to match the native code - they must match the event names that were set using the webworks event framework. Keeping them identical throughout will likely be safer. The Best Practice is to include the namespace of the Extension so that there are no collisions with other Extensions working with JNEXT events.

The trigger method will fire the callback registered with that name, and pass in the arguments provided as so:
```javascript
function aSyncJSONCallback(data) {
	if (data) {
		console.log(data);
	}
}
```

#### Callbacks for multiple events

The Asynchronous methods above use one-time callbacks to receive their results. Sometimes it is important to receive multiple events on a callback, such as when listening to a thread or longer process.

This is done as above but using the add and remove methods on the event framework, but otherwise with the same method signature as 'once':
```javascript
_self.startThread = function (callback) {
	window.webworks.event.add(_ID, "community.templateExt.jsonThreadEvent", callback);
	return window.webworks.execSync(_ID, "startThread", null);
};

_self.stopThread = function (callback) {
	window.webworks.event.remove(_ID, "community.templateExt.jsonThreadEvent", callback);
	return window.webworks.execSync(_ID, "stopThread", null);
};
```
At the native layer, the NotifyEvent method can be called multiple times and be received by the callback in this way.

#### Defining Properties

Properties can be defined on the extension so that code like:
```javascript
community.templateExt.templateProperty;
```
returns a value, and it can be set by the following code:
```javascript
community.templateExt.templateProperty = value;
```
The following code defines a property called templateProperty, backed by the extension method of the same name for setting or retrieving the value. The getter or setter could be removed to restrict what the property responds to.
```javascript
Object.defineProperty(_self, "templateProperty", {
	get: function () {
		return window.webworks.execSync(_ID, "templateProperty", null);
	},
	set: function (arg) {
		window.webworks.execSync(_ID, "templateProperty", {"value": arg });
	}
});
```

## Debugging

Debugging extensions is difficult, but there are some ways that can be effective. The extension Javascript files are loaded through an XmlHttpRequest, so you can not set breakpoints directly on that code with WebInspector, but you can set breakpoints where you call the extension methods in your code and keep stepping into the methods to follow the chain.

Simple JavaScript alerts also work in the extension files and this can be invaluable to check the value of some data being passed around. Since extensions deal with strings, it's easy to check.

### Common Issues
If you are getting a message saying the application can not load your .so file, it's in the wrong place or the JNEXT javascript code doesn't have the right name for it, or there's a runtime issue that is stopping the .so file from being loaded. Trying your code in the Test Harness should find the problem.

If your application launches but quickly freezes and then is closed, you may be using a BPS API but haven't called
```cpp
bps_initialize();
```
It is required for things like LED, Vibration, Audio and many others. See [more information here](http://developer.blackberry.com/native/reference/bb10/com.qnx.doc.bps.lib_ref/com.qnx.doc.bps.lib_ref/topic/bps_initialize.html).

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.