# QtBridge #
QtBridge is a WebWorks 2.0/Cordova native extension that provides a dynamic bridge between Qt and JavaScript allowing you to leverage Qt APIs directly from javascript, including the use of Qt signals and slots. With QtBridge you can easily create a custom qt class and expose it directly to javascript (see Custom Qt Classes below).

**Author**

* [Isaac Gordezky](http://www.twitter.com/igordezky)

# Javascript API #

The api exports a global module as Qt and blackberry.qt.

A simple example is shown here:

    if (Qt.initialize()) {
        var deviceInfo = Qt.createObject("bb.device.DeviceInfo");
        console.log("Current Orientation: " + deviceInfo.orientation);
        var connectionId = deviceInfo.connect("orientationChanged", function (newOrientation) {
            console.log("Orientation changed to " + newOrientation);
        });
    } else {
        console.log(“Could not initialize QtBridge.”);
    }

See the [Sample Application](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/QtBridge/SampleApp) for an full example of how to use the API.

## Object Methods ##

Object methods are available on any object created by QtBridge.

### introspect() ###
Introspect returns a json representation of a QObject. This method is used internally to define object properties and functions when an object is created. Use this method if you want to inspect the details of a QObject class that are not reflected in the resulting object structure: e.g. which signals are change events for which properties.

### connect(string signal, function slot) ###
Connects the specified signal on the object obj to the function slot.
Returns an id that can later be used with `disconnect()`.

- **signal**
The name of the signal to be connected.
- **slot**
The javascript function to be executed when the signal is fired.

Example:

    var connectionId = deviceInfo.connect("orientationChanged", function (newOrientation) {
        console.log("Orientation changed to " + newOrientation);
    });

### disconnect(object obj, string signal, [function or id] slot) ###
Disconnects the specified signal on the object obj from a previously connected function.

- **signal**
The name of the signal to be disconnected.
- **slot**
An function previously connected, or an id previously returned, by `connect()`

Example:

    deviceInfo.disconnect("orientationChanged", connectionId)


### destroy() ###
Destroys the C++ QObject and cleans up any signals and slots connections that were made to/from the object. You should call this method whenever you want to free an object created by QtBridge. This is the equivalent of `calling deleteLater()` from C++.

## Global Methods ##

Global methods are available on the global object (Qt or blackberry.qt).

### initialize() ###
Loads the qt core shared libraries and starts the module thread. Calling this method more than once  will have no effect.
Returns true if the module was successfully loaded.

### terminate() ###
Unloads the qt core shared libraries, as well as any additional libraries that were loaded by this module and shuts down the module thread.
Returns true if the module was successfully unloaded.

### createObject(string class) ###
Creates an new qt object.

- **class**
The fully qualified name of the C++ class (separated by "." not "::"). Optionally, an API level may be specified by appending an "@" followed by the api level to the class string. By default, the highest API level will be selected.

Example:

    var deviceInfo = Qt.createObject("bb.device.DeviceInfo");
    
    var deviceInfo_v1 = Qt.createObject("bb.device.DeviceInfo@1.0");

### listNamespace(string namespace) ###
Returns a list of classes available in the specified namespace. This is useful for feature detection.

- **namespace**
A fully qualified namespace (separated by "." not "::")

Example:

    var classList = Qt.listNamespace("bb.device");

### connect(object obj, string signal, function slot) ###
Connects the specified signal on the object obj to the function slot.
Returns an id that can later be used with `disconnect()`.

- **obj**
An object created by `createObject()`.
- **signal**
The name of the signal to be connected.
- **slot**
The javascript function to be executed when the signal is fired.

Example:

    var connectionId = Qt.connect(deviceInfo, "orientationChanged", function (newOrientation) {
        console.log("Orientation changed to " + newOrientation);
    });

### disconnect(object obj, string signal, [function or id] slot) ###
Disconnects the specified signal on the object obj from a previously connected function.

- **obj**
An object created by `createObject()`.
- **signal**
The name of the signal to be disconnected.
- **slot**
An function previously connected, or an id previously returned, by `connect()`

Example:

    Qt.disconnect(deviceInfo, "orientationChanged", connectionId);


## Global Properties ##

Global properties are available on the global object (Qt or blackberry.qt).

### initialized ###
Returns true if the qt module is currently loaded and running.


# Custom Qt Classes #

QtBridge can use any QObject-based C++ classes that are registered with the qml type system. A complete sample class that demonstrates this feature can be found in the QtBridge native extension in the CustomModules folder with the name Module1.

### Creating a custom Class ###

To expose your custom C++ class to javascript through QtBridge, you will need to do the following:

- Inherit from `QObject`

		class Module1: public QObject

- Use the `Q_OBJECT` macro in your class definition

- Exposed methods must be a slot (marked with `Q_SLOT`/`Q_SLOTS`) or invokable (marked with `Q_INVOKABLE`)

		Q_SLOT void doSomething();
		Q_INVOKABLE QString doSomethingWithResult();

- Exposed properties must be declared with `Q_PROPERTY`

		Q_PROPERTY (QString constantProperty READ constantProperty CONSTANT)
		Q_PROPERTY (QString fullProperty READ fullProperty WRITE setFullProperty NOTIFY fullPropertyChanged)
		
		...
		
		public:
		    QString constantProperty() const;
		    QString fullProperty() const();
		    void setFullProperty(QString fullProperty);
		    
		Q_SIGNALS:
		    void fullPropertyChanged();

- Your class must be declared as a qml type with `QML_DECLARE_TYPE(fully::qualified::class)` **outside** any namespaces

		QML_DECLARE_TYPE(custom::Module1)

- Your class must be registered with  `qmlRegisterType<fully::qualified::class>("fully.qualified", 1, 0, "Class")` in the **registerCustomTypes** function in **CustomModules/CustomModules.cpp** to make QtBridge aware of your class

	    void registerCustomTypes() {
		   qmlRegisterType<custom::Module1>("custom", 1, 0, "Module1");
	    }


### Notes ###

Linux users: replace webworks with cordova in all of the above commands

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
