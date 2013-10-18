# Contributing A Smartphone API

When building an API for the BlackBerry WebWorks SDK for Smartphones, you will start by creating a sub-directory within this _**Smartphone**_ directory.  Pick a name that suits your extension and try not to use spaces in your directory name.  It just plain makes things easier.  

As a recipe for building your API you can use the _**SpinnerControl**_ as a good example.  In your API directory you will want to create a source code directory called _**src**_ where your actual API code will reside.  You will also need to create a _**library.xml**_ file at the root of this directory.  More information on the _**library.xml**_ can be found below in this README.

A _**README.md**_ file also needs to be created at the root of your directory where you can place all the information about how to use and configure your API.  The _**SpinnerControl**_ is a good example of what type of information to include in your README.md file.  This README.md file uses Markdown wiki formatting. 


## Smartphone API Tutorial

You can find a [very detailed tutorial](http://supportforums.blackberry.com/t5/Web-and-WebWorks-Development/Building-a-BlackBerry-WebWorks-JavaScript-Extension/ta-p/494139) on creating Smartphone JavaScript API extensions on the BlackBerry Developer Resource Center.  This Tutorial is slightly out of date when it comes to writing your _**library.xml**_ file but the information below on _**library.xml**_ will show you the correct way of contributing an API to this repository.

## library.xml Formatting

The below example uses the Spinner Control details to show what information needs to be added to your library.xml file:

    <library isWhitelist="true">

      <extension id="blackberry.ui.Spinner">
        <entryClass>blackberry.ui.SpinnerExtension</entryClass>
      </extension>
      
      <platforms>
        <platform value="JAVA">
          <target version="default" config="JAVA" />
        </platform>
      </platforms>

      <configurations>
        <configuration name="JAVA">
          <src type="text/java" path="blackberry" comment="API implementation in Java" />
        </configuration>
      </configurations>   
 
      <features>
        <feature id="blackberry.ui.Spinner" version="1.0.0.0">Popup Spinner control</feature>
      </features>

    </library>

**library.isWhiteListed**  Should be set to _**true**_.  This means that your API will be injected into JavaScript when the whitelisted feature ID is used in the applications _**config.xml**_ file.

**library\extension.id** This is the feature ID of your JavaScript extension that will be used in an app's config.xml file

**library\extension\entryClass** This value is the fully qualified name of the Class that exposes the _net.rim.device.api.web.WidgetExtension_ Java interface.

**library\configurations\configuration\src.path** This is the name of the root directory that is found in your extensions _**src**_ directory

**library\features\feature.id** This is the feature ID of your JavaScript extension that will be used in an app's config.xml file.  Yes... it is in here twice!

**library\features\feature.version** This attribute is required but doesn't serve any meaning.  Leaving it at _1.0.0.0_ is all you need to do.

## Testing Your API

You can test your API with an existing installed WebWorks SDK for Smartphones by following the below steps:

1. Locate your BlackBerry WebWorks SDK for Smartphone extensions directory using your File Explorer.  Default path is _**C:\Program Files\Research In Motion\BlackBerry WebWorks Packager\ext**_

2. Create a new directory for your API in the _**ext**_ directory. Make sure that this new directory is the same as your feature id _**blackberry.ui.Spinner**_

3. Copy your _**library.xml**_ file into to your new _**ext\blackberry.ui.Spinner**_ directory

4. Copy your root code directory(specified in the library\configurations\configuration\src.path) to your new _**ext\blackberry.ui.Spinner**_ directory

5. Create a WebWorks Application using your API and create a feature element specifying the ID of your API.  &lt;feature id="blackberry.ui.Spinner" /&gt;

6. Build your application using the _**bbwp.exe**_ command line and test your application on the desired simulator