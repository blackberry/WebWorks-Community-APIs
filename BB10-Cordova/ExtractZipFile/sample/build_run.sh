#!/bin/bash
# Throws error, might be bug on my part
cordova plugin rm community.extractzipfile;
cordova plugin add ../plugin/;
cordova run;
