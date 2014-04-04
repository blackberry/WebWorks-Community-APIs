Cordova BarcodeScanner Plugin For Blackberry 10
===============================================

This plugin scans all types of barcodes supported by the [ZXing](https://github.com/blackberry/zxing) library. 
The Rear Camera is used and it paints a viewfinder onto a Canvas for the user to target the barcode.
It is a version of the [BarcodeScanner BB10 Webworks extension](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/BarcodeScanner) 
ported to Cordova. 

## Including the feature in your application

This API can be installed from source or from the [Cordova Plugin Registry](http://plugins.cordova.io/). Installation from the registry is done through the following:

	cordova plugin add com.blackberry.community.barcodescanner

or,
	
	webworks plugin add com.blackberry.community.barcodescanner

Installation from source is the same but instead of the id ("com.blackberry.community.barcodescanner"), use the file system path to the source plugin folder.

This feature depends on the blackberry.io which is installed with it. Permission for the camera is required, so add the following to your config.xml file.
```xml
<rim:permissions>
	<rim:permit>use_camera</rim:permit>
</rim:permissions>
```

It's also recommended that you stop scanning if the app goes into the background. To respond to app state,  include the blackberry.app feauture

When you start scanning, you can make the experience better for your users with the [Prevent Sleep Extension](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10-Cordova/PreventSleep). If you include Prevent Sleep in your app, the BarcodeScanner will automatically use it as needed to keep the screen on. Instructions for installing are in the Prevent Sleep [README](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10-Cordova/PreventSleep/README.md)

## Javascript API


```javascript
/* Start scanning for barcodes. 
 * @param succ
 *			Function that executes when the barcode has been recognized 
 *			successfully. Expected signature: succ(read_data), where 
 *			read_data is a string.
 * @param fail
 * 			Function that executes when the barcode has not been recognized 
 *			successfully. Expected signature: fail(error_message), 
 *			where error_message is a string.
 * @param args
 *			String denoting the ID of the Canvas where the viewfinder is 
 *			painted upon. In the sample it has a value of "myCanvas".
 */
 app.barcodeScanner.startRead(succ, fail, args)

 /*
 * Stop scanning for barcodes.
 * @param succ
 *			Function that executes when the barcode has been stopped
 *			successfully. Expected signature: succ(stopped_notification), where 
 *			stopped_notification is a string.
 * @param fail
 * 			Function that executes when the barcode could not be 
 *			successfully stopped. Expected signature: fail(error_message), 
 *			where error_message is a string.
 */
 app.barcodeScanner.stopRead(succ, fail)

```

## How to setup and run the sample

For Cordova Projects:

1. Create a new project using ```cordova create```.
2. Copy the _www_ folder in the _sample_ directory over the default one created by that command. 
3. Run ```__bbndk-env.bat``` or ```bbndk-env.sh``` script if the NDK is not on your PATH. 
4. Add the _blackberry10_ platform to your project by running ```cordova platform add blackberry10```.
5. Then add the BarcodeScanner plugin to the project using ```cordova plugin add /path/to/project/plugin```
6. Finally, execute ```cordova run```

For WebWorks 2.0 projects:

1. Create a new project using ```webworks create```.
2. Copy the _www_ folder in the _sample_ directory over the default one created by that command. 
5. Then add the BarcodeScanner plugin to the project using ```webworks plugin add /path/to/project/plugin```
6. Finally, execute ```webworks run```

Please take note that this plugin _does not_ work on a simulator.


## License

This plugin is Open Source licensed under the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0)

## Authors
[Jasmin Auger](https://github.com/FreakenK) and [Alex Tough](https://github.com/alextoughg)

Based on code written by [Alex Kwan](https://github.com/greentea0), [Mike Delong](https://github.com/mdelong) and [Tim Windsor](https://github.com/timwindsor)

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10-Cordova/BarcodeScanner?pixel)](https://github.com/igrigorik/ga-beacon)