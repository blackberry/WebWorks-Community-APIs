Google Analytics Plugin for BB10 Cordova
==========================================

This plugin offers a JavaScript API for [Google Analytics](http://www.google.com/analytics/) "Measurement Protocols", providing 4 different types of tracking requests to Google Analytics. It has the options to generate UUID for identifying individual mobile devices, and to store all tracking requests on-the-run so no data will be lost even when the connection is lost or timed-out. It will also retrieve and send remaining un-sent data from previous session, if any exists.

## Brief Intro to Google Analytics
Google Analytics (GA) provides a free web and mobile tracking service. All tracking is done by sending HTTP POST request to GA, with the required parameters as payload of the request. Tracking statistics can be seen from the dashboard on GA website. You need to sign up for an account for GA. You can use a single account for tracking multiple apps or multiple versions of an app. Your GA account number (UA-xxxxx-x) is a required parameter for sending all tracking requests.

One difficulty with using GA is to debug issues with the HTTP requests. The GA server always returns OK status with no useful info in the status or request text, regardless of whether the HTTP requests are formatted correctly or not. In addition, GA only updates the full tracking statistics once a day. There is a *Real-time* tab in GA tracking dashboard, but it only provides limited info.

## Installing the Plugin From Source
- Copy the whole plugin directory to your local file system
- Open a new or existing project from WebWorks (2.0 or above)
- In the *Plugins* option, enter the full file system path to where `plugin.xml` is
- To try the sample app included, copy the `sample\www` directory to the `\www` directory of a new WebWorks project

## Using the GA Plugin
**Make sure you sign up for an account with Google Analytics, then use your account number (UA-xxxxxxxx-x) in place of the default in the sample app.**

### Init & Properties
To start the Google Analytics plugin, call:
```
community.googleanalyticsplugin.initializeGA(gaAccount, appName, [UUID], [useQueue]);
```
- `gaAccount` -- your registered Google Analytics account number in the format UA-xxxxxxxx-x
- `appName` -- name of the app
- `UUID` -- (optional) an unique ID for the mobile device. If not explicitly set, UUID from previous session will be used if exists. Otherwise a random UUID will be generated and stored. UUID format used is [HERE](http://en.wikipedia.org/wiki/Universally_unique_identifier#Version_4_.28random.29).
- `useQueue` -- (optional, "true"/"false") If enabled, will queue up and save all tracking data until successfully sent. Un-sent data from previous session will also be retrieved to be re-sent. If disabled, will only attempt to send every tracking request once, and no tracking data will be queued or saved.

You can get/set individual properties if you want/need to:
- `community.googleanalyticsplugin.appName;`
- `community.googleanalyticsplugin.uuid;`
- `community.googleanalyticsplugin.gaAccount;`
- `community.googleanalyticsplugin.lastPayload;` - get only property; return the last payload data being sent/queued
- `community.googleanalyticsplugin.randomUuid;` - "true" or "false"; if enabled, will use random UUID for every single tracking, good for real-time debugging purpose. Note that the random UUIDs generated with this feature is not saved. The original UUID from init or set property will remain.


### Tracking Functions
Calling these functions will send a HTTP POST request to GA, with the sepecified parameters. The function parameters in square-brackets, e.g. `[pageTitle]` are optional. Unless otherwise stated, most parameters take strings as input. If error occurs, the tracking functions return an error message. Otherwise an empty string is returned.

- Tracking pageviews:
```
community.googleanalyticsplugin.trackPageview(pageURL, [pageTitle], [hostName])
```
- Tracking events: *Actions* are grouped under *Categories* in GA dashboard. There is no pre-defined actions or categories in GA, you can define any you need.

```
community.googleanalyticsplugin.trackEvent(eventCategory, eventAction, [eventLabel], [eventValue])
```
- Tracking eCommerce transaction: **You need to enable eCommerce tracking in GA dashboard**, otherwise all transaction tracking will simply be discarded without any warning message. `tRevenue`, `tShipn` and `tTax` require currency input, i.e. non-negative number. `tAffil` is used for company or store name.
```
community.googleanalyticsplugin.trackTransaction(tID, [tAffil], [tRevenue], [tShipn], [tTax], [tCurr])
```
- Tracking eCommerce item hits. **You need to enable eCommerce tracking in GA dashboard**, otherwise all item hit tracking will simply be discarded without any warning message. `iPrice` requires currency input, and `iQuant` requires non-negative integer. An item hit tracking must be assosicated with a transaction ID `tID`.
```
community.googleanalyticsplugin.trackItem(tID, iName, [iPrice], [iQuant])
```


### Tips
- For debugging purpose, using a random ID for each HTTP request can be helpful since each request will be considered as an unique user. In turn, that will show up as unique user count in GA dashboard limited *Real-time* statistics. To do this, set `community.googleanalyticsplugin.randomUuid = "true";`. This will use a random UUID for every single tracking request.
- For app that needs tracking of numerous "events", e.g. clicking/taping actions in gaming app, it would be better to disable queueing of tracking payload data, as queueing requires disk storage access, which may affect performance. Also, if the network is down, then a very large amount of data will be stored with queue enabled.
- For app that needs a small number of trackings and all tracking requests to be collected, enabling queueing will be useful. Queueing ensures all data is sent to GA. Data will be stored and re-sent later if the connection is down or time-out occurs.
- With queueing enabled, `Network busy` error may appear when multiple tracking requests are made within a very short time even with active network connectivity, as the plugin can only send one request at a time. Thus it is not a true error and the tracking data is being queued and sent. If the network is down or time-out occurs, this error will also appear on all subsequent tracking requests when there is at least one request already queued up waiting to be sent. 


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

