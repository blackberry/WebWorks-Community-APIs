# PBKDF2 Plugin for BB10 Cordova / Webworks

This Cordova/WebWorks plugin allows an app to hash a password using [PBKDF2](http://en.wikipedia.org/wiki/PBKDF2) for storage in a 
FAST manner. PBKDF2 is a [Key Derivation Function](http://en.wikipedia.org/wiki/Key_derivation_function) used for [securing passwords](https://crackstation.net/hashing-security.htm). 
Although JavaScript implementations for PBKDF2 already exist, this implementation ran at around 100x faster JavaScript implementations tested.
Faster implementations means higher iteration counts can be used, leading to a more secure output. 
The [OpenSSL](http://www.openssl.org/) Crypto library has been used to perform the hashing. 

The sample code for this application is Open Source under the [Apache 2.0 License](http://www.apache.org/licenses/LICENSE-2.0.html).


**Author** 

* [Shikhir Singh](http://code.shikhir.com/)


**Release History**

* **V1.0.0** - Initial release

**How to install this extension**

In command prompt while in your Cordova/WebWorks project directory, type: 
```
cordova plugin add cordova-plugin-password-crypto
```

**Usage**

```
var passwdParam = {
	"password": "MyPassword",  
	"salt": "MySalt", // 
	"iterations": 50000, 
	"keyLength": "32" //  is bytes, not bits!
};
						
var hashedValue=community.PasswordCrypto.pbkdf2_Sync(passwdParam); 
```
**Iterations**
Iterations as well as the keylength determine the speed of the algorithm. You want the algorithm to be slow to be secure, but not slow
enough to irratate the user. A minimum iterations count of around 30,000 at a key length of 32 bytes is recommended. 

 
**Known Issues**

None! 

## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR 
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.