# webworks.template

This template is designed to help developers get started writing their own WebWorks extensions. Download and modify 
the source and making changes throughout wherever you see a "STEP n" instruction.

**Authors:** [Adam Stanley](https://github.com/astanley)

## Tested On

* BlackBerry Bold 9900 (OS 7.0.0.569)
* BlackBerry Bold 9700 (OS 5.0.0.680)


## TODO
1. Demonstrate how to support callbacks to JavaScript.

## Troubleshooting
If you encounter RAPC errors when building your WebWorks app, it likely means there is a syntax or compiling error 
within your extension code.

How to test?  Suggest building a sample WebWorks app, similar to test_app, that uses JavaScript unit tests to evaluate 
test your extension.

Note: You can can use Web Inspector to check runtime values and set breakpoints for JavaScript, however you cannot
use Web Inspector to debug into the J2ME layer (from JavaScript).

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/astanley)

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.template**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\webworks.template**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\webworks.template**_ directory to your new _**ext\webworks.template**_ directory

6. Copy the downloaded _**Smartphone\webworks.template\webworks**_ directory to your new _**ext\webworks.template**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.template**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.template" />

## Summary

	static boolean bool
	static string string
	static int integer
	static int add(int first, int second)
	static void log()

## Code Example

	function properties()
	{
		webworks.template.string = (webworks.template.bool ? "yes" : "no");
		webworks.template.integer = webworks.template.integer + 1;
	}

	function functions()
	{
		var result = webworks.template.add(5,5);
		webworks.template.log();
	}

