#!/bin/bash
rm -rf debug1
cordova create debug1
cd debug1
cordova plugin add ../plugin
rm -rf www/
cp -r ../sample/* ./
cordova run
