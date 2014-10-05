#!/bin/bash

PLUGIN="com.blackberry.community.audiometadata"
PLUGIN_PATH="../AudioMetaData/plugin"
DEVICE_PASS="1414"

echo "Removing Plugin " $PLUGIN
webworks plugin rm $PLUGIN

echo "Adding Plugin " $PLUGIN
webworks plugin add  $PLUGIN_PATH

echo "Deploying Plugin " $PLUGIN
webworks run --devicepass $DEVICE_PASS
