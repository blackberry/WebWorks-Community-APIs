function testNamespace() {
    alert("Testing Namespaces.");

    try {
        alert("BB ns: " + blackberry);
        alert("blackberry.media ns: " + blackberry.media);
        alert("blackberry.media.barcode ns: " + blackberry.media.barcode);
    } catch (e) {
        alert("Error with namespace: " + e);
    }
}

function decodeBarcode() {
    alert("Decoding Barcode..");

    try {
        blackberry.media.barcode.scan(function successCB(resultObj) {
            alert("Barcode scanning succeed.");

            if(resultObj.code === 0) {
                alert("Content: " + resultObj.data.content);
            }

        }, function errorCB(errorObj) {
            alert("Error occured.");
            if (errorObj.code === -1) {
                alert("Error info: " + errorObj.code + ", " + errorObj.msg);
            }

        }, {
            'tryHarder' : false,
            'format' : ['EAN_13', 'QR']
        });
    } catch(e) {
        alert("Error occured when scanning barcode: " + e);
    }
}

function encodeBarcode() {
    alert("Encoding Barcode..");
    var cameraDirFilePath = blackberry.io.dir.appDirs.shared.camera.path + "/qrImageEncode.png";
    alert('Barcode folder path:' + cameraDirFilePath);

    try {
        blackberry.media.barcode.generate("PlayBook Barcode App!", function successCB(resultObj) {
            alert("Barcode generation succeed.");

            if(resultObj.code === 0) {
                alert("File Path: " + resultObj.data.filePath);
            }
        }
        , function errorCB(errorObj) {
            alert("Error occured.");
            if (errorObj.code === -1) {
                alert("Error info: " + errorObj.code + ", " + errorObj.msg);
            }

        }, {
            'format' : 'qr',
            'width' : 86,
            'height' : 86,
            'characterSet' : 'UTF-8',
            'filePath' : cameraDirFilePath
        });
    } catch(e) {
        alert("Error occured when generating barcode: " + e);
    }
}