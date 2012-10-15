Its a common place for various screen related extensions.

# Currently support

```
1. community.screen.setWallpaper(<file-path>) - Sets the wallpaper using an absolute file path.
```

# Using this extension
1. Open the NDK project using the BB10 NDK. https://developer.blackberry.com/native/beta/
2. Import the project located in the ndk-project folder
3. Compile the extension for the simulator and device configurations.
4. Copy the device (arm) compiled extension into extension/device
4. Copy the simulator (x86) compiled extension into extension/simulator
5. Copy the entire extension folder into \<WebWorks-SDK\>/Framework/ext
6. Package your app using the webworks SDK from the path \<WebWorks-SDK\> where the extension was copied.
7. In your app you should be able to use community.screen APIs

# Using the example
There is an example in the TestApp folder.

1. To use the sample please include a webworks.js file in the sample from the SDK/Framework/clientFiles folder.
2. Also change the script tag in the index.html folder to make it exactly same as the version webworks.js file copied in step1.

# Disclaimer
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Third Party software used
* JsonCpp v0.5 - Used for JSON parsing.
* JNEXT - Used for plugin interfacing with C/C++

Third party licenses are found in licenses.txt


# Comming Soon -
1. Currently working on a screenshot extension for taking the screenshot of the current screen and saving it to the file system with a path.
