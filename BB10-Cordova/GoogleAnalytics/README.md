Google Analytics Plugin for BB10 Cordova
==========================================

This plugin offers a JavaScript API for Google Analytics: http://www.google.com/analytics/

## Brief Intro for Google Analytics
Google Analytics (GA) provides a free web and mobile tracking service. All tracking is done by sending HTTP POST request to GA, with the required parameters as payload of the request. Tracking statistics can be seen from the dashboard on GA website. You need to sign up for an account for GA. You can use a single account for tracking multiple apps or multiple versions of an app. Your GA account number (UA-xxxxx-x) is a required parameter for sending the tracking request.

One issue with using GA is that it is difficult to debug issues with HTTP requests. The GA server always returns OK status with no useful info in status or request text, regardless of whether the HTTP requests are formatted correctly or not. Add to that, GA only updates the full tracking statistics once a day. There is a *Real-time* tab in GA tracking dashboard, but it only provides limited info.

## Installing the Plugin From Source
- Copy the whole plugin directory to your local file system
- Open a new or existing project from WebWorks (2.0 or above)
- In the *Plugins* option, enter the full file system path to where `plugin.xml` is
- To try the sample app included, copy the `sample\www` directory to the `\www` directory of a new WebWorks project

## Using the GA Plugin
### Properties
You will need to set these properties before proceeding with any tracking.
```
community.googleanalyticsplugin.appName
```
Set/get the name of the app to be tracked. If not set, `Default_AppName` will be used.

```
community.googleanalyticsplugin.uuid
```
Set/get an unique ID for per-user tracking. If not set, a default number will be used.
Setting `community.googleanalyticsplugin.uuid=""` generates a random ID which you can use for your app. Use web-storage to save the ID for per user/device tracking.
GA documentation says the ID needs to be in UUID format, but it appears any number or text will work. For debugging purpose, using a random ID for each HTTP request can be helpful since each request will be considered as an unique user, which will show up as unique user count in GA dashboard limited *Real-time* statistics.

```
community.googleanalyticsplugin.gaAccount
```
Set/get the GA account number in the format UA-xxxxxxxx-x. It must be set with no default value.
```
community.googleanalyticsplugin.lastPayload
```
Get-only property. It returns the last payload of the HTTP POST request being sent to GA.


### Tracking Functions
Calling these functions will send a HTTP POST request to GA, with the sepecified parameters. The function parameters in square-brackets, e.g. `[pageTitle]` are optional. Unless otherwise stated, most parameters take strings as input. If error occurs, the tracking functions return an error message. Otherwise an empty string is returned.
```
community.googleanalyticsplugin.trackPageview(pageURL, [pageTitle], [hostName])
```
Used for tracking page-views.
```
community.googleanalyticsplugin.trackEvent(eventCategory, eventAction, [eventLabel], [eventValue])
```
Used for tracking events. *Actions* are grouped under *Categories* in GA dashboard. There is no pre-defined actions or categories in GA, you can define any you need.
```
community.googleanalyticsplugin.trackTransaction(tID, [tAffil], [tRevenue], [tShipn], [tTax], [tCurr])
```
Used for tracking eCommerce transaction. **You need to enable eCommerce tracking in GA dashboard**, otherwise all transaction tracking will simply be discarded without any warning message. `tRevenue`, `tShipn` and `tTax` require currency input, i.e. non-negative number. `tAffil` is used for company or store name.
```
community.googleanalyticsplugin.trackItem(tID, iName, [iPrice], [iQuant])
```
Used for tracking eCommerce item hits. **You need to enable eCommerce tracking in GA dashboard**, otherwise all item hit tracking will simply be discarded without any warning message. `iPrice` requires currency input, and `iQuant` requires non-negative integer. An item hit tracking must be assosicated with a transaction ID `tID`.

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

