# blackberry.template

This template is designed to help developers get started writing their own WebWorks extensions. Download and modify 
the source and make changes throughout wherever you see a "STEP n" instruction.

**Authors:** [Adam Stanley](https://github.com/astanley)

## Tested On

* BlackBerry Tablet OS v 2.0.0.7971
* BlackBerry Tablet OS v 1.0.8.6067
* BlackBerry Tablet OS v 1.0.7.2670

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/astanley)

## How to install this extension

1. Download the source from this repository and extract it to a location on your computer.

2. Using File Explorer browse to the downloaded source code for this extension: _**Tablet\TEMPLATE**_.

3. Copy the downloaded _**Tablet\TEMPLATE\blackberry.template**_ directory to the extensions directory for the BlackBerry WebWorks SDK for Tablet OS. The default path for this location is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK for TabletOS\bbwp\ext**_.

**NOTE:** Be sure to back-up this _**ext\blackberry.template**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Troubleshooting
If you encounter Air Packager exceptions when building your WebWorks app, it likely means there is a syntax or compiling error within your extension code.

How to test?  Suggest building a sample WebWorks app, similar to **test_app**, that uses JavaScript unit tests to evaluate 
test your extension.

Note: You can can use Web Inspector to check runtime values and set breakpoints for JavaScript, however you cannot
use Web Inspector to debug into the AIR layer (from JavaScript).

## TODO
1. Show to support callbacks to JavaScript [example](http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/Callback-Extension-Object/m-p/1215117#M15050).
2. Refactor this sample. Is there any way to make it easier to understand by simplifying the code?

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.template" />

## Summary

This template demonstrates the basics of how to provide readable and writeable properties, as well as functions and properties.

	static boolean bool
	static string string
	static int integer
	static int add(int first, int second)
	static void log()

## Code Example

	function properties()
	{
		blackberry.template.string = (blackberry.template.bool ? "yes" : "no");
		blackberry.template.integer = blackberry.template.integer + 1;
	}

	function functions()
	{
		var result = blackberry.template.add(5,5);
		blackberry.template.log();
	}
