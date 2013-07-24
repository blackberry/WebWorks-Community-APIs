/*
* Copyright 2010-2011 Research In Motion Limited.
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

package webworks.media.barcode
{

    import com.google.zxing.BarcodeFormat;
    import com.google.zxing.BinaryBitmap;
    import com.google.zxing.BufferedImageLuminanceSource;
    import com.google.zxing.DecodeHintType;
    import com.google.zxing.EncodeHintType;
    import com.google.zxing.MultiFormatReader;
    import com.google.zxing.MultiFormatWriter;
    import com.google.zxing.Result;
    import com.google.zxing.client.result.ParsedResult;
    import com.google.zxing.client.result.ResultParser;
    import com.google.zxing.common.ByteMatrix;
    import com.google.zxing.common.GlobalHistogramBinarizer;
    import com.google.zxing.common.flexdatatypes.HashTable;
    
    import flash.display.Bitmap;
    import flash.display.BitmapData;
    import flash.display.Loader;
    import flash.events.ErrorEvent;
    import flash.events.Event;
    import flash.events.IOErrorEvent;
    import flash.events.MediaEvent;
    import flash.filesystem.File;
    import flash.filesystem.FileMode;
    import flash.filesystem.FileStream;
    import flash.media.CameraUI;
    import flash.media.MediaPromise;
    import flash.media.MediaType;
    import flash.utils.ByteArray;
    
    import json.JSON;
    import json.JSONEncoder;
    
    import mx.graphics.codec.PNGEncoder;
    
    import webworks.extension.DefaultExtension;
    import webworks.extension.WebWorksReturnValue;

    public class Barcode extends DefaultExtension
    {
        private const SUPPORTED_FILE_TYPES:Array = new Array("png");
        private const DEFAULT_FILE_TYPE:String = "png";
		private const BARCODE_GENERAL_ERROR_CODE:Number = -1;

        public function Barcode()
        {
        }

        override public function getFeatureList():Array
        {
            return new Array("webworks.media.barcode");
        }

        public function scan(jsOnCaptured:String, jsOnError:String = "", options:String = "{}"):void
        {
            var scanData:Object = JSON.decode(options, true);
            scanData.jsOnCaptured = jsOnCaptured;
            scanData.jsOnError = jsOnError;
            scanData.filePath = "";
			scanData.resultObj = createResultObject();
            scanData.htDecodeHints = getAllDecodeHints(scanData.tryHarder, scanData.format);

            invokeCamera(scanData);
        }

        private function getAllDecodeHints(tryHarder:Boolean, barcodeTypeArr:Array):HashTable
        {
            var ht:HashTable = new HashTable();

            for (var barcodeType:String in barcodeTypeArr)
            {
                switch (barcodeTypeArr[barcodeType].toString().toUpperCase())
                {
                    case "1d": //1D Barcode
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.UPC_E);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.UPC_A);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.EAN_8);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.EAN_13);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.CODE_39);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.CODE_128);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.PDF417);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.ITF);
                        break;

                    case "2d": //2D Barcode 
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.QR_CODE);
                        ht.Add(DecodeHintType.POSSIBLE_FORMATS, BarcodeFormat.DATAMATRIX);
                        break;
                    default:
                        if (ht.getKeyByValue(barcodeTypeArr[barcodeType]) == -1)
                        {
                            ht.Add(DecodeHintType.POSSIBLE_FORMATS, getMappedBarcodeFormatObject(barcodeTypeArr[barcodeType]).toString());
                        }
                        break;
                }
            }

            if (tryHarder)
            {
                ht.Add(DecodeHintType.TRY_HARDER, tryHarder);
            }

            return ht;
        }

        private function invokeCamera(scanData:Object):void
        {
            var defaultDeviceCamera:CameraUI = new CameraUI();
            defaultDeviceCamera.addEventListener(MediaEvent.COMPLETE, function onBarcodeCaptureComplete(event:MediaEvent):void
            {
                var imagePromise:MediaPromise = event.data;

                scanData.filePath = getSharedDirURL(imagePromise.relativePath);
                scanData.barcodeLoader = new Loader();

                if (imagePromise.isAsync)
                {
                    scanData.barcodeLoader.contentLoaderInfo.addEventListener(Event.COMPLETE, function asyncImageLoaded(event:Event):void
                    {
                        loadingBarcodeCompleted(scanData);
                    });
                    scanData.barcodeLoader.addEventListener(IOErrorEvent.IO_ERROR, function onBarcodeLoadingError(error:ErrorEvent):void
                    {
						scanData.resultObj.code = BARCODE_GENERAL_ERROR_CODE;
						scanData.resultObj.msg = error.toString();
						
                        this.evalJavaScriptEvent(scanData.jsOnError, [scanData.resultObj]);
                    });

                    scanData.barcodeLoader.loadFilePromise(imagePromise);
                }
                else
                {
                    scanData.barcodeLoader.loadFilePromise(imagePromise);
                    loadingBarcodeCompleted(scanData);
                }

            });
            defaultDeviceCamera.addEventListener(ErrorEvent.ERROR, function onBarcodeCaptureError(error:ErrorEvent):void
            {
				scanData.resultObj.code = BARCODE_GENERAL_ERROR_CODE;
				scanData.resultObj.msg = error.toString();

                this.evalJavaScriptEvent(scanData.jsOnError, [scanData.resultObj]);
            });

            defaultDeviceCamera.launch(MediaType.IMAGE);
        }

        private function loadingBarcodeCompleted(scanData:Object):void
        {
            var bitmap:Bitmap = scanData.barcodeLoader.content as Bitmap;
            decodeBitmapData(bitmap.bitmapData, bitmap.width, bitmap.height, scanData);
        }

        private function decodeBitmapData(bmpd:BitmapData, width:int, height:int, scanData:Object):void
        {
            var errorMsg:String = null;
            // create the container to store the image data in
            var lsource:BufferedImageLuminanceSource = new BufferedImageLuminanceSource(bmpd);
            // convert it to a binary bitmap
            var bitmap:BinaryBitmap = new BinaryBitmap(new GlobalHistogramBinarizer(lsource));
            // get all the hints
            var res:Result = null;
            try
            {
                // try to decode the image
                res = (new MultiFormatReader).decode(bitmap, scanData.htDecodeHints);
            }
            catch (e:Error)
            {
                errorMsg = e.message;
            }
            finally
            {
                try
                {
					// Delete barcode file from the file system after being decoded.
                    var file:File = new File(scanData.filePath);
                    if (file.exists)
                    {
                        file.deleteFile();
                    }
                }
                catch (e:Error)
                {
                }
            }

            // did we find something?
            if (res == null)
            {
				scanData.resultObj.code = BARCODE_GENERAL_ERROR_CODE;
				scanData.resultObj.msg = (errorMsg ? errorMsg : "Can not decode the barcode.") + (scanData.tryHarder ? "" : " " + "Retry with the 'Try Harder'.");

                this.evalJavaScriptEvent(scanData.jsOnError, [scanData.resultObj]);
            }
            else
            {
                // yes : parse the result
                var parsedResult:ParsedResult = ResultParser.parseResult(res);
                // get a formatted string
				scanData.resultObj.data["content"] = parsedResult.getDisplayResult();
				
                this.evalJavaScriptEvent(scanData.jsOnCaptured, [scanData.resultObj]);
            }
        }

        public function generate(contents:String, onGenerated:String, jsOnError:String = "", options:String = "{}"):void
        {
            try
            {
                var errorMsg:String = null;
                var generateData:Object = JSON.decode(options, true);
                generateData.onGenerated = onGenerated;
                generateData.jsOnError = jsOnError;
				generateData.resultObj = createResultObject();
                generateData.htEncodeHints = getAllEncodeHints(generateData.characterSet, generateData.errorCorrection);
                generateData.filePath = getValidFilePath(generateData.filePath);

                var imageByteArray:ByteArray = generateBarcode(contents, generateData);
                var barcodeFile:File = new File(generateData.filePath);

                saveBarcodeImageToFile(barcodeFile, imageByteArray);
            }
            catch (e:Error)
            {
                errorMsg = e.message;
            }

            if (errorMsg == null && isBarcodeFileExists(generateData.filePath))
            {
				generateData.resultObj.data["filePath"] = generateData.filePath;
				
                this.evalJavaScriptEvent(generateData.onGenerated, [generateData.resultObj]);
            }
            else
            {
				generateData.resultObj.code = BARCODE_GENERAL_ERROR_CODE;
				generateData.resultObj.msg = "Could not create an output file:" + (errorMsg ? " " + errorMsg : "");

                this.evalJavaScriptEvent(generateData.jsOnError, [generateData.resultObj]);
            }
        }

        private function getAllEncodeHints(characterSet:String, errorCorrection:int):HashTable
        {
            var ht:HashTable = new HashTable();

            if (characterSet != null && characterSet.length > 0)
            {
                ht.Add(EncodeHintType.CHARACTER_SET, characterSet);
            }

            return ht;
        }

        private function getValidFilePath(filePath:String):String
        {
            if (filePath)
            {
                if (isFileTypeValid(filePath))
                {
                    return filePath;
                }
            }

            return getSharedDirURL(new Date().getTime().toString() + "." + DEFAULT_FILE_TYPE);
        }

        private function getSharedDirURL(fileName:String):String
        {
            var sharedDirURL:String = "";
            var shared:File = File.userDirectory.resolvePath("shared");
            var fileList:Array = shared.getDirectoryListing();

            for (var i:uint = 0; i < fileList.length; i++)
            {
                var file:File = fileList[i] as File;

                if (file.isDirectory)
                {
                    if (file.name == "camera")
                    {
                        sharedDirURL = file.url + "/" + fileName;
                    }
                }
            }

            return sharedDirURL;
        }

        private function isFileTypeValid(filePath:String):Boolean
        {
            if (filePath)
            {
                filePath = filePath.toLowerCase();
                for (var fileType:String in SUPPORTED_FILE_TYPES)
                {
                    var suffix:String = SUPPORTED_FILE_TYPES[fileType].toString();
                    if (suffix == filePath.substring(filePath.length - suffix.length))
                    {
                        return true;
                    }
                }

            }

            return false;
        }

        private function generateBarcode(contents:String, data:Object):ByteArray
        {
            // read input string
            var barcodeWriter:MultiFormatWriter = new MultiFormatWriter();

            try
            {
                // try to encode the string
                var result:ByteMatrix = (barcodeWriter.encode(contents, getMappedBarcodeFormatObject(data.format), data.width, data.height, data.htEncodeHints)) as ByteMatrix;
            }
            catch (e:Error)
            {
                throw e;
            }
            // try to generate an image with the result data
            var bmpd:BitmapData = new BitmapData(data.width, data.height, false, 0x009900);

            for (var h:int = 0; h < data.height; h++)
            {
                for (var w:int = 0; w < data.width; w++)
                {
                    if (result._get(w, h) == 0)
                    {
                        bmpd.setPixel(w, h, 0);
                    }
                    else
                    {
                        bmpd.setPixel(w, h, 0xFFFFFF);
                    }
                }
            }


            var pngEncoder:PNGEncoder = new PNGEncoder();

            return pngEncoder.encode(bmpd);
        }

        // Return BarcodeFormat object instead of creating one from the string because
        // encode method check against instance instead of the value object holds.
        private function getMappedBarcodeFormatObject(barcodeFormat:String):BarcodeFormat
        {
            switch (barcodeFormat.toUpperCase())
            {
                case BarcodeFormat.UPC_E.toString():
                    return BarcodeFormat.UPC_E;
                case BarcodeFormat.UPC_A.toString():
                    return BarcodeFormat.UPC_A;
                case BarcodeFormat.EAN_8.toString():
                    return BarcodeFormat.EAN_8;
                case BarcodeFormat.EAN_13.toString():
                    return BarcodeFormat.EAN_13;
                case BarcodeFormat.CODE_39.toString():
                    return BarcodeFormat.CODE_39;
                case BarcodeFormat.CODE_128.toString():
                    return BarcodeFormat.CODE_128;
                case BarcodeFormat.PDF417.toString():
                    return BarcodeFormat.PDF417;
                case BarcodeFormat.ITF.toString():
                    return BarcodeFormat.ITF;
                case "QR": //For simplicity, using QR instead of QR_CODE
                    return BarcodeFormat.QR_CODE;
                case BarcodeFormat.DATAMATRIX.toString():
                    return BarcodeFormat.DATAMATRIX;
                default:
                    return new BarcodeFormat("");
            }
        }

        private function saveBarcodeImageToFile(barcodeFile:File, imageByteArray:ByteArray):void
        {
            var fs:FileStream = new FileStream();
            //open file in write mode
            fs.open(barcodeFile, FileMode.WRITE);
            //write bytes from the byte array
            fs.writeBytes(imageByteArray);
            //close the file
            fs.close();
        }

        private function isBarcodeFileExists(filePath:String):Boolean
        {
            var file:File = new File(filePath);

            // if path exists but points to directory, should still return false				
            if (file.exists && !file.isDirectory)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
		
		private function createResultObject(code:int = 0, msg:String = null, data:Object = null) : Object {			
			return {
				"code" : 0,
				"msg" : null,
				"data" : data ? data : {}
			};	
		}
    }
}
