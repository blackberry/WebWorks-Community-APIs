Message Plugin For Blackberry 10
===============================================

Plugin for retrieving a message body knowing account ID and message ID. These information comes to the application with the invocation data.

Example : getting a body of the email message, action invoked by SHARE from the hub.

```javascript
var json = base64.decode(invokedInfo.data);
if (community && community.messageplugin) {
    json=eval("(" + json + ')');
    json=community.messageplugin.getEmailMessage(json.attributes.accountid+" "+json.attributes.messageid);
    console.log(json);
    json=eval("(" + json + ')');
    console.log(json.body);
}
```

#DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

