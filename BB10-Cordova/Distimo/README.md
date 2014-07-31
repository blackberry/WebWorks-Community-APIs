Cordova Distimo Analytics Plugin For Blackberry 10 (incomplete)
===============================================================

Important Note: the source file of the plugin is hosted [here](https://github.com/blackberry/DistimoSDK-PhoneGap-Plugin).

## Overview

This plugin was written based on the Android and iOS versions of the [Distimo SDK](https://github.com/distimo/DistimoSDK-PhoneGap-Plugin/tree/master/src).

There are four main gears that facilitate the Distimo SDK in general, unless the Windows Phone version is substantially different:

1. Storage Manager
2. Event Manager
3. Uncaught Exception Handler
4. AppLink Manager

In a nutshell, an event (e.g. a purchase) received from a user of the plugin (i.e. the developer of the app) is processed through some validation and then passed into Event Manager and Storage Manager, as necessary.

Event Manager is responsible for sending the event to the Distimo server so that the user can later go see how the app analytics (e.g. how items are selling). Typically, events are only sent when app is running in foreground.

Storage Manager is a wrapper to the device's local database. Event Manager will use this to save events that are not yet sent; events may not be sent because they are still in the queue waiting to be sent, or simply because app is running in background.

Uncaught Exception Handler is responsible for reporting app crashes. Specifically, it saves the stack trace of the crash in the local database (using the Storage Manager) and sends the stack trace (by the Event Manager) when the app is relaunched.

AppLink Manager is responsible for handling operations related to AppLink, one of the key features that Distimo provides (More information required here).


## Notes and Tasks

In addition to the above, I have also implemented Debug Logger, which helps you print out values of some variables or structures, for example, data stored in the local storage. 

There are four tasks that needs potential work:

1. Try actually sending events to Distimo server: Plase see the implementation of the Event Manager. All the required bits and pieces are there, but they have never been actually tested yet due some barriers*. Though I don't think it will be a big problem, as the implementation is very straightforward. You will need a Distimo account and a Blackberry Vendor account.

2. Uncaught Exception Handler: First, you will have to determine if it is possible to capture a stack trace or something similar when app crash occurs. If it is, start implementing the handler by looking at the implementations in other platforms. If not, please discuss with Tim what alternative options are available.

3. AppLink Manager: I have not had a chance to fully understand what AppLink Manager does, but I'm pretty sure you will need a Distimo account and maybe a released app.

4. Other BB10 specific functions: If you look at distimosdk.js, you will notice that some of the functions are specific to some platforms. There will probably be BB10 specific functions that needs to be implemented, as well. Please discuss with Tim for this.

5. Merge files: When all of the above tasks are done, you will have to merge what's in our client.js and plugin.xml into the [corresponding files](https://github.com/distimo/DistimoSDK-PhoneGap-Plugin).

(* It looked like I had to actually release my test app to the store in order to test sending; otherwise Distimo would not recognize my app. It was already near end of the term by the time I was ready to test this, and my test app was not approved until the point I am writing this.)


## License

This plugin is Open Source licensed under the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0)


## Authors

[Byunghoon Yoon](https://github.com/bh2yoon)


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
