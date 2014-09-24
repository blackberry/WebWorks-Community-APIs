# AboutScreen Control #

## NOTE: THIS IS A WORK IN PROGRESS ##

The AboutScreen Control will allow you to show a beautiful and standard about screen for your apps.

**Author:** [Walkline Wang](https://github.com/Walkline80)

![AboutScreen 9900](https://github.com/blackberry/WebWorks-Community-APIs/raw/master/Smartphone/AboutScreen/screenshot_9900.png)

![AboutScreen 9800](https://github.com/blackberry/WebWorks-Community-APIs/raw/master/Smartphone/AboutScreen/screenshot_9810.png)

## Tested On ##

* BlackBerry Bold 9900 v7.1.0.1098
* BlackBerry Bold 9930 v7.0.0.318 (simulator)
* BlackBerry Torch 9810 v7.1.0.912

**Requires BlackBerry WebWorks SDK for Smartphones v2.0 or higher, and only apply to BlackBerry OS6 or higher**

Have a problem with this extension?  [Log an Issue](https://github.com/blackberry/WebWorks-Community-APIs/issues) or contact the [Author](https://github.com/Walkline80)

## How To Configure The Extension For Use ##

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new _**webworks.ui.AboutScreen**_ directory in the _**ext**_ directory

3. Download the source from this repository and unzip it to a location on your computer

4. Using File Explorer browse to this extension's downloaded source code _**Smartphone\AboutScreen\Extension**_

5. Copy the _**library.xml**_ file from the downloaded _**Smartphone\AboutScreen\Extension**_ directory to your new _**ext\webworks.ui.AboutScreen**_ directory

6. Copy the downloaded _**Smartphone\AboutScreen\Extension\webworks**_ directory to your new _**ext\webworks.ui.AboutScreen**_ directory

**NOTE:** Be sure to back-up this _**ext\webworks.ui.AboutScreen**_ directory in your WebWorks SDK extensions directory before performing a WebWorks SDK upgrade. Simply copy it back into the _**ext**_ directory after you have completed your SDK upgrade.

## Required Feature ID ##
Whenever you use the below feature id in any of your WebWorks applications this extension will be loaded for use.

    <feature id="webworks.ui.AboutScreen" />

## Summary ##

    static void show(params : OptionsInterface)

    OptionsInterface:
    readwrite  property  String   title
    readwrite  property  String   version
    readwrite  property  String   intro
    readwrite  property  String   author
    readwrite  property  String   email
    readwrite  property  String   weibo
    readwrite  property  String   alipay
    readwrite  property  String   facebook
    readwrite  property  String   twitter

## Code Example ##

    function showAboutScreen()
    {
        //Configure the options
        var params = {'title': 'AboutScreen Control',
      				  'version': '0.0.1',
      				  'intro': 'Used to show a beautiful and standard about screen, just like this :)',
      				  'author': 'Walkline Wang',
      				  'email': 'walkline@gmail.com',	//optional
      				  'weibo': '@Walkline',				//optional
      				  'alipay': 'walkline@gmail.com'	//optional
      				  'facebook': 'walklinewang',		//optional
      				  'twitter': '@walklinewang',		//optional
      			 	 };

        //Open the about screen
		webworks.ui.AboutScreen.show(params);	
    }

## Usage Information ##
When creating a AboutScreen control you will pass it an options object that you can
create out of simple JSON. This will provide the AboutScreen with all the information it 
needs to display properly.

## Properties ##
**title:**
Is the title of your app

**version:**
Is the current version of your app

**intro:**
Is the shortly description of your app

**author:**
Is yourself

**email:**
Is your e-mail address, this is an optional item

**weibo**
Is an ID of your Sina Weibo, this is an optional item

**alipay**
Is an ID of your Alibaba payment tool, this is an optional item

**facebook**
Is an ID of your Facebook, this is an optional item

**twitter**
Is an ID of your Twitter, this is an optional item

## Change Log ##
_Empty_
