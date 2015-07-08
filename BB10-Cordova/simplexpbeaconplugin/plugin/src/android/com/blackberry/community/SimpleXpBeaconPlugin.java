package com.blackberry.community;

/*
 * Copyright (c) 2015 BlackBerry Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.pm.PackageManager;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaArgs;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.LOG;
import org.apache.cordova.PluginResult;
import org.json.JSONException;
import org.json.JSONObject;
import android.util.Log;
import java.util.UUID;

public class SimpleXpBeaconPlugin extends CordovaPlugin {

    private static final String TAG = "SimpleXpBeaconPlugin";

    private static final String PLUGIN_VERSION = "1.0.0";

    private static final String ACTION_INITIALISE_BLUETOOTH = "initialiseBluetooth";
    private static final String ACTION_TERMINATE_BLUETOOTH = "terminateBluetooth";
    private static final String ACTION_PLUGIN_VERSION = "pluginVersion";
    private static final String ACTION_START_MONITORING = "startMonitoring";
    private static final String ACTION_STOP_MONITORING = "stopMonitoring";

    private static final String JSON_KEY_STATUS = "status";
    private static final String JSON_KEY_DESCRIPTION = "desc";
    private static final String JSON_KEY_ERROR_CODE = "error_code";
    private static final String JSON_KEY_EVENT = "event";
    private static final String JSON_KEY_PLUGIN_VERSION = "plugin_version";
    private static final String JSON_KEY_DATA = "data";
    private static final String JSON_KEY_IBEACON_UUID = "uuid";
    private static final String JSON_KEY_IBEACON_MAJOR = "major";
    private static final String JSON_KEY_IBEACON_MINOR = "minor";
    private static final String JSON_KEY_IBEACON_RSSI = "rssi";
    private static final String JSON_KEY_IBEACON_TXPOWER = "txpower";

    private static final String JSON_VALUE_ERROR = "ERROR";
    private static final String JSON_VALUE_OK = "OK";
    private static final String JSON_VALUE_STARTED = "STARTED";
    private static final String JSON_VALUE_IBEACON = "IBEACON";
    private static final String JSON_VALUE_PLUGIN_DESCRIPTION = "Plugin Version";
    private static final String JSON_VALUE_NO_BT_LE_FEATURE = "This device doesn't have BT LE feature";
    private static final String JSON_VALUE_NO_BT_ADAPTER = "Unable to obtain Bluetooth Adapter";
    private static final String JSON_VALUE_BT_ALREADY_INITIALISED = "Bluetooth already initialised";
    private static final String JSON_VALUE_BT_NOT_INITIALISED = "Bluetooth not initialised";
    private static final String JSON_VALUE_BT_INITIALISED = "Bluetooth initialised";
    private static final String JSON_VALUE_BT_TERMINATE = "Bluetooth Terminated";
    private static final int JSON_VALUE_DEFAULT_ERROR_CODE = -1;
    private static final String JSON_VALUE_ALREADY_MONITORING_FOR_I_BEACONS = "Already monitoring for iBeacons";
    private static final String JSON_VALUE_FAILED_TO_ENABLE_MONITORING = "Failed to enable iBeacon monitoring";
    private static final String JSON_VALUE_NOT_MONITORING = "Not monitoring for iBeacons";
    private static final String JSON_VALUE_FAILED_TO_DISABLE_MONITORING = "Failed to Disable Monitoring";
    private static final String JSON_VALUE_STOPPED_MONITORING = "Stopped Monitoring for iBeacons OK";
    private static final String JSON_VALUE_REQUESTED_MONITORING = "Requested iBeacon Monitoring OK";
    private static final String JSON_VALUE_IBEACON_EVENT = "iBeacon event";

    private BluetoothAdapter mBluetoothAdapter;
    private boolean mBtInitialised = false;
    private boolean mMonitoring = false;
    private CallbackContext mMonitoringCallbackContext;

    private boolean mSupressMonitorCallback = false;

    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {
                @Override
                public void onLeScan(final BluetoothDevice device, final int rssi, final byte[] scanRecord) {
                    cordova.getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Log.d(TAG, "XXXX Device Address: " + device.getAddress().toString());
                            Log.d(TAG, "XXXX RSSI: " + rssi);
                            BeaconData beaconData = new BeaconData(scanRecord);
                            if (beaconData.hasIBeaconData()) {
                                Log.d(TAG, "XXXX TxPowerLevel: " + beaconData.txPowerLevel());
                                Log.d(TAG, "XXXX Major: " + beaconData.major());
                                Log.d(TAG, "XXXX Minor: " + beaconData.minor());
                                Log.d(TAG, "XXXX UUID: " + beaconData.uuid().toString());

                                JSONObject response = new JSONObject();
                                JSONObject data = new JSONObject();

                                try {
                                    data.put(JSON_KEY_IBEACON_UUID, beaconData.uuid().toString());
                                    data.put(JSON_KEY_IBEACON_MAJOR, beaconData.major());
                                    data.put(JSON_KEY_IBEACON_MINOR, beaconData.minor());
                                    data.put(JSON_KEY_IBEACON_RSSI, rssi);
                                    data.put(JSON_KEY_IBEACON_TXPOWER, beaconData.txPowerLevel());

                                    response.put(JSON_KEY_STATUS, JSON_VALUE_OK);
                                    response.put(JSON_KEY_EVENT, JSON_VALUE_IBEACON);
                                    response.put(JSON_KEY_DESCRIPTION, JSON_VALUE_IBEACON_EVENT);
                                    response.put(JSON_KEY_DATA, data);

                                    if (!isSupressMonitorCallback()) {
                                        PluginResult result = new PluginResult(PluginResult.Status.OK, response.toString());
                                        result.setKeepCallback(true);
                                        getMonitoringCallbackContext().sendPluginResult(result);
                                    }

                                } catch (JSONException e) {
                                    Log.d(TAG, "XXXX JSON Error: " + e.getMessage());
                                }
                            }
                        }
                    });
                }
            };

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        LOG.d(TAG, "XXXX in initialize");
    }
    @Override
    public boolean execute(String action, CordovaArgs args, CallbackContext callbackContext) throws JSONException {

        LOG.d(TAG, "XXXX requested action = " + action);

        boolean validAction = false;

        if (action.equals(ACTION_INITIALISE_BLUETOOTH)) {

            LOG.d(TAG, "XXXX Processing Initialise Bluetooth request");

            validAction = true;

            if (!deviceHasBtLeFeature()) {
                errorResponse(callbackContext, JSON_VALUE_NO_BT_LE_FEATURE);
                return validAction;
            }

            if (isBtInitialised()) {
                errorResponse(callbackContext, JSON_VALUE_BT_ALREADY_INITIALISED);
                return validAction;
            }

            BluetoothManager bluetoothManager =
                    (BluetoothManager) cordova.getActivity()
                            .getSystemService(Context.BLUETOOTH_SERVICE);
            mBluetoothAdapter = bluetoothManager.getAdapter();

            if (mBluetoothAdapter == null) {
                errorResponse(callbackContext, JSON_VALUE_NO_BT_ADAPTER);
                return validAction;
            }

            successResponse(callbackContext, JSON_VALUE_BT_INITIALISED);
            setBtInitialised(true);
            return validAction;

        } else if (action.equals(ACTION_TERMINATE_BLUETOOTH)) {
            
            LOG.d(TAG, "XXXX Processing Terminate Bluetooth request");

            validAction = true;

            if (!isBtInitialised()) {
                errorResponse(callbackContext, JSON_VALUE_BT_NOT_INITIALISED);
                return validAction;
            }

            mBluetoothAdapter.disable();
            mBluetoothAdapter = null;

            successResponse(callbackContext, JSON_VALUE_BT_TERMINATE);
            setBtInitialised(false);
            return validAction;

        } else if (action.equals(ACTION_PLUGIN_VERSION)) {

            LOG.d(TAG, "XXXX Processing Plugin Version request");

            validAction = true;

            pluginVersionResponse(callbackContext, PLUGIN_VERSION);
            return validAction;

        } else if (action.equals(ACTION_START_MONITORING)) {
            
            LOG.d(TAG, "XXXX Processing Start Monitoring request");
            validAction = true;

            if (!isBtInitialised()) {
                errorResponse(callbackContext, JSON_VALUE_BT_NOT_INITIALISED);
                return validAction;
            }

            if (isMonitoring()) {
                errorResponse(callbackContext, JSON_VALUE_ALREADY_MONITORING_FOR_I_BEACONS);
                return validAction;
            }

            if (!enableMonitoring(callbackContext)) {
                monitorFailResponse(callbackContext);
                return validAction;
            }

            setSupressMonitorCallback(false);
            monitorSuccessResponse(callbackContext);
            return validAction;

        } else if (action.equals(ACTION_STOP_MONITORING)) {

            LOG.d(TAG, "XXXX Processing Stop Monitoring request");
            validAction = true;

            if (!isBtInitialised()) {
                errorResponse(callbackContext, JSON_VALUE_BT_NOT_INITIALISED);
                return validAction;
            }

            if (!isMonitoring()) {
                errorResponse(callbackContext, JSON_VALUE_NOT_MONITORING);
                return validAction;
            }

            if (!disableMonitoring(callbackContext)) {
                errorResponse(callbackContext, JSON_VALUE_FAILED_TO_DISABLE_MONITORING);
                return validAction;
            }

            successResponse(callbackContext, JSON_VALUE_STOPPED_MONITORING);
            return validAction;

        } else {

            LOG.d(TAG, "XXXX Unmatched action" + action);
            validAction = false;
        }

        return validAction;
    }

    private boolean enableMonitoring(CallbackContext callbackContext) {

        boolean rc = true;

        if (mBluetoothAdapter.startLeScan(mLeScanCallback)) {
            setMonitoring(true);
            setMonitoringCallbackContext(callbackContext);
        } else {
            setMonitoring(false);
            rc = false;
        }
        return rc;
    }

    private boolean disableMonitoring(CallbackContext callbackContext) {
        mBluetoothAdapter.stopLeScan(mLeScanCallback);
        setMonitoring(false);
        setMonitoringCallbackContext(null);
        return true;
    }

    private void pauseMonitoring() {
        if (isMonitoring()) {
            setSupressMonitorCallback(true);
            mBluetoothAdapter.stopLeScan(mLeScanCallback);
        }
    }

    private void resumeMonitoring() {
        if (isMonitoring()) {
            mBluetoothAdapter.startLeScan(mLeScanCallback);
            setSupressMonitorCallback(false);
        }
    }

    private void pluginVersionResponse(CallbackContext callbackContext, String version) throws JSONException {

        // {"desc":"Plugin Version","plugin_version":"1.0.0","status":"OK"}

        JSONObject response = new JSONObject();
        response.put(JSON_KEY_PLUGIN_VERSION, version);
        response.put(JSON_KEY_DESCRIPTION, JSON_VALUE_PLUGIN_DESCRIPTION);
        response.put(JSON_KEY_STATUS, JSON_VALUE_OK);
        callbackContext.success(response.toString());
    }

    private void successResponse(CallbackContext callbackContext, String description) throws JSONException {

        // {"desc":"...","status":"OK"}

        JSONObject response = new JSONObject();
        response.put(JSON_KEY_DESCRIPTION, description);
        response.put(JSON_KEY_STATUS, JSON_VALUE_OK);
        callbackContext.success(response.toString());
    }

    private void monitorSuccessResponse(CallbackContext callbackContext) throws JSONException {

        // {"desc":"...","status":"OK"}

        JSONObject response = new JSONObject();
        response.put(JSON_KEY_DESCRIPTION, JSON_VALUE_REQUESTED_MONITORING);
        response.put(JSON_KEY_STATUS, JSON_VALUE_OK);
        response.put(JSON_KEY_EVENT, JSON_VALUE_STARTED);

        PluginResult result = new PluginResult(PluginResult.Status.OK, response.toString());
        result.setKeepCallback(true);
        callbackContext.sendPluginResult(result);
    }

    private void monitorFailResponse(CallbackContext callbackContext) throws JSONException {

        // {"desc":"...","status":"ERROR", "error_code": nnn, "event":"STARTED"}

        JSONObject response = new JSONObject();
        response.put(JSON_KEY_DESCRIPTION, JSON_VALUE_FAILED_TO_ENABLE_MONITORING);
        response.put(JSON_KEY_STATUS, JSON_VALUE_ERROR);
        response.put(JSON_KEY_ERROR_CODE, JSON_VALUE_DEFAULT_ERROR_CODE);
        response.put(JSON_KEY_EVENT, JSON_VALUE_STARTED);
        callbackContext.success(response.toString());
    }

    private void errorResponse(CallbackContext callbackContext, String description) throws JSONException {

        // {"desc":"...","status":"ERROR", "error_code": nnn}

        JSONObject response = new JSONObject();
        response.put(JSON_KEY_DESCRIPTION, description);
        response.put(JSON_KEY_STATUS, JSON_VALUE_ERROR);
        response.put(JSON_KEY_ERROR_CODE, JSON_VALUE_DEFAULT_ERROR_CODE);
        callbackContext.success(response.toString());
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        LOG.d(TAG, "XXXX In onDestroy()");

        if (isMonitoring()) {
            mBluetoothAdapter.stopLeScan(mLeScanCallback);
            setMonitoring(false);
        }

        if (mBluetoothAdapter != null) {
            mBluetoothAdapter = null;
            setBtInitialised(false);
        }
    }

    @Override
    public void onPause(boolean multitasking) {
        super.onPause(multitasking);
        LOG.d(TAG, "XXXX In onPause()");
        pauseMonitoring();
    }

    @Override
    public void onResume(boolean multitasking) {
        super.onResume(multitasking);
        LOG.d(TAG, "XXXX In onResume()");
        resumeMonitoring();
    }

    private boolean deviceHasBtLeFeature() {
        boolean rc = false;
        if (cordova.getActivity().getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            rc = true;
        }
        return rc;
    }

    public boolean isSupressMonitorCallback() {
        return mSupressMonitorCallback;
    }

    public void setSupressMonitorCallback(boolean supressMonitorCallback) {
        this.mSupressMonitorCallback = supressMonitorCallback;
    }

    private boolean isBtInitialised() {
        return mBtInitialised;
    }

    private void setBtInitialised(boolean btInitialised) {
        this.mBtInitialised = btInitialised;
    }

    private boolean isMonitoring() {
        return mMonitoring;
    }

    private void setMonitoring(boolean monitoring) {
        this.mMonitoring = monitoring;
    }


    private CallbackContext getMonitoringCallbackContext() {
        return mMonitoringCallbackContext;
    }

    private void setMonitoringCallbackContext(CallbackContext monitoringCallbackContext) {
        this.mMonitoringCallbackContext = monitoringCallbackContext;
    }

    private class BeaconData {

        private byte[] mScanData;
        private int mMajor = 0;
        private int mMinor = 0;
        private int mTxPowerLevel = 0;
        private UUID mUuid;
        private boolean mHasIBeaconData = false;

        public BeaconData(byte[] scanData) {
            this.mScanData = scanData.clone();
            parseScanData();
        }

        private void parseScanData() {

            mHasIBeaconData = false;

            if (mScanData.length < 27) {
                return;
            }

            byte[] beaconUuid = new byte[16];
            byte[] beaconUuidLowHalf = new byte[beaconUuid.length/2];
            byte[] beaconUuidHighHalf = new byte[beaconUuid.length/2];
            long beaconUuidLeastSig = 0;
            long beaconUuidMostSig = 0;

            int i = 0;
            int entryLen = 0;
            int entryType = 0;
            do {
                entryLen = mScanData[i];
                entryType = mScanData[i+1];

                if ((entryType & 0xff) == 0xff) {
                    int j = i+2;
                    if ((mScanData[j] == 0x4c) && (mScanData[j+1] == 0x00) &&
                            (mScanData[j+2] == 0x02) && (mScanData[j+3] == 0x15)) {


                        mHasIBeaconData = true;
                        for (int k=0; k<beaconUuid.length; k++) {
                            beaconUuid[k] = mScanData[k+j+4];
                        }
                        j += (beaconUuid.length + 4);
                        mMajor = 0;
                        mMajor = (mScanData[j+1] & 0xff);
                        mMajor += ((mScanData[j] & 0xff) << 8);
                        j += 2;
                        mMinor = 0;
                        mMinor = (mScanData[j+1] & 0xff);
                        mMinor += ((mScanData[j] & 0xff) << 8);
                        j += 2;
                        mTxPowerLevel = ((-1 << 8) & 0xffffff00);
                        mTxPowerLevel += (mScanData[j] & 0xff);
                    }
                }
                i += (entryLen+1);
            } while (i < mScanData.length && entryLen != 0 );

            if (mHasIBeaconData) {
                for (i=0; i < (beaconUuidLowHalf.length); i++) {
                    beaconUuidHighHalf[i] = beaconUuid[i];
                    beaconUuidLowHalf[i] = beaconUuid[i + beaconUuidLowHalf.length];
                }
                beaconUuidLeastSig = bytesToLong(beaconUuidLowHalf);
                beaconUuidMostSig = bytesToLong(beaconUuidHighHalf);
                mUuid = new UUID(beaconUuidMostSig, beaconUuidLeastSig);
            }
        }

        private long bytesToLong(byte[] b) {
            long result = 0;
            for (int i = 0; i < 8; i++) {
                result <<= 8;
                result |= (b[i] & 0xFF);
            }
            return result;
        }

        public boolean hasIBeaconData() {
            return mHasIBeaconData;
        }

        public int txPowerLevel() {
            return mTxPowerLevel;
        }

        public int major() {
            return mMajor;
        }

        public int minor() {
            return mMinor;
        }

        public UUID uuid() {
            return mUuid;
        }
    }
}
