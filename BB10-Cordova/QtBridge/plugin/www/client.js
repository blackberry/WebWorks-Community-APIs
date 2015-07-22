/*
 * Copyright (c) 2013 BlackBerry Limited
 * Author: Isaac Gordezky
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

var _ID = "cordova-plugin-qtbridge",
	exec = cordova.require("cordova/exec");

var getArgs = function(args) {
	return Array.prototype.slice.apply(args);
};

var signals = {};

var static = {

	connect : function(id, signal, slot) {
		var instance = this;
		if (slot[" qtbridge sid "] === undefined)
			slot[" qtbridge sid "] = {};
		//console.log("[client.connect]\tobject<" + id + "> " + signal + "\t" + slot);

		var sid = undefined;
		var success = function (data) {
			//console.log("[SIGNAL] " + signal + "\t" + data);
			result = slot.apply(instance, data);
		},
		fail = function (data) {
			// handle connect here, so hotpath is smaller
			if (sid === undefined) {
				sid = data.replace(_ID, "");
				try {
					sid = parseInt(sid);
				} catch (err) {
					console.log("[client.connect]\tERROR\t" + err);
					throw err;
				}
				return;
			} else if (data == "__disconnect__") {
				delete slot[" qtbridge sid "][sid];
				return;
			}
			//console.log("[client.connect]\tERROR: " + data);
		};
		exec(success, fail, _ID, "connect", { message: ["connect", id, signal ] });
		
		slot[" qtbridge sid "][sid] = {
			id : id,
			signal : signal
		};
		//console.log("[client.connect]\t" + sid);
		return sid;
	},

	disconnect : function(slot, id, signal) {
		var result = "false";
		var sids = slot[" qtbridge sid "];
		var handler = function (data) {
			result = data;
		};
		
		if (sids == undefined || Object.keys(sids).length == 0) {
			sid = parseInt(slot, 10);
			if ((""+sid) == "NaN" || sid <= 0) {
				console.warn("slot is not currently connected");
				return false;
			}
			exec(handler, handler, _ID, "disconnect", { message: ["disconnect", id, sid ] });
			//console.log("[client.disconnect]\t" + id + " " + sid);
		} else {
			for (var sid in sids) {
				var meta = sids[sid];
				if (signal == undefined || (meta.id == id && (meta.signal == signal || meta.signal == undefined))) {
					exec(handler, handler, _ID, "disconnect", { message: ["disconnect", id, sid ] });
					//console.log("[client.disconnect]\t" + id + " " + sid);
					delete sids[sid];
				}
			}
		}
		if (result != "")
			console.warn(result);
		return (result == "");
	},

	invoke : function(message) {
		var result = undefined, error = undefined,
		success = function (data) {
			result = data;
			//console.log("[client.invoke.success]\t" + data);
		},
		fail = function (data) {
			console.log("[client.invoke.error]\t" + data);
			error = data;
		};
		exec(success, fail, _ID, "invoke", {
			"message" : message
		});
		if (error !== undefined)
			throw error;
		if (result == "true")
			result = true;
		else if (result == "false")
			result = false;
		return result;
	},
	
	deleteObject : function(message) {
		var result = undefined,
		handler = function (data) {
			result = data == "" ? undefined : data;
		};
		exec(handler, handler, _ID, "deleteObject", {
			"message" : message
		});
		return result;
	},
	
	createObject : function(message) {
		var result = undefined,
		handler = function (data) {
			result = data;
		};
		exec(handler, handler, _ID, "createObject", {
			"message" : message
		});
		return result;
	}
	
};

var initialized = false;

var defineGetFunction = function(id, name) {
	return function() {
		return static.invoke(['get', id, name]);
	};
};

var defineSetFunction = function(id, name) {
	return function(value) {
		return static.invoke(['set', id, name, value]);
	};
};

var defineCallFunction = function(id, name) {
	return function() {
		return static.invoke(['call', id, name].concat(Array.prototype.slice.apply(arguments)));
	};
};

var createObject = function() {
	var sid = 0;
	var metadata = static.createObject(getArgs(arguments));
	try {
		metadata = JSON.parse(metadata);
	} catch (e) {
		throw (metadata);
	}
	var id = metadata["id"];
	if (isNaN(id - 0))
		throw id;
	var friendlyName = arguments[0].split("@")[0];
	initialized = true;

	var metaProperties = {

		/*__id__ : {
		 value : parseInt(id),
		 },*/

		constructor : {
			value : Object.create(null, {
				name : {
					enumerable : true,
					value : friendlyName
				}
			})
		},

		introspect : {
			value : function() {
				return JSON.parse(static.invoke(["introspect", id]));
			}
		},
		
		disconnect : {
			value : function(slot) {
				return static.disconnect(slot, id);
			}
		},
		
		connect : {
			value : function(signal, slot) {
				return static.connect(id, signal, slot);
			}
		},

		destroy : {
			value : function() {
				return static.deleteObject(["deleteObject", id]);
			}
		},
		
		/*deleteLater : {
			value : function() {
				return static.invoke(["deleteObject", id]);
			}
		},*/

	};

	//metadata = JSON.parse(static.invoke(["introspect", id]));
	if (metadata.methods) {
		for (var i = 0; i < metadata.methods.length; ++i) {
			var name = metadata.methods[i].name;
			metaProperties[name] = {
				value : defineCallFunction(id, name)
			};
		}
	}

	var createDefinition = function(instance, name, signature) {
		var def = {
			enumerable : false,
			configurable : false,
			value : Object.create(null, {
				constructor : {
					enumerable : false,
					value : Object.create(null, {
						name : {
							enumerable : true,
							value : signature
						}
					})
				},
				connect : {
					value : function(slot) {
						return static.connect.call(instance, id, name, slot);
					}
				},
				disconnect : {
					value : function(slot) {
						return static.disconnect.call(instance, slot, id, name);
					}
				},
			})
		};
		return def;
	};

	if (metadata.properties) {
		for (var i = 0; i < metadata.properties.length; ++i) {
			var name = metadata.properties[i].name;
			var definition = {
				enumerable : true,
				configurable : false,
				get : defineGetFunction(id, name),
				set : metadata.properties[i].readOnly ? undefined : defineSetFunction(id, name),
			};

			metaProperties[name] = definition;
		}
	}
	
	var metaSignals = {};
	if (metadata.signals) {
		for (var i = 0; i < metadata.signals.length; ++i) {
			var name = metadata.signals[i].name;
			// Qt can have multiple signals with the same name, we can't
			if (metaSignals[name])
				continue;

			var signature = friendlyName + "." + name + "(";
			var parameters = metadata.signals[i].parameters;
			var plist = [];
			for (var j in parameters) {
				plist.push(parameters[j].type + " " + parameters[j].name);
			}
			signature += plist.join(", ") + ")";

			if (name in metaProperties)
				name = "on" + name[0].toUpperCase() + name.slice(1);
			metaSignals[name] = createDefinition(instance, name, signature);
		}
	}

	var instance = Object.create(null, metaProperties);
	Object.defineProperties(instance, metaSignals);
	Object.freeze(instance);

	return instance;
};

var init = function() {
	if (initialized == true)
		return;
	initialized = true;
	var result = true,
	success = function (data) {
		console.log("[client.init]\tsuccess\t" + data);
	},
	fail = function (data) {
		result = false;
		console.log("[client.init]\tError: " + data);
	};
	exec(success, fail, _ID, "init", null);
	return result;
};

var dispose = function() {
	if (initialized != true)
		return;
	initialized = false;
	var result,
	success = function (data) {
		console.log("[client.dispose]\tsuccess\t" + data);
	},
	fail = function (data) {
		console.log("[client.dispose]\tError: " + data);
	};
	exec(success, fail, _ID, "dispose", null);
	return result;
};

var clientDescriptor = {
	initialized : {
		enumerable : true,
		get : function() {
			return initialized;
		}
	},
	
	listNamespace : {
		enumerable : true, 
		value : function(ns) {
			if (!initialized)
				init.apply(this, arguments);
			
			var result = {};
			var success = function(data) {
				try {
					result = JSON.parse(data);
				} catch (e) {
					throw (data);
				}
			}, fail = function(data) {
				result = data;
			};
			exec(success, fail, _ID, "invoke", {
				"message" : ["listNamespace", ns]
			});
			return result;
		}
	},

	initialize : {
		enumerable : true,
		value : function() {
			return init.apply(this, arguments);
		}
	},

	terminate : {
		enumerable : true,
		value : function() {
			return dispose.apply(this, arguments);
		}
	},

	createObject : {
		enumerable : true,
		value : function() {
			if (!initialized)
				init.apply(this, arguments);
			return createObject.apply(this, arguments);
		}
	},
	
	connect : {
		enumerable : true,
		value : function(object, signal, slot) {
			var signal = object["signal"];
			if (signal && signal.connect)
				signal.connect(slot);
		}
	},
	
	disconnect : {
		enumerable : true,
		value : function(slot) {
			static.disconnect(slot);
		}
	}
	
};

var client = Object.create(null, clientDescriptor);
Object.defineProperty(client, "__proto__", {
	value : null
});
Object.freeze(client);

module.exports = client;
