# QtBridge #
QtBridge is a WebWorks 2.0/Cordova native extension that provides a dynamic bridge between Qt and JavaScript allowing you to leverage Qt APIs directly from javascript, including the use of Qt signals and slots. 

This folder contains the native extension, which is a momentics project that behaves similarly to a Cascades project. The result of this project is a WebWorks 2.0/Cordova extension for BlackBerry10 that cannot be used on its own. See SampleApp in the QtBridge root folder for instructions on creating a WebWorks 2.0/Cordova app with this extension.

**Applies To**

* [BlackBerry 10 Native SDK](https://developer.blackberry.com/native/download/sdk)

**Author**

* [Isaac Gordezky](http://www.twitter.com/igordezky)

# Custom Qt Classes #

QtBridge can use any QObject-based C++ classes that are registered with the qml type system. A complete sample class that demonstrates this feature can be found in the QtBridge in the CustomModules folder with the name Module1.

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

- Exposed enumerations must be marked with `Q_ENUMS` and declared as a qml type (see below)

		Q_ENUMS(EnumType)
			    
	    public:
	        enum EnumType {
	            EnumType1 = 1,
	            EnumType2 = 2,
	            EnumType3 = 3,
	            EnumTypeX = 100
	        };

- Your **class**, and any **enumerations** that it uses, must be declared as a qml type with `QML_DECLARE_TYPE(fully::qualified::class)` **outside** any namespaces

	    QML_DECLARE_TYPE(custom::Module1)
	    QML_DECLARE_TYPE(custom::Module1::EventType)


- Your **class** must be registered with  `qmlRegisterType<fully::qualified::class>("fully.qualified", 1, 0, "Class")` in the **registerCustomTypes** function in CustomModules/CustomModules.cpp to make QtBridge aware of your class

	    void registerCustomTypes() {
		   qmlRegisterType<custom::Module1>("custom", 1, 0, "Module1");
	    }


### Custom Class Example ###
This class is a usable example of what you can expose directly to javascript. Use this as a starting point to create your own custom classes!

	#include <QtDeclarative>
	#include <QObject>
	
	namespace custom {
	    class Module1: public QObject {
	        Q_OBJECT
	        Q_ENUMS(EnumType)
	
	        Q_PROPERTY (QString constantProperty READ constantProperty CONSTANT)
	        Q_PROPERTY (QString fullProperty READ fullProperty WRITE setFullProperty NOTIFY fullPropertyChanged)
	    
	    public:
	        enum EnumType {
	            EnumType1 = 1,
	            EnumType2 = 2,
	            EnumType3 = 3,
	            EnumTypeX = 100
	        };
	        Module1(QObject *parent = NULL)
	           : QObject(parent),
	             mConstantProperty("constantPropertyValue"),
	             mFullProperty("mFullPropertyValue") {}
	        virtual ~Module1() {}
	
	        Q_SLOT void doSomething() {}
	        Q_INVOKABLE QString doSomethingWithResult() {
	            return "done";
	        }
	
	        QString constantProperty() const {
	            return mConstantProperty;
	        }
	        QString fullProperty() const {
	            return mFullProperty;
	        }
	        void setFullProperty(QString fullProperty) {
	            mFullProperty = fullProperty;
	            Q_EMIT fullPropertyChanged();
	        }
	
	    Q_SIGNALS:
	        void fullPropertyChanged();
	        void enumChanged(custom::Module1::EnumType enumType);
	
	    private:
	        QString mConstantProperty, mFullProperty;
	    };
	}
	
	QML_DECLARE_TYPE(custom::Module1)
	QML_DECLARE_TYPE(custom::Module1::EnumType)



# How To Build #

1.  Clone this repo to your local machine

2.  Ensure the [BlackBerry 10 **Native** SDK](https://developer.blackberry.com/native/download/sdk) is correctly installed

3.  Import the native extension project into momentics. It is located at `/plugin/src/blackberry10/native` within **this project**

4.  Build the native extension project for all architectures: Right click on the project and choose "Build Configurations" > "Build All".


## Internals ##

This section offers a brief overview of how QtBridge works internally

* **module.cpp**
contains the JNext plugin api, used by JNext to load this plugin. It is equivalent to the `plugin.h` api provided by the JNext plugin framework. It is responsible for managing the plugin lifecycle and string buffer management.

* **QtBridge class**
is responsible for lifecycle management of the QtBridge thread and passing messages to and from the `QJnextMainLoop` class within that thread. This class does not modify messages as it passes them.

* **QJnextMainLoop class**
is the main class in the QtBridge thread. It is responsible for creating and destroying QObject-based class instances, signal connect/disconnect, managing signal wrappers, serializing/de-serializing parameters and calling methods and property getters/setters.

* **ApplicationThread class**
is a QThread class that has additional lifecycle management normally handled by the `bb::Application` class.

* **CustomModules.hpp/cpp**
is an interface for 3rd party extension classes to register themselves with the qt type system. This allows you to easily develop custom extension classes without creating a qml import module.

* **QtDeclarativePrivate**
contains *INTERNAL* headers from Qt 4.8 which are used to perform qml module loading and type introspection.

## More Info

* [QObject propertis, signals and slots](http://developer.blackberry.com/native/documentation/cascades/dev/qt_fundamentals/index.html#qobjecttheqtbaseclass) - documentation on the blackberry developer microsite
* [QObject reference documentation](http://qt-project.org/doc/qt-4.8/qobject.html) - documentation at qt-project.org
* [BlackBerry Native API Reference](http://developer.blackberry.com/native/reference/cascades/)
* [BlackBerry Native Community Forums](http://supportforums.blackberry.com/t5/Native-Development/bd-p/Cascades)

## Contributing Changes

Please see the [README](https://github.com/blackberry/WebWorks-Community-APIs) of the Webworks-Community-APIs repository for instructions on how to add new Samples or make modifications to existing Samples.

## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/BB10-WebWorks-Samples/issues) for the Sample.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
