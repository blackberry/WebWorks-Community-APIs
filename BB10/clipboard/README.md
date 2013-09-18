##Clipboard Extension for BlackBerry 10 WebWorks

###How to use

1. copy the `community.clipboard` folder under`ext` folder to your webworks installation directory's `/framework/ext` folder , and make it looks like this:

	`C:\Program Files (x86)\Research In Motion\BlackBerry 10 WebWorks SDK 1.0.4.11\Framework\ext\community.clipboard`
	
	`+  client.js`
	
	`+  index.js`
	
	`+  manifest.json`
	
	`+-device/libclipboard.so`
	
	`\-simulator/libclipboard.so`


2. in your `config.xml` file, add the following line feature:


	`<feature id="community.clipboard" required="true" version="1.0.0.0"/>`


3. in your app, use the following code to call the extension:

	`community.clipboard.getText();`
	
	`community.clipboard.setText("String");`

###About the author

Merrick Zhang (anphorea@gmail.com), BlackBerry Developer
