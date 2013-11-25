#!/bin/bash
# Throws error, might be bug on my part
cordova plugin rm community.extractZipFile
cordova platform rm blackberry10
cordova platform add blackberry10
cordova plugin add ../plugin/
cordova run
