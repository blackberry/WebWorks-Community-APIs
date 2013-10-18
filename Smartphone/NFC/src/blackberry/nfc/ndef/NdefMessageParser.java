/*
 * Copyright 2011-2012 Research In Motion Limited.
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
package widgetpackage;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Vector;

import net.rim.device.api.io.Base64OutputStream;
import net.rim.device.api.io.nfc.ndef.NDEFMessage;
import net.rim.device.api.io.nfc.ndef.NDEFRecord;

import org.json.me.JSONArray;
import org.json.me.JSONException;
import org.json.me.JSONObject;

public class NdefMessageParser implements Runnable {

    private static NdefMessageParser _parser = new NdefMessageParser();
    private static NdefJavaScriptBridge _js_bridge = NdefJavaScriptBridge
            .getInstance();
    private Vector _messages;

    private NdefMessageParser() {
        _messages = new Vector();
    }

    public static NdefMessageParser getInstance(NDEFMessage message) {
        return _parser;
    }

    public void parseAndDeliver(NDEFMessage message) {
        log("parseAndDeliver(NDEFMessage)");
        synchronized (_messages) {
            _messages.addElement(message);
        }
        Thread parser_thread = new Thread(this);
        parser_thread.start();
    }

    protected JSONObject parseMessage(NDEFMessage _message)
            throws JSONException, IOException {
        log("parseMessage(NDEFMessage)");
        String record_type = "?";
        int typeNameFormat = NDEFRecord.TNF_WELL_KNOWN;

        JSONObject result = new JSONObject();
        /*
         * The NDEF Message may consist of a number of NDEF records
         */

        NDEFRecord[] records = _message.getRecords();
        /*
         * This is the number of NDEF records in the NDEF message
         */
        int numRecords = records.length;

        log("Parsing #records=" + numRecords);

        /*
         * Only unpick the message if it contains a non-zero number of records
         */
        JSONArray recordsArray = new JSONArray();
        result.put(Constants.NDEF_RECORDS, recordsArray);

        if (numRecords > 0) {
            /*
             * Work our way through each record in the message in turn
             */
            for (int j = 0; j < numRecords; ++j) {
                log("parseMessage record #" + j);

                NDEFRecord currentRecord = records[j];
                JSONObject jsonRecord = new JSONObject();
                recordsArray.put(jsonRecord);

                log("Processing NDEF record#=" + j);

                typeNameFormat = currentRecord.getTypeNameFormat();
                record_type = records[j].getType();

                if (typeNameFormat == NDEFRecord.TNF_WELL_KNOWN) {
                    if (Constants.NDEF_SMART_POSTER_TYPE.equals(records[j]
                            .getType())) {
                        mergeInto(jsonRecord, parseSmartPoster(records[j]));
                    } else if (Constants.NDEF_TEXT_TYPE.equals(records[j]
                            .getType())) {
                        mergeInto(jsonRecord, parseText(records[j]));
                    } else if (Constants.NDEF_URI_TYPE.equals(records[j]
                            .getType())) {
                        mergeInto(jsonRecord, parseURI(records[j]));
                    } else {
                        // Gc, Hr, Hs, Hc, Sig are not currently parsed by this
                        // implementation...
                    }
                } else if (typeNameFormat == NDEFRecord.TNF_MEDIA) {
                    mergeInto(jsonRecord, parseMediaRecord(records[j]));
                } else if (typeNameFormat == NDEFRecord.TNF_EXTERNAL) {
                    NDEFMessage message = new NDEFMessage(
                            records[j].getPayload());
                    mergeInto(jsonRecord, parseMessage(message));
                }

                if (!jsonRecord.has(Constants.NDEF_PAYLOAD64)) {
                    mergeInto(jsonRecord, parseGenericRecord(records[j]));
                }

                // Put the first record at the top level - to simplify
                // accessing.
                if (j == 0) {
                    mergeInto(result, jsonRecord);
                }
            }
        } else {
            log("No records in message");
            return null;
        }
        return result;
    }

    public void run() {
        NDEFMessage message = null;
        synchronized (_messages) {
            message = (NDEFMessage) _messages.elementAt(_messages.size() - 1);
            _messages.removeElementAt(_messages.size() - 1);
        }

        JSONObject toUse;
        try {
            toUse = parseMessage(message);
            try {
                _js_bridge.useNDEFMessage(
                        toUse.getInt(Constants.NDEF_TYPE_NAME_FORMAT),
                        toUse.getString(Constants.NDEF_TYPE), toUse.toString());
            } catch (JSONException j) {
                log("Could not send object back to event handler", j);
            }
        } catch (Exception e) {
            String record_type = "?";
            int typeNameFormat = NDEFRecord.TNF_WELL_KNOWN;
            try {
                NDEFRecord[] records = message.getRecords();
                if (records.length > 0) {
                    record_type = records[0].getType();
                    typeNameFormat = records[0].getTypeNameFormat();
                }
            } catch (Exception toIgnore) {
                // consume
            }
            _js_bridge.reportError(typeNameFormat, record_type, e.getClass()
                    .getName() + ": " + e.getMessage());
            log("Problem inside run", e);
        }
    }

    private void log(String message) {
        Utilities.log(NdefMessageParser.class.getName() + ": " + message);
    }

    private void log(String message, Throwable t) {
        Utilities.log(NdefMessageParser.class.getName() + ": " + message, t);
    }

    private void mergeInto(JSONObject target, JSONObject source)
            throws JSONException {
        log("mergeInto(JSONObject,JSONObject)");
        Enumeration e = source.keys();
        while (e.hasMoreElements()) {
            String key = String.valueOf(e.nextElement());
            if (!target.has(key)) {
                target.put(key, source.get(key));
            }
        }
    }

    protected JSONObject parseURI(NDEFRecord record) throws JSONException,
            IOException {
        log("parseURL(NDEFRecord)");
        JSONObject toReturn = parseGenericRecord(record);
        byte[] payload = record.getPayload();
        StringBuffer uriBuffer = new StringBuffer();
        uriBuffer.append(URLPrefixes.getPrefix(payload[0] & 0xff));
        uriBuffer.append(new String(payload, 1, payload.length - 1, "UTF-8"));
        toReturn.put(Constants.NDEF_URI, uriBuffer.toString());
        return toReturn;
    }

    protected JSONObject parseGenericRecord(NDEFRecord record)
            throws JSONException, IOException {
        log("parseGenericRecord(NDEFRecord)");
        JSONObject toReturn = new JSONObject();
        toReturn.putOpt(Constants.NDEF_ID, record.getId());
        toReturn.putOpt(Constants.NDEF_TYPE, record.getType());
        toReturn.put(Constants.NDEF_TYPE_NAME_FORMAT,
                record.getTypeNameFormat());

        byte[] payloadBytes = record.getPayload();
        JSONArray bytePayload = new JSONArray();

        for (int i = 0; i < payloadBytes.length; i++) {
            byte b = payloadBytes[i];
            bytePayload.put(b & 0xff);
        }

        toReturn.put("payload", bytePayload);
        return toReturn;
    }

    protected JSONObject parseText(NDEFRecord record) throws JSONException,
            IOException {
        log("parseText(NDEFRecord)");
        byte[] payload = record.getPayload();
        if (payload.length == 0) {
            log("Text record too short to read");
            return null;
        }
        int statusByte = payload[0] & 0xff;

        boolean isUTF16 = Utilities.isUtf16Encoded(statusByte);
        int languageCodeLength = Utilities
                .getIanaLanguageCodeLength(statusByte);

        int textLength = payload.length - 1 - languageCodeLength;

        String languageCode = "";
        if (languageCodeLength > 0) {
            languageCode = new String(payload, 1, languageCodeLength,
                    "US-ASCII");
        }

        String text = "";
        if (textLength > 0) {
            text = new String(payload, 1 + languageCodeLength, textLength,
                    isUTF16 ? "UTF-16" : "UTF-8");
        }

        JSONObject toReturn = parseGenericRecord(record);
        toReturn.putOpt(Constants.NDEF_TEXT_LANGUAGE_CODE, languageCode);
        toReturn.putOpt(Constants.NDEF_TEXT_VALUE, text);
        return toReturn;
    }

    protected JSONObject parseSmartPoster(NDEFRecord record)
            throws JSONException, IOException {
        log("parseSmartPoster(NDEFRecord)");
        JSONObject toReturn = new JSONObject();
        JSONArray recordsArray = new JSONArray();
        toReturn.put("records", recordsArray);
        // testing

        // if (true)
        // throw new JSONException("Fictitious JSON problem");

        // if(true)
        // throw new
        // UnsupportedEncodingException("Fictitious encoding problem");

        // if(true)
        // throw new BadFormatException("Fictitious format problem");

        // if(true)
        // throw new NFCException("Fictitious NFC problem");

        log("Recognised a Smart Poster Message");
        // try {
        NDEFMessage smartPosterMessage = new NDEFMessage(record.getPayload());
        NDEFRecord[] spRecords = smartPosterMessage.getRecords();
        int numSpRecords = spRecords.length;
        log("Parsing smartposter #records=" + numSpRecords);

        if (numSpRecords > 0) {
            log("Parsing Smart Poster Message");
            for (int k = 0; k < numSpRecords; ++k) {
                log("parseSmartPoster record #" + k);
                JSONObject currentRecordJSON = parseGenericRecord(spRecords[k]);
                recordsArray.put(currentRecordJSON);
                log("Parsing SP record#=" + k);
                byte[] spPayloadBytes = spRecords[k].getPayload();

                int tnf = spRecords[k].getTypeNameFormat();
                String type = spRecords[k].getType();

                log("entering parseSmartPoster if");

                if (tnf == NDEFRecord.TNF_WELL_KNOWN) {
                    if (Constants.NDEF_TEXT_TYPE.equals(type)) {
                        log(Constants.NDEF_TEXT_TYPE);
                        JSONObject text = parseText(spRecords[k]);
                        mergeInto(currentRecordJSON, text);
                        if (text != null) {
                            toReturn.putOpt(Constants.NDEF_TEXT,
                                    text.get(Constants.NDEF_TEXT_VALUE));
                            getOrCreateArray(toReturn,
                                    Constants.NDEF_TEXT_VALUES).put(text);
                        }
                    } else if (Constants.NDEF_URI_TYPE.equals(type)) {
                        log(Constants.NDEF_URI_TYPE);
                        JSONObject uri = parseURI(spRecords[k]);
                        mergeInto(currentRecordJSON, uri);
                        toReturn.put(Constants.NDEF_URI,
                                uri.get(Constants.NDEF_URI));
                        toReturn.put(Constants.NDEF_URI_LEGACY,
                                uri.get(Constants.NDEF_URI));
                    } else if (Constants.NDEF_SMART_POSTER_RECOMMENDED_ACTION_TYPE
                            .equals(type)) {
                        log("Reading an action");
                        if (spPayloadBytes.length != 1) {
                            log("Incorrect length noted for 'action' record: "
                                    + spPayloadBytes.length);
                        }
                        if (spPayloadBytes.length > 0) {
                            toReturn.put(
                                    Constants.NDEF_SMART_POSTER_RECOMMENDED_ACTION,
                                    spPayloadBytes[0] & 0xff);
                        }
                    } else if (Constants.NDEF_SMART_POSTER_URI_SIZE_TYPE
                            .equals(type)) {
                        log("Reading size");
                        if (spPayloadBytes.length != 4) {
                            log("Incorrect length noted for 'size' record: "
                                    + spPayloadBytes.length);
                        }
                        if (spPayloadBytes.length >= 4) {
                            long size = 0;
                            for (int i = 0; i < 4; ++i) {
                                size <<= 8;
                                size |= spPayloadBytes[i] & 0xff;
                            }
                            toReturn.put(Constants.NDEF_SMART_POSTER_URI_SIZE,
                                    size);
                        }
                    } else if (Constants.NDEF_SMART_POSTER_URI_TYPE_TYPE
                            .equals(type)) {
                        log("Reading URI Type");
                        if (spPayloadBytes.length == 0) {
                            toReturn.put(Constants.NDEF_SMART_POSTER_URI_TYPE,
                                    "");
                        } else {
                            toReturn.put(Constants.NDEF_SMART_POSTER_URI_TYPE,
                                    new String(spPayloadBytes, "UTF-8"));
                        }
                    } else {
                        // Unexpected TNF_WELL_KNOWN for a smart poster...
                        log("Unexpected type for TNF_WELL_KNOWN for smart poster: "
                                + type);
                    }
                } else if (tnf == NDEFRecord.TNF_MEDIA) {
                    log("Reading " + NDEFRecord.TNF_MEDIA);
                    try {
                        getOrCreateArray(toReturn,
                                Constants.NDEF_SMART_POSTER_ICONS).put(
                                parseMediaRecord(spRecords[k]));
                    } catch (IOException ioe) {
                        log("Could not build media record: " + ioe.getMessage());
                    }
                } else {
                    log("Unexpected type name format for smart poster: " + tnf);
                }

                log("finished if");
            }
        } else {
            log("Empty Smart Poster Message");
            toReturn.putOpt(Constants.NDEF_ERROR,
                    "NdefMessageParser empty Smart Poster Message");
        }
        return toReturn;
    }

    protected JSONObject parseMediaRecord(NDEFRecord ndefRecord)
            throws JSONException, IOException {
        log("parseMediaRecord(NDEFRecord)");
        JSONObject toReturn = parseGenericRecord(ndefRecord);
        byte[] payLoad = ndefRecord.getPayload();
        String base64 = Base64OutputStream.encodeAsString(payLoad, 0,
                payLoad.length, false, false);
        toReturn.put(Constants.NDEF_MEDIA_MIME_TYPE, ndefRecord.getType());
        toReturn.put(Constants.NDEF_MEDIA_MIME_BODY, base64);
        toReturn.put(Constants.NDEF_MEDIA_DATA_URI,
                "data:" + ndefRecord.getType() + ";base64," + base64);
        return toReturn;
    }

    private JSONArray getOrCreateArray(JSONObject object, String name)
            throws JSONException {
        log("getOrCreateArray(JSONObject,String)");
        JSONArray toReturn = null;
        if (object.has(name)) {
            toReturn = object.getJSONArray(name);
        } else {
            toReturn = new JSONArray();
            object.put(name, toReturn);
        }
        return toReturn;
    }
}
