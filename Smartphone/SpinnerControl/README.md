# Spinner Control
The spinner control will allow you to have a more usable interface when interacting
with &lt;select&gt; elements on a BlackBerry 5.0 browser.  It will also allow you to keep 
the same consistent feel with your UI on a BlackBerry 6.0 browser.

**Author:** [Tim Neil](https://github.com/tneil)

![Spinner Image](https://github.com/blackberry/WebWorks-Community-APIs/raw/master/Smartphone/SpinnerControl/screenShot.png)

## Tested On

* BlackBerry Bold 9700 v5.0.0.469
* BlackBerry Bold 9700 v5.0.0.545
* BlackBerry Curve 8900 v5.0.0.411
* BlackBerry Pearl 3G 9100 v5.0.0.604
* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Storm 9520 v5.0.0.428
* BlackBerry Storm 9520 v5.0.0.436
* BlackBerry Tour 9630 v5.0.0.419
* BlackBerry Torch 9800 v6.0.0.190
* BlackBerry Torch 9800 v6.0.0.246
* BlackBerry Style 9670 v6.0.0.309

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/tneil)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**blackberry.ui.Spinner**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\SpinnerControl**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\SpinnerControl**_ directory to your new _**ext\blackberry.ui.Spinner**_ directory

6. Copy the downloaded _**Smartphone\SpinnerControl\src\blackberry**_ directory to your new _**ext\blackberry.ui.Spinner\blackberry**_ directory

**NOTE:** Be sure to back-up this _**ext\blackberry.ui.Spinner**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.ui.Spinner" />

## Summary

    static void open(options : OptionsInterface, callback : function (selectedIndex : Number) )

    OptionsInterface:
    readwrite  property  String[]   items
    readwrite  property  Number     rowHeight
    readwrite  property  String     selectedIndex
    readwrite  property  String     title
    readwrite  property  Number     visibleRows

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
When creating a spinner control you will pass it an options object that you can
create out of simple JSON.  This will provide the spinner with all the information it 
needs to display properly.

You will also pass in an event callback that will be triggered once the user has 
selected an item or cancelled the screen.  If the user does not select an item
the selectedIndex returned as a parameter in the callback will be undefined.

_**NOTE:**_ The callback is handled asynchronously, so code that is placed directly after
the "open" function call will be executed immediately while waiting for the user's 
response input.

## Properties
**items:**
Is an array of string items you wish to display in the spinner control

**rowHeight:**
Is the height of each of the rows that you wish to display in the spinner

**selectedIndex:**
Is the index of the item you wish to be displayed as the default selected 
item when the spinner is shown

**title:**
Is the title caption you wish to have at the top of the spinner

**visibleRows:**
Is the number of rows you wish to have visible in the spinner control

## Change Log
_Empty_
