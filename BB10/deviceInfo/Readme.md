# community.deviceInfo Native Extension for applications built using BlackBerry 10 WebWorks

This extension provides additional APIs to identify the device.

**Applies To**

* [BlackBerry 10 WebWorks SDK](https://developer.blackberry.com/html5/download/sdk) 

**Author(s)** 

* Anzor Bashkhaz (http://www.twitter.com/anzor_b)

## Set up

Copy the ext\community.deviceInfo folder into the Framework\ext folder of the WebWorks SDK

## Use

<pre>
//returns model number i.e. 'Z10'
community.deviceInfo.getModelNumber()
</pre> 

<pre>
//returns whether the device is a Simulator - 'true' or 'false'
community.deviceInfo.isSimulator()
</pre> 

<pre>
//returns Roaming Status as 'true' or 'false'
community.deviceInfo.getRoamingStatus()
</pre> 


The following APIs require the following permission:
<b>read_cellular_data</b>

<pre>
//returns Mobile Country Code
community.deviceInfo.getMCC()
//returns Mobile Network Code, that identifies the carrier
community.deviceInfo.getMNC()
</pre>	

## Contributing Changes

Please see the [README](https://github.com/blackberry/WebWorks-Community-APIs) of the WebWorks-Community-APIs repository for instructions on how to add new Samples or make modifications to existing Samples.


## Bug Reporting and Feature Requests

If you find a bug in a Sample, or have an enhancement request, simply file an [Issue](https://github.com/blackberry/WebWorks-Community-APIs//issues) for the Sample and send a message (via github messages) to the Sample Author(s) to let them know that you have filed an [Issue](https://github.com/blackberry/WebWorks-Community-APIs//issues).

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.