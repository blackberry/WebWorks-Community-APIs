Cordova BarcodeScanner Plugin For Blackberry 10
===============================================

This plugin scans all types of barcodes supported by the [ZXing](https://github.com/blackberry/zxing) library. 
The Rear Camera is used and it paints a viewfinder onto a Canvas for the user to target the barcode.
It is a version of the [BarcodeScanner BB10 Webworks extension](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/BarcodeScanner) 
ported to Cordova. 

## How to setup the plugin and run the sample

1. Install Apache Cordova and Webworks. 
2. Clone the contents of the Webworks-Community-APIs repo into a directory of your choice.
3. Navigate to BB10-Cordova/BarcodeScanner/sample.
4. Add the com.blackberry.io plugin found in B10-Cordova/BarcodeScanner/ext:
```
cordova plugin add ../ext/blackberry.io 
```
Notice that this is only necessary to run the sample, not the actual plugin. 
5. Set Cordova's target device to ``` phone ```. 
6. Execute the ```run``` script: 
``` ./run ``` 
7. Wait for a few seconds. You now have the example running on your phone/simulator. 

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

## Including the feature in your application
This feature depends on the blackberry.io feature as well, so both must be included in your applications config.xml file. Also, permission for the camera is required.
```xml
<feature id="blackberry.io" />
<feature id="community.barcodescanner" />

<rim:permissions>
	<rim:permit>use_camera</rim:permit>
</rim:permissions>
```

It's also recommended that you stop scanning if the app goes into the background. To respond to app state, include the following feature:
```xml
<feature id="blackberry.app" />
```

When you start scanning, you can make the experience better for your users with the [Prevent Sleep Extension](https://github.com/blackberry/WebWorks-Community-APIs/tree/master/BB10/PreventSleep). If you include Prevent Sleep in your app, the BarcodeScanner will automatically use it as needed to keep the screen on. Instructions for installing are in the Prevent Sleep [README](https://github.com/blackberry/WebWorks-Community-APIs/blob/master/BB10/PreventSleep/README.md)

```xml
<feature id="community.preventsleep" />
```


## License

Copyright 2013 Jasmin Auger and Alex Tough

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

"Including the feature in your application" of this README is copyright Tim Windsor, Alex Kwan and Mike Delong. 

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.