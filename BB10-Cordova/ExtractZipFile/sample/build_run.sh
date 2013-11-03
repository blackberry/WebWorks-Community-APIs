#!/bin/bash
# Throws error, might be bug on my part
cordova plugin rm community.extractZipFile;
cordova plugin add ../plugin/;
cordova run;
