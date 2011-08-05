# Screen Object
The screen object will allow you to subscribe to scrolling events on the screen, set the screen's title and show/hide the virtual keyboard.  

**Author:** [Tim Neil](https://github.com/tneil)

## Tested On

* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Torch 9800 v6.0.0.190
* BlackBerry Torch 9800 v6.0.0.246

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

## Known Issues
* Currently the setTitle() function has painting problems and isn't very functional

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/tneil)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.ui.screen**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\Screen**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\Screen**_ directory to your new _**ext\blackberry.ui.screen**_ directory

6. Copy the downloaded _**Smartphone\Screen\src\blackberry**_ directory to your new _**ext\blackberry.ui.screen\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.ui.screen**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.ui.screen" />

## Summary

    static void onScrollStart(callback : function() )
    static void onScrollEnd(callback : function(verticalPosition : Number, horizontalPosition : Number) )
	static void showKeyboard()
	static void hideKeyboard()
	static void setTitle(title : String)

## Code Example

    function foo()
    {
      blackberry.ui.screen.onScrollStart( function() {
        alert('starting to scroll');
      });

      blackberry.ui.screen.onScrollEnd(function(vPos, hPos) {
        alert('Vertical Position: ' + vPos + ' Horizontal Position: ' + hPos);
      });
    }


