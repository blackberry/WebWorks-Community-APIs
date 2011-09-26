# Spinner Control
The spinner control will allow you to have a more usable interface when interacting
with &lt;select&gt; elements on a BlackBerry 5.0 browser.  It will also allow you to keep 
the same consistent feel with your UI on a BlackBerry 6.0 browser.

**Author:** [Jerome Carty](https://github.com/jcarty)

## Tested On

* BlackBerry Bold 9900 v7.0.0.353
* BlackBerry Bold 9650 v6.0.0.546
* BlackBerry Curve 9300 v5.0.0.716

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/jcarty)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.ui.Spinner**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\FileUploader**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\FileUploader**_ directory to your new _**ext\webworks.io.FileUploader**_ directory

6. Copy the downloaded _**Smartphone\FileUploader\src\blackberry**_ directory to your new _**ext\webworks.io.FileUploader\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.io.FileUploader**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.io.FileUploader" />

## Summary


## Code Example

    function foo()
    {
        var rowHeight;
        var visibleRows;

        // Populate our items
        var items = new Array('Barcelona', 'Beijing', 'Brasilia', 'Melbourne', 'Moscow', 'New York', 'Paris' );

        // Figure out our height and rows based on screen size
        if (screen.height < 480){
          rowHeight = 60;
          visibleRows = 3;
        }
        else {
          rowHeight = 75;
          visibleRows = 4;
        }

        // Configure the options 
        var options = {'title': 'Choose A City:',
          'rowHeight': rowHeight,
          'visibleRows': visibleRows,
          'selectedIndex': 2,
          'items' : items};

        // Open the spin dialog
            blackberry.ui.Spinner.open(options, function (selectedIndex) {
              alert(selectedIndex); }	  
        );	
    }

## Usage Information


_**NOTE:**_ The callback is handled asynchronously, so code that is placed directly after
the "open" function call will be executed immediately while waiting for the user's 
response input.

## Properties
**items:**
Is an array of string items you wish to display in the spinner control

## Change Log
_Empty_