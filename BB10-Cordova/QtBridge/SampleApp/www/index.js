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

var app = {
    initialize: function () {
        this.bindEvents();
    },

    bindEvents: function () {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },

    onDeviceReady: function () {
        app.receivedEvent('deviceready');
    },

    receivedEvent: function (id) {
        console.log('Received Event: ' + id);
        var propertyTemplate = _.template(document.getElementById("propertyTemplate").innerHTML);
        
        var customizations = {
        	CellularDataInterface : function(root, cdi) {
        		cdi.name = "blackberry";
        	},
        	NetworkDataUsage : function (root, ndu) {
        		var hwinfo = Qt.createObject("bb.device.HardwareInfo");
        		var model = hwinfo.modelName;
        		hwinfo.destroy();
        		
        		if (model == "Z10")
        			ndu.networkInterfaceName = "tiw_sta0";
        		else
        			ndu.networkInterfaceName = "bcm0";
        		/*setInterval(function() {
        			ndu.update();
        		}, 2500);*/
        		ndu.update();
        	},
        	FileSystemInfo : function(root, fsi) {
        		var parentNode = root.querySelector("ul");
        		parentNode.insertAdjacentHTML("beforeend", propertyTemplate({
        			"name": "Root Filesystem Free",
        			"value": parseInt(100 * fsi.availableFileSystemSpace("/accounts/1000") / fsi.fileSystemCapacity("/accounts/1000")) + " %"
        		}));
        		
        		var childs = parentNode.querySelectorAll("li");
        		var propNode = childs.item(childs.length-1);
        		/*setInterval(function() {
        			propNode.innerHTML = "<span class='propertyName'>Root Filesystem Usage</span>:\t" + (parseInt(100 * fsi.availableFileSystemSpace("/accounts/1000") / fsi.fileSystemCapacity("/accounts/1000")) + " %");
        		}, 60000);*/
        	},
        	MemoryInfo : function(root, mi) {
        		var parentNode = root.querySelector("ul");
        		parentNode.insertAdjacentHTML("beforeend", propertyTemplate({
        			"name": "Memory Free",
        			"value": parseInt(mi.availableDeviceMemory()/1024/1024) + " MB"
        		}));
        		
        		var childs = parentNode.querySelectorAll("li");
        		var propNode = childs.item(childs.length-1);
        		/*setInterval(function() {
        			propNode.innerHTML = "Memory Free\t" + parseInt(mi.availableDeviceMemory()/1024/1024) + " MB";
        		}, 2500);*/
        		
        		parentNode.insertAdjacentHTML("beforeend", propertyTemplate({
        			"name": "Process Memory Usage",
        			"value": parseInt(mi.memoryUsedByCurrentProcess()/1024/1024) + " MB"
        		}));
        		
        		var childs = parentNode.querySelectorAll("li");
        		var propNode2 = childs.item(childs.length-1);
        		/*setInterval(function() {
        			propNode2.innerHTML = "Process Memory Usage\t" + parseInt(mi.memoryUsedByCurrentProcess()/1024/1024) + " MB";
        		}, 2500);*/
        	}
        };

		if (id === 'deviceready') {			
			if (window.namespaces === undefined)
				window.namespaces = [ "custom", "bb.device", "bb.platform", "bb" ];
			var ingnore = [
			    "RouteMapInvoker", "LocationMapInvoker", "NotificationDialog", "Notification"
			];
			var namespaces = window.namespaces;
			
			Qt.initialize();
			var objectTemplate = _.template(document.getElementById("objectTemplate").innerHTML);
			var signalTemplate = _.template(document.getElementById("signalTemplate").innerHTML);
			
			var connectNotifySignal = function(li, object, signalName, propertyName) {
	    		var signal = object[signalName];
	    		if (signal !== undefined) {
					var timeoutId = undefined;
	    			signal.connect(function(value) {
	    				var value2 = object[propertyName];
	    				if (value2 === undefined || value2 == "")
	    					value2 = value;
	    				console.log(signalName + "\t" + value2);
	    				li.innerHTML = "<span  style='color:#E6B400;'><span class='propertyName'>" + propertyName + "</span>:\t" + value2 + "</span>";
	    				clearTimeout(timeoutId);
	    				timeoutId = setTimeout(function() {
							li.innerHTML = "<span class='propertyName'>" + propertyName + "</span>:\t" + value2;
						}, 3000);
	    			});
	    		}
			};
			
			var connectSignal = function(li, object, signal) {
	    		var s = object[signal.name];
	    		if (s !== undefined) {
					var oldInner = li.innerHTML;
					var timeoutId = undefined;
		    		s.connect(function() {
	    				var value = Array.prototype.slice.apply(arguments);
	    				console.log(signal.name + "\t" + JSON.parse(value));
	    				
	    				li.innerHTML = signalTemplate({
	    					"signal" : signal,
	    					"params" : signal.parameters ? signal.parameters.map(function(v) { return v.name; }) : [],
	    					"args" : value
	    				});
	    				clearTimeout(timeoutId);
	    				timeoutId = setTimeout(function() {
							li.innerHTML = oldInner;
						}, 3000);
	    			});
	    		}
			};
			
			var enclosing = document.createElement("div");
			enclosing.classList.add("enclosing");
			document.body.appendChild(enclosing);
			
			var nsIndex = 0, nsLength = namespaces.length;
			
			var loadNs = function() {
				var namespace = namespaces[nsIndex++];
				var classList = Qt.listNamespace(namespace);
				for (var i in classList) {
					var cls = classList[i];
					if (ingnore.indexOf(cls) >= 0)
						continue;
					
					var qobject = Qt.createObject(namespace + "." + cls);
					window[cls] = qobject;
					var meta = qobject.introspect();
					
					var root = document.createElement("div");
			    	root.innerHTML = objectTemplate({
			    		"title": qobject.constructor.name,
			    		"object": qobject,
			    		"meta": meta,
			    	});
			    	
			    	/* bind property notifiers to property elements to update text */
			    	var listItems = root.querySelectorAll("li");
			    	var items = {};
			    	for (var i=0; i < listItems.length; ++i) {
			    		var li = listItems[i],
			    		    dataId = li.getAttribute("data-id");
			    		if (dataId)
			    			items[dataId] = li;
			    	}
			    	
			    	var notifySignals = [];
			    	var props = meta.properties ? meta.properties : [];
			    	for (var i = 0; i < props.length; ++i) {
			    		var prop = props[i];
			    		var notifySignal = prop.notifySignal;
			    		if (notifySignal !== undefined) {
			    			notifySignals.push(notifySignal);
			    			connectNotifySignal(items[prop.name], qobject, notifySignal, prop.name);
			    		}
			    	}
			    	
			    	var signals = meta.signals ? meta.signals : [];
					var xSignals = {};
					for (var i = 0; i < signals.length; i++) {
						var signal = signals[i];
						var params = signal.parameters ? signal.parameters : [];
						if (notifySignals.indexOf(signal.name) >= 0 && 
								xSignals[signal.name] == undefined)
							continue;
						connectSignal(items["signal-" + signal.name], qobject, signal);
					}
			    	
			    	if (customizations[cls] !== undefined) {
			    		customizations[cls](root, qobject);
			    	}
			    	
			    	enclosing.appendChild(root);
				}
				
				if (nsIndex < nsLength)
					setTimeout(loadNs, 0);
			};
			
			setTimeout(loadNs, 0);
		}
    }
};
