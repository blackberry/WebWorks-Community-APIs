# Contributing A Tablet OS API

When building an API for the BlackBerry WebWorks SDK for Tablet OS, you will start by creating a sub-directory within this _**Tablet**_ directory.  Pick a name that suits your extension and try not to use spaces in your directory name.  It just plain makes things easier.  

As a recipe for building your API you can use the _**Locale**_ as a good example.  In your API directory you will want to create a source code directory called _**src**_ where your ActionScript code will reside, and a source code directory called _**js**_ where your JavaScript code will reside.  You will also need to create a _**library.xml**_ file at the root of this directory.  More information on the _**library.xml**_ can be found below in this README.

A _**README.md**_ file also needs to be created at the root of your directory where you can place all the information about how to use and configure your API.  The _**Locale**_ is a good example of what type of information to include in your README.md file.  This README.md file uses Markdown wiki formatting. 


## Tablet OS API Tutorial

You can find an [API tutorial](http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/Tutorial-for-Writing-WebWorks-Extension-for-PlayBook/m-p/1172987) on creating JavaScript extensions for Tablet OS created by a member of the community. 

## library.xml Formatting

The below example shows what information needs to be added to your library.xml file:

    <library isWhitelist="true">

      <extension id="my.fancy.api">
        <entryClass>my.air.fully.qualified.namespace.fancy.api</entryClass>
      </extension>
      
      <platforms>
        <platform value="AIR">
          <target version="default" config="AIR_XHR" />
        </platform>
      </platforms>

      <configurations>
        <configuration name="AIR_XHR" version="1.0" comment="For XHR architecture">
			<src type="text/javascript" path="js/common/fancynewapi_dispatcher.js" comment="Application JS interface" />
			<src type="text/javascript" path="js/common/fancywapi_ns.js" comment="Application JS interface" />
			<src type="text/actionscript" path="src/Air/FancyNewAPI/src" comment="ActionScript implementation" />
		</configuration>
      </configurations>   
 
      <features>
        <feature id="fancy.new.api" version="1.0.0.0" />
      </features>

    </library>

**library.isWhiteListed**  Should be set to _**true**_.  This means that your API will be injected into JavaScript when the whitelisted feature ID is used in the applications _**config.xml**_ file.

**library\extension.id** This is the feature ID of your JavaScript extension that will be used in an app's config.xml file

**library\extension\entryClass** This value is the fully qualified name of the Class that implements your ActionScript extension.

**library\configurations\configuration\src.path** These are paths to the files that you need for your extention to run.  One file is for the JS dispatcher, one is for the JS namespace and one is for your ActionScript implementation _**src**_ directory

**library\features\feature.id** This is the feature ID of your JavaScript extension that will be used in an app's config.xml file.  Yes... it is in here twice!

**library\features\feature.version** This attribute is required but doesn't serve any meaning.  Leaving it at _1.0.0.0_ is all you need to do.

## Testing Your API

You can test your API with an existing installed WebWorks SDK for Tablet OS by following the below steps:

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks SDK for TabletOS\bbwp\ext**_

2. Create a new directory for your API in the _**ext**_ directory. This should be named the same as your feature id _**fancy.new.api**_

3. Copy your _**library.xml**_ file into to your new _**ext\fancy.new.api**_ directory

4. Copy your root code _**src**_ directory and your _**js**_ directory to your newly created _**ext\fancy.new.api**_ directory

5. Create a WebWorks Application using your API and create a feature element specifying the ID of your API.  &lt;feature id="fancy.new.api" /&gt;

6. Build your application using the _**bbwp.exe**_ command line and test your application on the desired simulator