# Message List Information

## NOTE: THIS IS A WORK IN PROGRESS

The Message List API will allow you to add items to the main message list much like BBM, Facebook and Twitter do.  When clicking on the item it will then invoke your application. 

**Authors:** [Tim Neil](https://github.com/tneil), [John Mutter](https://github.com/jmutter)

## Known Issues

* Currently none of the items persist past a restart
* Unique identifiers hardcoded and will not allow for more than one application on the device using this API
* Default image overrides on adding new items is not yet working

## Tested On

* BlackBerry Torch 9800 v6.0.0.227
* BlackBerry Storm 9550 v5.0.0.469
* BlackBerry Bold 9700 v5.0.0.469

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks/issues) or contact the Authors

## How To Configure The Extension For Use

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.message.list**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\MessageList**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\MessageList**_ directory to your new _**ext\webworks.message.list**_ directory

6. Copy the downloaded _**Smartphone\MessageList\src\webworks**_ directory to your new _**ext\webworks.message.list\webworks**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.message.list**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.message.list" />

## Summary

    static function onItemOpened;  		// function(item : Item)
	static function onItemMarkedRead;	// function(item : Item)
	static function onItemMarkedUnread;	// function(item : Item)
	static function onItemDeleted;		// function(item : Item)
	
	static void init(options : InitOptions); 
    static void addItem(item : Item); 
	static Item getItem(id : string);
	static void removeItem(id : string);
	static void markItemRead(id : string);
	
	/* TODO: 
	static void markAllRead();	
	static Item[] getItems();
	static void updateItem(item : Item);

## Options Summary

    readwrite String guid;  				// Unique Identifier for your message store
	readwrite String folderName; 			// Name for the custom folder
	readwrite String defaultNewImage;		// Default image to use for new items.  Must be local relative path
	readwrite String defaultReadImage;		// Default image to use for read items.  Must be local relative path
	
## Item Summary

    readwrite String id;					// Unique ID given to the item
	readwrite String title; 				// Title of item
	readwrite String description;			// Description of the item
	optional readwrite String imageNew;		// Image to be displayed when the item is new.  Uses the default if not specified
	optional readwrite String imageRead;	// Image to be displayed when the item is read.  Uses the default if not specified

## Initialization Example

    function init() {
 	    
		// Setup our listeners
		webworks.message.list.onItemOpened = onItemOpened;
		webworks.message.list.onItemMarkedRead = onItemMarkedRead;
		webworks.message.list.onItemMarkedUnread = onItemMarkedUnread;
		webworks.message.list.onItemDeleted = onItemDeleted;
		
		// Configure our options
		var options = { 'guid' : 'my.app.id',
					'folderName' : 'My Cool App',
					'defaultNewImage' : '/images/new.png',
					'defaultReadImage' : '/images/read.png',
					'defaulDeletedImage' : '/images/deleted.png'
				    }
	    
		// Initialize the message list integration
	    webworks.message.list.init(options);
    }

    function onItemOpened(item) {
		blackberry.app.requestForeground();
		addStatusMessage('Item Opened: ' + item.id);
    }

    function onItemMarkedRead(item) {
		addStatusMessage('Item Marked Read: ' + item.id);
	}
	
	function onItemMarkedUnread(item) {
		addStatusMessage('Item Marked Unread: ' + item.id);
	}

	function onItemDeleted(item) {
		addStatusMessage('Item Deleted: ' + item.id);
	}
	
	function addStatusMessage(message) {
		var list = document.getElementById('messageList');
		var item = document.createElement('div');
		item.innerHTML = message;
		list.appendChild(item);
		item = document.createElement('br');
		list.appendChild(item);
	}

## Add Item Example

    function postItemToList() {
	    var item = { 'id' : blackberry.utils.generateUniqueId().toString(),
				'title' : 'my title',
				'description' : 'my description'
				}
	    webworks.message.list.addItem(item);
    }
	
## Delete Item Example

    function removeItem(id) {
		webworks.message.list.removeItem(id);
	}
	

