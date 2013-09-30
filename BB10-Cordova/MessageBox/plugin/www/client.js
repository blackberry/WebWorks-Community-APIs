(function(cordova) {
    var exec = cordova.require("cordova/exec"),
        _ID = "org.apache.cordova.MessageBox";

    function MessageBox() {}

    MessageBox.prototype.defaults = {
        okButtonTitle: 'OK',
        yesButtonTitle: 'Yes',
        noButtonTitle: 'No',
        cancelButtonTitle: 'Cancel'
    };

    MessageBox.prototype.alert = function(options, callback) {
        options || (options = {});
        var scope = options.scope || null;

        var config = {
            title: options.title || '',
            message: options.message || '',
            okButtonTitle: options.okButtonTitle || this.defaults.okButtonTitle
        };

        var _callback = function(buttonIndex) {
            var button = 'ok';
            if(typeof callback == 'function') callback.call(scope, button);
        };

        navigator.notification.alert(config.message, _callback, config.title, config.okButtonTitle + '');
    };

    MessageBox.prototype.confirm = function(options, callback) {
        options || (options = {});
        var scope = options.scope || null;

        var config = {
            title: options.title || '',
            message: options.message || '',
            yesButtonTitle: options.yesButtonTitle || this.defaults.yesButtonTitle,
            noButtonTitle: options.noButtonTitle || this.defaults.noButtonTitle
        };

        var _callback = function(buttonIndex) {
            var button = (buttonIndex === 2) ? 'yes' : 'no';
            if(typeof callback == 'function') callback.call(scope, button);
        };

        navigator.notification.confirm(config.message, _callback, config.title, [config.noButtonTitle + ', ' + config.yesButtonTitle]);
    };
    
    MessageBox.prototype.prompt = function(options, callback) {
        options || (options = {});
        var scope = options.scope || null;

        var config = [
            options.message || '',
            options.title || '',
            options.type || 'text',
            "Prompt Dialog",
            options.okButtonTitle || this.defaults.okButtonTitle,
            options.cancelButtonTitle || this.defaults.cancelButtonTitle
        ];

        var _callback = function(result) {
            var value = result.input1 || "";
            var button = (result.buttonIndex == 1) ? 'ok' : 'cancel';
            if(typeof callback == 'function') callback.call(scope, button, value);
        }

        var cordova_callback = function(result) {
            navigator.notification.prompt(config.message, _callback, config.title, config.noButtonTitle + ', ' + config.yesButtonTitle, "Prompt dialog");
        }

        exec(cordova_callback, null, _ID, 'prompt', config);
    };

    cordova.addConstructor(function() {
        if(!window.plugins) window.plugins = {};
        window.plugins.messageBox = new MessageBox();
    });

})(window.cordova || window.Cordova);