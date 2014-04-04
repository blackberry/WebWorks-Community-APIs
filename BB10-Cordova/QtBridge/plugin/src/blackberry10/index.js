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

var _ID = "com.blackberry.qt",
	asyncResults = {};

var formatInvoke = function(jnextMessage) {
	var json = JSON.parse(jnextMessage);
	return ("<" + json[1] + ">." + json[0] + " " + json.slice(2)); 
};

var Qt = function() {
	var self = this, hasInstance = false;

	self.getId = function() {
		return self.m_id;
	};

	self.init = function(success, fail, args, env) {
		//console.log("[index.init]");
		var pr = new PluginResult(args, env);
		if (!hasInstance) {
			if (!JNEXT.require("libQtBridge"))
				return pr.error("__create__ require libQtBridge falied", false);
	
			self.m_id = JNEXT.createObject("libQtBridge.Qt");
			if (self.m_id === "")
				return pr.error("__create__ createObject(libQtBridge.Qt) falied", false);
			hasInstance = true;
	
			JNEXT.registerEvents(self);
			
			var views = qnx.webplatform.getWebViews()
			var windowGroup = null;
            var z = -1;
            for (var i = 0; i < views.length; i++) {
                    if (views[i].visible && views[i].zOrder > z){
                            z = views[i].zOrder;
                            windowGroup = views[i].windowGroup;
                    }
            }
			var x = JNEXT.invoke(self.m_id, JSON.stringify([
				"join", windowGroup
			]));
			console.log("[index.init]\t" + x);
		}
		pr.noResult(false);
	};
	
	self.dispose = function(success, fail, args, env) {
		var pr = new PluginResult(args, env);
		if (hasInstance) {
			for (var pluginResults in asyncResults) {
				for(var sid in pluginResults) {
					var pluginResult = pluginResults[sid];
					console.log(pluginResult);
					if (pluginResult)
						pluginResult.callbackError("__disconnect__", false);
				}
			}
			
			JNEXT.invoke(self.m_id, "Dispose");
			self.m_id = undefined;
			hasInstance = false;

			JNEXT.unregisterEvents(self);
		}
		pr.noResult(false);
	};

	self.invoke = function (success, fail, args, env) {
		var pr = new PluginResult(args, env);
		var jnextMessage = decodeURIComponent(args["message"]);
		
		console.log("[index.invoke]          -->   " + formatInvoke(jnextMessage));
		var value = JNEXT.invoke(self.m_id, jnextMessage);
		console.log("[index.invoke]          <--   " + value.slice(0,80).replace(/\n/gm,""));
		
		pr.ok(value,false);
	};
	
	self.deleteObject = function(success, fail, args, env) {
		try {
			var message = JSON.parse(decodeURIComponent(args["message"]));
			var id = message[1];
			
			var pluginResults = asyncResults[id];
			for(var sid in pluginResults) {
				var pluginResult = pluginResults[sid];
				console.log(pluginResult);
				if (pluginResult)
					pluginResult.callbackError("__disconnect__", false);
			}
			delete asyncResults[id];
			
			self.invoke.call(this, success, fail, args, env);
		} catch (e) {
			console.log(e);
		}
	};
	
	self.createObject = function(success, fail, args, env) {
		try {
			var pr = new PluginResult(args, env);
			var jnextMessage = JSON.parse(decodeURIComponent(args["message"]));
			jnextMessage.unshift("createObject");
			
			console.log("[index.createObject]    -->   " + jnextMessage);
			var objectId = JNEXT.invoke(self.m_id, JSON.stringify(jnextMessage));
			var objectData = JNEXT.invoke(self.m_id, JSON.stringify(["introspect", objectId]));
			console.log("[index.createObject]    <--   <" + objectId + "> {" + jnextMessage[1] + " ...}");
			
			pr.ok(objectData,false);
		} catch (e) {
			console.log(e);
		}
	};
	
	self.connect = function (success, fail, args, env) {
		var result = new PluginResult(args, env),
		    message = JSON.parse(decodeURIComponent(args["message"])),
		    sid = result.callbackId.replace(_ID, ""),
		    id = message[1];

		if (asyncResults[id] === undefined)
			asyncResults[id] = [];
		asyncResults[id][sid] = result;
		result.signal = message[2];
		message.push(sid);	
		var jnextMessage = JSON.stringify(message);	
		
		// subscribe before c++ does
		result.error(sid, true);
		var value = JNEXT.invoke(self.m_id, jnextMessage);
		console.log("[index.connect]         ++>   <" + id + ">." + message[2]);
	};
	
	self.disconnect = function (success, fail, args, env) {
		try {
			var message = JSON.parse(decodeURIComponent(args["message"])),
			    sid = message[2],
			    id = message[1];
			
			var connectResult = asyncResults[id];
			if (connectResult)
			    connectResult = connectResult[sid];
	
			var jnextMessage = JSON.stringify(message);
			if (connectResult) {
				connectResult.callbackError("__disconnect__", false);
				delete asyncResults[id][sid];
				var value = JNEXT.invoke(self.m_id, jnextMessage);
				
				console.log("[index.disconnect]      xx>   <" + id + ">." + connectResult.signal);
			} else
				var value = "";

			var result = new PluginResult(args, env);
	        result.ok(value,false);        
		} catch (e) {
			console.log(e);
		}
	};
	
	self.onEvent = function(strData) {
		// Event names are set in native code when fired,
		// and must be checked here.
		var arData = strData.split(" "),
			callbackId = arData[0].replace(_ID, ""),
			jsonData = arData.slice(1, arData.length).join(" ");

		try {
			jsonData = JSON.parse(jsonData);
		} catch (e) {
			jsonData = {};
		}
		
		var id = jsonData.shift();
		if (id) {
			result = asyncResults[id][callbackId];
			if (result) {
				result.callbackOk(jsonData, true);
				console.log("[index.onEvent]         <++   <" + id + ">." + result.signal + "\t" + jsonData);
			} else {
				// TODO : implement
				//self.deleteObject()
				JNEXT.invoke(self.m_id, ["deleteObject", ]);
			}
		}
	};

	// ************************
	// End of methods to edit
	// ************************
	self.m_id = "";
};

JNEXT.Qt = new Qt();

module.exports = {

	invoke : function(success, fail, args, env) {
		JNEXT.Qt.invoke.call(this, success, fail, args, env);
	},
	
	connect : function(success, fail, args, env) {
		JNEXT.Qt.connect.call(this, success, fail, args, env);
	},
	
	disconnect : function(success, fail, args, env) {
		JNEXT.Qt.disconnect.call(this, success, fail, args, env);
	},
	
	createObject : function(success, fail, args, env) {
		JNEXT.Qt.createObject.call(this, success, fail, args, env);
	},
	
	deleteObject : function(success, fail, args, env) {
		JNEXT.Qt.deleteObject.call(this, success, fail, args, env);
	},

	init : function(success, fail, args, env) {
		try {
			JNEXT.Qt.init.call(this, success, fail, args, env);
		} catch (e) {
			console.log(e);
			(new PluginResult(args, env)).error(e, false);
		}
	},
	
	dispose : function(success, fail, args, env) {
		try {
			JNEXT.Qt.dispose.call(this, success, fail, args, env);
		} catch (e) {
			console.log(e);
			(new PluginResult(args, env)).error(e, false);
		}
	}

};
