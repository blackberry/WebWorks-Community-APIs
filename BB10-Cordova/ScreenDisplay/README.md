Screen Display
==============

This BlackBerry 10 WebWorks extension provides additional APIs supplying information about the device screen display.

The API returns the information about the screen directly from the device. The returned values are the physical metrics of the screen without the browser applying any scaling etc.

The single most valuable figure returned from this API is ppi. The ppi value allows you to make the text and images in your app look the same size on any device by adjusting the font-size and image widths / heights based on the ppi.

A simple example is that you design for a 36px font on a device with 356 ppi (a Z10). If you get a 600 ppi device all you have to do is alter the font-sizes using newFontSize = Math.round((device.ppi / 356) * 36) giving a 61px font size on a 600 ppi device. The same holds true for lower ppi devices - a Q10 would end up with a 34px font.

** Tested On **

BlackBerry 10 Dev Alpha C 10.2.1.1927

** Author **

[Peardox] (http://supportforums.blackberry.com/t5/user/viewprofilepage/user-id/325249)
[Alexandre Huot] (https://github.com/alexhuot1)

## Building and Testing the Sample

1. Deploy the extension into your BB10 WebWorks SDK by copying the community.screendisplay folder from the ext folder to the Framework/ext folder in your WebWorks SDK, or run the Ant build script in the /ext folder to do so.
2. Package the sample in the sdtest folder. Use the [Ant Build Script](https://github.com/blackberry/BB10-WebWorks-Community-Samples/tree/master/Ant-Build-Script), or you can do this with the WebWorks packager command:
	__bbwp__ sdtest -g _signingpassword_
3. Load it on your phone using the Ant Build Script, or with the _blackberry-deploy_ command found in the dependencies/tools folder of your WebWorks SDK.
4. When the application runs, an informational page will be displayed as in the screenshots below.

![Screenshot](sshotz10.png) &nbsp;
![Screenshot](sshotdac.png)

## Including the feature in your application
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.
```xml
<feature id="community.screendisplay" required="true" version="1.0.0.0" />
```

## The Extension API
The Screen Display Extension provides the following API:

```javascript
var result = community.screendisplay.sdgetsize();

/*
Result is a javascript object with the following properties

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
*/
```

## Notes

The first four numbers are returned from the device. The other numbers are calculated from these four base readings.

The device may return numbers indicating non-square pixels if either the physical sizes are not accurate or the device truly has non-square pixels.

The ppmm and ppi values are calculated from the screen diagonal to get one easy number

The pixelShape (aspect ratio) figure is based calculated figures and so may be slightly off as well.

pixelShape = 1 means square, 2 means twice as wide as high, 0.5 means twice as wide as high (it is doubtful you'll ever see anything far from 1)

## Building the extension from source

Copy the extension folder to a location on your computer to start working with it.

### Momentics NDK setup

You can either import the project from the Template folder, or use the New Project Wizard in Momentics to create a starter project.

#### Importing the Template

1. Open the Momentics IDE. Navigate to the workbench and from the program menu
select File -> Import and choose "Existing Projects into Workspace".
2. Choose "Select root directory: " and browse to the _/plugin/src/blackberry10/native_ directory where you copied the plugin. Select the PreventSleep project in the Projects list and uncheck "Copy projects into workspace". Click Finish.
3. Follow these next steps to build the plugin to be sure the setup is working.

#### How to build your native Plugin

1. Right click your project and select the Clean Project option.
2. Right click your project again and select Build Configurations -> Build Selected... .
3. A window will appear that shows all the available build configurations
for the project. Select __device__ and __simulator__ and click ok.
4. You should see the shared libraries (libScreenDisplay.so files) generated in the folders for each Build Configuration that you selected.

### Using the Plugin in an Application

To use the plugin in another project, that's been created with Cordova, run __cordova plugin add <path to the PreventSleep folder>/plugin__.
That will copy the plugin into the project, and update the www/config.xml file to include the feature as below:

```xml
<feature name="community.screendisplay" value="community.screendisplay" />
```

All the methods in the extension will be prefixed by that feature id, so a method called sdgetsize() supplied in the community.screendisplay extension will be called in JavaScript like so:

```javascript
community.screendisplay.sdgetsize();
```

### Modifying the Extension

See the examples in the [Template Extension](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/Template) for how to add additional features to this extension

#### Rebuild 
When making changes, rebuild regularly so you don't make a really hard to find typo.

Follow the steps above to:
1. [Build the native portion](#how-to-build-your-native-extension),
2. [Copy the extension to your SDK](#copying-the-extension-to-the-sdk), and
3. [Use the extension in your test app](#using-the-extension-in-an-application).


**To contribute code to this repository you must be [signed up as an official contributor](http://blackberry.github.com/howToContribute.html).**

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/ScreenDisplay?pixel)](https://github.com/igrigorik/ga-beacon)