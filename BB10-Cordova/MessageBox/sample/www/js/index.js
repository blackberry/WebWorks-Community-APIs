/*
 * copyright Olivier Louvignes 2012, under the MIT License.
 */
var app = {
	options: {
		alert: {
			title: 'Title',
			message: 'Message'
		},
		prompt: {
			title: 'Title',
			message: 'Message'
		},
		prompt_password: {
			title: 'Title',
			message: 'Message',
			type: 'password'
		},
		confirm: {
			title: 'Title',
			message: 'Message'
		}
	},

	// Application Constructor
	initialize: function() {
		this.bindEvents();
	},
	bindEvents: function() {
		document.addEventListener('deviceready', this.onDeviceReady, false);
	},
	onDeviceReady: function() {
		app.receivedEvent('deviceready');
	},
	receivedEvent: function(id) {
		var btns = {
			alert : document.getElementById("messagebox_alert"),
			prompt : document.getElementById("messagebox_prompt"),
			prompt_password : document.getElementById("messagebox_prompt_password"),
			confirm : document.getElementById("messagebox_confirm")
		}

		for(var btn in btns) {
			btns[btn].addEventListener("click", this.messagebox_btnClick, false)
		}
	},

	messagebox_btnClick: function(evt) {
		evt.preventDefault();
		var btn_id_prepend = "messagebox_";
		var messageBox = window.plugins.messageBox
		var fn_name = evt.target.id.substring(btn_id_prepend.length);

		var cb = function(return_val1, return_val2) {
			var results = document.getElementById("results");
			var results_cnt = "";

			results_cnt += "return_val1: " + (return_val1 != '' ? return_val1 : "") + "<br />";
			results_cnt += "return_val2: " + (return_val2 != '' ? return_val2 : "");

			results.innerHTML = results_cnt;
		}

		messageBox[(fn_name == "prompt_password" ? "prompt" : fn_name)](app.options[fn_name], cb);
	}
};
