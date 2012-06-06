# SendMenu Extension

This extension exposes the Blackberry Java Send/"Share" Menu API for use within WebWorks applications (Twitter, Facebook, SocialFeed, SMS, E-mail, PIN, etc).

**Authors:** [Wes Johnson](https://twitter.com/SterlingWes)

## Tested On

* BlackBerry Bold 9900 (OS 7.1.0.267)

**OS Requirement:** This API requires > OS 7.0

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/sterlingwes)

## How to install this extension

1. Download the source from this repository and extract it to a location on your computer.

2. Using File Explorer browse to the downloaded source code for this extension: _**Smartphone\SendMenu**_.

3. Copy the downloaded _**Smartphone\SendMenu\blackberry.sendmenu**_ directory to the extensions directory for the BlackBerry WebWorks SDK for Smartphone. The default path for this location is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_.

**NOTE:** Be sure to back-up this _**ext\blackberry.sendmenu**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="blackberry.ui.sendmenu" />

## Usage

### Constants

Send Command Types (the primary data type you're looking to send):

* blackberry.ui.sendmenu.TYPE_PATH
* blackberry.ui.sendmenu.TYPE_TEXT

Context JSON Keys:

* blackberry.ui.sendmenu.PATH
* blackberry.ui.sendmenu.SUBJECT
* blackberry.ui.sendmenu.TEXT

###  Methods

#### blackberry.ui.sendmenu.add(TYPE,CONTEXT[,GET_ALL,ORDERING,PRIORITY,FILTER])

Adds the Send Menu to the app's menu.

Arguments:

* `TYPE` is one of the above `TYPE_PATH` or `TYPE_TEXT` constants
* `CONTEXT` is a JSON object with valid keys `PATH`, `SUBJECT` and/or `TEXT`
* `GET_ALL` (optional) is a boolean indicating whether to get all commands, regardless of availability (defaults to `FALSE`)
* `ORDERING` (optional) is an integer indicating menu ordering (defaults to 0)
* `PRIORITY` (optional) is an integer indicating menu priority (defaults to 0)
* `FILTER` (optional) is a JSON object similar to `CONTEXT` which replaces values in `CONTEXT` based on command filters (in testing this had no affect on commands I thought it would, ie: Twitter or SMS limits), leave null for none

### blackberry.ui.sendmenu.remove()

Removes the Send Menu previously added in the current application session (the menu will not persist between sessions).

### blackberry.ui.sendmenu.setLabel(STRING_LABEL)

Sets the parent menu label for the sub menu containing all send commands. Defaults to `Share`.

## Code Example

	var context = {};
	blackberry.ui.sendmenu.setLabel('Send...');
	context[blackberry.ui.sendmenu.SUBJECT] = 'This is an e-mail subject';
	context[blackberry.ui.sendmenu.TEXT] = 'This message was sent from my WebWorks app!';
	blackberry.ui.sendmenu.add(blackberry.ui.sendmenu.TYPE_TEXT,JSON.stringify(context));
