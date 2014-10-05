/*
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.
 */

var _self = {},
    _ID = "com.blackberry.invoke.card",
    exec = cordova.require("cordova/exec"),
    _btoa = require('./btoa'),
    _noop = function () {};

function cb(done, cancel, invoke) {
    return function (data) {
        var type = data.type,
            result = data.result;

        switch (type) {
        case "done":
            if (done && typeof(done) === "function") {
                done(result);
            }
            break;
        case "cancel":
            if (cancel && typeof(cancel) === "function") {
                cancel(result);
            }
            break;
        case "invoke":
            if (invoke && typeof(invoke) === "function") {
                invoke(result);
            }
            break;
        }
    };
}

function defineReadOnlyField(obj, field, value) {
    Object.defineProperty(obj, field, {
        "value": value,
        "writable": false
    });
}

_self.invokeMediaPlayer = function (options, done, cancel, invokeCallback) {
    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeMediaPlayer", {options: options || {}});
};

_self.invokeCamera = function (mode, done, cancel, invokeCallback) {
    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeCamera", {mode: mode || ""});
};

_self.invokeFilePicker = function (options, done, cancel, invokeCallback) {
   /*
   * options = {
   *    mode: Picker or Saver or PickerMultiple or SaverMultiple, //one of them
   *    type:["Picture","Document","Music","Video","Other"], // , separated types but we pass array
   *    defaultType: "Picture"|"Document"|"Music"|"Video"|"Other"
   *    title: "some string",
   *    defaultSaveFileNames: ["fileName1","fileName2"], // , separated but we pass array
   *    directory:["/path/folder1","/path/folder2"], //, separated but we pass array
   *    filter:[".jpg",".bmp"], // , separated but we pass array
   *    viewMode:ListView or GridView or Default, // one of them
   *    sortBy:Default or Name or Date or Suffix or Size, //one of them
   *    sortOrder:Default or Ascending or Descending // one of them
   *    imageCrop: true|false,
   *    allowOverwrite: true|false
   * }
   */

    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeFilePicker", {options: options || ""});
};

_self.invokeIcsViewer = function (options, done, cancel, invokeCallback) {
    /*
    * options = {
    *     uri: path to the ICS file on device
    *     accountId: id of the calendar account to open the file in (optional)
    * }
    */

    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeIcsViewer", {options: options || ""});
};


_self.invokeCalendarPicker = function (options, done, cancel, invokeCallback) {
   /*
   * options = {
   *    filepath: path to file where .vcs will be saved
   * }
   */

    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeCalendarPicker", {options: options || ""});
};

_self.invokeCalendarComposer = function (options, done, cancel, invokeCallback) {
   /* options = {
    *    accountId : account ID //used with syncId or folderId to identify a specific account
    *    syncId : sync ID
    *    folderId : folder ID
    *    subject : event subject
    *    body : event body
    *    startTime : event start time e.g: Wed Jun 13 09:39:56 2012
    *    duration : event duration
    *    participants : array of pariticipant email addresses
    * }
    */

    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeCalendarComposer", {options: options || ""});
};

_self.invokeEmailComposer = function (options, done, cancel, invokeCallback) {
     /* options = {
      *    from : accountId this message should be sent from
      *    subject : message subject
      *    body : plaintext message body
      *    calendarevent : calendar event ID
      *    to : array of recipient emails
      *    cc : array of emails
      *    attachment : array of attachment filepaths
      * }
      */

    var callback = cb(done, cancel, invokeCallback);
    exec(callback, _noop, _ID, "invokeEmailComposer", {options: options || ""});
};

_self.invokeTargetPicker = function (request, title, onSuccess, onError) {

    var callback = function (data) {
        if (data.status === "success") {
            if (onSuccess && typeof(onSuccess) === "function") {
                onSuccess(data.result);
            }
        } else if (data.status === "error") {
            if (onError && typeof(onError) === "function") {
                onError(data.result);
            }
        }
    };

    try {
        if (request.hasOwnProperty('data')) {
            request.data = _btoa(request.data);
        }

        exec(callback, _noop, _ID, "invokeTargetPicker", {
            request: request,
            title: title
        });
    } catch (e) {
        onError(e);
    }
};

//CAMERA PROPERTIES
defineReadOnlyField(_self, "CAMERA_MODE_PHOTO", 'photo');
defineReadOnlyField(_self, "CAMERA_MODE_VIDEO", 'video');
defineReadOnlyField(_self, "CAMERA_MODE_FULL", 'full');

//FILE PICKER PROPERTIES
defineReadOnlyField(_self, "FILEPICKER_MODE_PICKER", 'Picker');
defineReadOnlyField(_self, "FILEPICKER_MODE_SAVER", 'Saver');
defineReadOnlyField(_self, "FILEPICKER_MODE_PICKER_MULTIPLE", 'PickerMultiple');
defineReadOnlyField(_self, "FILEPICKER_MODE_SAVER_MULTIPLE", 'SaverMultiple');

defineReadOnlyField(_self, "FILEPICKER_VIEWER_MODE_LIST", 'ListView');
defineReadOnlyField(_self, "FILEPICKER_VIEWER_MODE_GRID", 'GridView');
defineReadOnlyField(_self, "FILEPICKER_VIEWER_MODE_DEFAULT", 'Default');

defineReadOnlyField(_self, "FILEPICKER_SORT_BY_NAME", 'Name');
defineReadOnlyField(_self, "FILEPICKER_SORT_BY_DATE", 'Date');
defineReadOnlyField(_self, "FILEPICKER_SORT_BY_SUFFIX", 'Suffix');
defineReadOnlyField(_self, "FILEPICKER_SORT_BY_SIZE", 'Size');

defineReadOnlyField(_self, "FILEPICKER_SORT_ORDER_ASCENDING", 'Ascending');
defineReadOnlyField(_self, "FILEPICKER_SORT_ORDER_DESCENDING", 'Descending');

defineReadOnlyField(_self, "FILEPICKER_TYPE_PICTURE", 'picture');
defineReadOnlyField(_self, "FILEPICKER_TYPE_DOCUMENT", 'document');
defineReadOnlyField(_self, "FILEPICKER_TYPE_MUSIC", 'music');
defineReadOnlyField(_self, "FILEPICKER_TYPE_VIDEO", 'video');
defineReadOnlyField(_self, "FILEPICKER_TYPE_OTHER", 'other');

module.exports = _self;
