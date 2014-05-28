/*
* Copyright 2013-2014 BlackBerry Limited.
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
_ID = require("./manifest.json").namespace;

_self.start = function () {
	window.webworks.event.add(_ID, "community.joypad.eventCallback", _self.eventHandler);
	return window.webworks.execSync(_ID, "start", null);
};

_self.stop = function () {
	return window.webworks.execSync(_ID, "stop", null);
};

_self.button = {
	BUTTON_A     : (1 << 0),
	BUTTON_B     : (1 << 1),
	BUTTON_C     : (1 << 2),
	BUTTON_X     : (1 << 3),
	BUTTON_Y     : (1 << 4),
	BUTTON_Z     : (1 << 5),
	BUTTON_MENU1 : (1 << 6),
	BUTTON_MENU2 : (1 << 7),
	BUTTON_MENU3 : (1 << 8),
	BUTTON_MENU4 : (1 << 9),
	BUTTON_L1    : (1 << 10),
	BUTTON_L2    : (1 << 11),
	BUTTON_L3    : (1 << 12),
	BUTTON_R1    : (1 << 13),
	BUTTON_R2    : (1 << 14),
	BUTTON_R3    : (1 << 15),
	DPAD_UP      : (1 << 16),
	DPAD_DOWN    : (1 << 17),
	DPAD_LEFT    : (1 << 18),
	DPAD_RIGHT   : (1 << 19)
};

_self.analog = {
	X: 0,
	Y: 1,
	Z: 2
};

_self.type = {
	BUTTON:  0,
	ANALOG0: 1,
	ANALOG1: 2
};


// HTML5 spec http://www.w3.org/TR/2014/WD-gamepad-20140225/#widl-Navigator-getGamepads-Gamepad
_self.getGamepads = function() {
	return gamepads;
};

var gamepads = [];

_self.eventHandler = function(event) {

	var gamepad = gamepads[event.ctrl];
	if (!gamepad) {
		gamepad = {};
		gamepads.push(gamepad);
	}
	gamepad.connected = true;
	gamepad.index = event.ctrl;
	gamepad.buttons = event.buttons;
	gamepad.axes = event.axes;
}


// setup as a shim for the HTML5 spec
navigator.getGamepads = navigator.getGamepads || _self.getGamepads;
module.exports = _self;
