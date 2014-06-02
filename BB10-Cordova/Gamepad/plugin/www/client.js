/*
* Copyright (c) 2014 BlackBerry Limited
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

var _self = {},
	_ID = "com.blackberry.community.joypad",
	exec = cordova.require("cordova/exec");

	

_self.start = function (callback) {
	var success = function (data, response) {
			callback(data);
		},
		fail = function (data, response) {
			console.log("Error: " + data);
		};
	exec(success, fail, _ID, "start", null);
};

_self.stop= function () {
	var result,
		success = function (data, response) {
			result = data;
		},
		fail = function (data, response) {
			console.log("Error: " + data);
		};
	exec(success, fail, _ID, "stop", null);
	return result;
};

// HTML5 spec http://www.w3.org/TR/2014/WD-gamepad-20140225/#widl-Navigator-getGamepads-Gamepad
_self.getGamepads = function() {
	return gamepads;
};

var gamepads = [];

eventHandler = function(event) {
	var gamepad = gamepads[event.ctrl];
	if (!gamepad) {
		gamepad = {};
		gamepads.push(gamepad);
	}
	gamepad.index = event.ctrl;
	gamepad.buttons = event.buttons;
	gamepad.axes = event.axes;
	gamepad.mapping = event.mapping;
	gamepad.id = event.id;
	gamepad.connected = true;
	if (gamepad.timestamp >= 0) {
		gamepad.timestamp += 1;
	} else {
		gamepad.timestamp = 0;
	}
	switch (event.type) {
		case 0:
			break;
		case 1:
			// fire gamepadconnected event
			dispatchGamepadEvent("gamepadconnected", gamepad);
			break;
		case 2:
			// fire gamepaddisconnected event
			gamepad.connected = false;
			dispatchGamepadEvent("gamepaddisconnected", gamepad);
			break;
	}
};

dispatchGamepadEvent = function(type, gamepad) {
	var event = new CustomEvent(type);
	event.gamepad = gamepad;
	window.dispatchEvent(event);
};

if (!navigator.getGamepads) {
	// only start if the HTML5 API is not present
	var startup_messages = JSON.parse(_self.start());
	if (startup_messages.error) {
		console.log(startup_messages);
	}
}

// setup as a shim for the HTML5 spec
navigator.getGamepads = navigator.getGamepads || _self.getGamepads;

module.exports = _self;