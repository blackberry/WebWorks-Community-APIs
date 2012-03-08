# blackberry.clipboard

This extension provides clipboard read/write access to blackberry WebWorks applications.

**Authors:** [Martin Kleinschrodt](https://github.com/MaKleSoft)

## Tested On

* BlackBerry Tablet OS v 2.0.0

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/MaKleSoft)

## How to install this extension

1. Download the source from this repository and extract it to a location on your computer.

2. Using File Explorer browse to the downloaded source code for this extension: _**Tablet\Clipboard**_.

3. Copy the downloaded _**Tablet\Clipboard\blackberry.clipboard**_ directory to the extensions directory for the BlackBerry WebWorks SDK for Tablet OS. The default path for this location is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK for TabletOS\bbwp\ext**_.

**NOTE:** Be sure to back-up this _**ext\blackberry.clipboard**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.clipboard" />

## API
Write text to clipboard:
	blackberry.clipboard.setText("some text");

Get text from clipboard:
	var text = blackberry.clipboard.getText();