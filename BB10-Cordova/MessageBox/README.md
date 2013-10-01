Cordova MessageBox Plugin For Blackberry 10
===========================================

This plugin provides a standard API for the Dialog object documented in BB HTML5 Webworks, 
in addition to an extension created for password prompts.

## How to setup the plugin and run the sample

1. Install Apache Cordova and Webworks. 
2. Clone the contents of the Webworks-Community-APIs repo into a directory of your choice.
3. Navigate to BB10-Cordova/MessageBox/sample.
4. To run the sample on your phone or simulator, type into the console:
```
cordova run
```
5. Wait for a few seconds, until ```cordova run``` returns. You now have the 
sample running on your phone/simulator!

## Javascript API


```javascript
/*
 * Displays an alert message defined according to options. 
 * options is an object that contains one or more of the 
 * following fields:
 *	- okButtonTitle: Default value 'OK'.
 *  - title : A string describing the title of the alert box.
 *  - message : A string with the contents of the confirm box.
 * callback is a function that gets called as soon as the 
 * native code has done its work. Expected signature:
 * callback(button), where button is a string that describes 
 * the selected option. Default value for button is 'ok'.
 */
 window.plugins.messageBox.alert(options, callback)

 /*
 * Displays a confirm message defined according to options. 
 * options is an object that contains one or more of the 
 * following fields:
 *  - yesButtonTitle: Default value 'Yes'.
 *  - noButtonTitle: Default value 'No'.
 *  - title : A string describing the title of the confirm box.
 *  - message : A string with the contents of the confirm box.
 * callback is a function that gets called as soon as the 
 * native code has done its work. Expected signature: 
 * callback(button), where button is a string that describes 
 * the selected option. Possible values for button are 'yes' or 'no'.
 */
 window.plugins.messageBox.confirm(options, callback)


/*
 * Displays a password prompt defined according to options. 
 * options is an object that contains one or more of the 
 * following fields:
 *	- okButtonTitle: Default value 'OK'.
 *  - cancelButtonTitle: Default value 'Cancel'.
 *  - title : A string describing the title of the prompt box.
 *  - message : A string with the contents of the prompt box.
 * callback is a function that gets called as soon as the 
 * native code has done its work. Expected signature: 
 * callback(button, value), where button is a string that describes 
 * the selected option (possible values: 'ok' or 'cancel') and value is a string 
 * containing the user's input. 
 */
 window.plugins.messageBox.prompt(options, callback)

```

## License

Copyright 2013 [Jasmin Auger](https://github.com/FreakenK) and [Alex Tough](https://github.com/alextoughg)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.