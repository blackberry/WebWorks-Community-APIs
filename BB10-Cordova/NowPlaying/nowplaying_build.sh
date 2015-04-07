#!/bin/bash
rm -rf debug1
webworks create debug1
cd debug1
webworks plugin add ../plugin
rm -rf www/
cp -r ../sample/* ./
webworks run
