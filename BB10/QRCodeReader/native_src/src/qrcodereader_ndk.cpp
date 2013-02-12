/*
 * Copyright 2013 Research In Motion Limited.
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

#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>

#include <camera/camera_api.h>
#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <sstream>
#include "qrcodereader_ndk.hpp"
#include "qrcodereader_js.hpp"
using namespace zxing;
using namespace zxing::qrcode;

namespace webworks {

	QRCodeReaderJS *eventController;
	QRCodeReaderNDK* self;
	QRCodeReaderNDK::QRCodeReaderNDK(QRCodeReaderJS *parent) {
			m_pParent = parent;
			eventController = parent;
			self = this;
	}


	void printError(camera_error_t err)
	{
		switch (err) {
		case CAMERA_EAGAIN:
		fprintf(stderr,"The specified camera was not available. Try again.\n");
		break;
		case CAMERA_EINVAL:
		fprintf(stderr,"The camera call failed because of an invalid parameter.\n");
		break;
		case CAMERA_ENODEV:
		fprintf(stderr, "No such camera was found.\n");
		break;
		case CAMERA_EMFILE:
		fprintf(stderr,"The camera called failed because of a file table overflow.\n");
		break;
		case CAMERA_EBADF:
		fprintf(stderr,"Indicates that an invalid handle to a @c camera_handle_t value was used.\n");
		break;
		case CAMERA_EACCESS:
		fprintf(stderr,"Indicates that the necessary permissions to access the camera are not available.\n");
		break;
		case CAMERA_EBADR:
		fprintf(stderr,"Indicates that an invalid file descriptor was used.\n");
		break;
		case CAMERA_ENOENT:
		fprintf(stderr,"Indicates that the access a file or directory that does not exist.\n");
		break;
		case CAMERA_ENOMEM:
		fprintf(stderr, "Indicates that memory allocation failed.\n");
		break;
		case CAMERA_EOPNOTSUPP:
		fprintf(stderr,
		"Indicates that the requested operation is not supported.\n");
		break;
		case CAMERA_ETIMEDOUT:
		fprintf(stderr,"Indicates an operation on the camera is already in progress. In addition, this error can indicate that an error could not be completed because i was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.\n");
		break;
		case CAMERA_EALREADY:
		fprintf(stderr,
		"Indicates an operation on the camera is already in progress. In addition,this error can indicate that an error could not be completed because it was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.\n");
		break;
		case CAMERA_EUNINIT:
		fprintf(stderr,"Indicates that the Camera Library is not initialized.\n");
		break;
		case CAMERA_EREGFAULT:
		fprintf(stderr,"Indicates that registration of a callback failed.\n");
		break;
		case CAMERA_EMICINUSE:
		fprintf(stderr,"Indicates that it failed to open because microphone is already in use.\n");
		break;
		}
	}


	QRCodeReaderNDK::~QRCodeReaderNDK() {}

	void
	viewfinder_callback(camera_handle_t handle,camera_buffer_t* buf,void* arg)
	{
		camera_frame_nv12_t* data = (camera_frame_nv12_t*)(&(buf->framedesc));
		uint8_t* buff = buf->framebuf;
		int stride = data->stride;
		int width  = data->width;
		int height = data->height;

		try{
			Ref<LuminanceSource> source(new GreyscaleLuminanceSource((unsigned char *)buff, stride, height, 0,0,width,height));

			Ref<Binarizer> binarizer(new HybridBinarizer(source));
			Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
			Ref<Result> result;

			QRCodeReader *reader = new QRCodeReader();
			DecodeHints *hints = new DecodeHints();

			hints->addFormat(BarcodeFormat_QR_CODE);
			hints->addFormat(BarcodeFormat_EAN_8);
			hints->addFormat(BarcodeFormat_EAN_13);
			hints->addFormat(BarcodeFormat_UPC_A);
			hints->addFormat(BarcodeFormat_UPC_E);
			hints->addFormat(BarcodeFormat_DATA_MATRIX);
			hints->addFormat(BarcodeFormat_CODE_128);
			hints->addFormat(BarcodeFormat_CODE_39);
			hints->addFormat(BarcodeFormat_ITF);
			hints->addFormat(BarcodeFormat_AZTEC);

			result = reader->decode(bitmap, *hints);
			std::string newBarcodeData = result->getText()->getText().data();

			Json::FastWriter writer;
			Json::Value root;
			root["value"] =  newBarcodeData;
			std::string event = "community.QRCodeReader.codeFoundCallback.native";

			eventController->NotifyEvent(event + " " + writer.write(root));
			self->stopQRCodeRead();
			#ifdef DEBUG
				fprintf(stderr, "This is the detected QR Code : %s\n", newBarcodeData.c_str());
			#endif
		}
		catch (const std::exception& ex)
		{
		#ifdef DEBUG
			fprintf( stderr, "error occured : \%s \n", ex.what());
		#endif
		}
	}



	// Asynchronous callback with JSON data input and output
	int QRCodeReaderNDK::startQRCodeRead(){
		mCameraHandle = CAMERA_HANDLE_INVALID;
		camera_error_t err;

		err = camera_open(CAMERA_UNIT_REAR,CAMERA_MODE_RW | CAMERA_MODE_ROLL,&mCameraHandle);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
		   fprintf(stderr, " Ran into an issue when initializing the camera = %d\n ", err);
		   printError( err ) ;
#endif
		   return EIO;
		}

		err = camera_start_photo_viewfinder( mCameraHandle, &viewfinder_callback, NULL, NULL);

		if ( err != CAMERA_EOK) {
#ifdef DEBUG
		   fprintf(stderr, "Ran into a strange issue when starting up the camera viewfinder\n");
		   printError( err ) ;
#endif
		   return EIO;
		}
		return EOK;
	}

	int QRCodeReaderNDK::stopQRCodeRead(){
		camera_error_t err;
		//check to see if the view finder is enabled, if it is enabled, disable it
		err = camera_stop_photo_viewfinder(mCameraHandle);
		if ( err != CAMERA_EOK)
		{
		   fprintf(stderr, "Error with turning off the photo viewfinder \n");
		   printError( err ) ;
		   return EIO;
		}

		//check to see if the camera is open, if it is open, then close it
		err = camera_close(mCameraHandle);
		if ( err != CAMERA_EOK){
		   fprintf(stderr, "Error with closing the camera \n");
		   printError( err ) ;
		   return EIO;
		}

		Json::FastWriter writer;
					Json::Value root;
					root["disabled"] =  "true";
					std::string event = "community.QRCodeReader.disabledCallback.native";

					eventController->NotifyEvent(event + " " + writer.write(root));
					self->stopQRCodeRead();
		return EOK;
	}

}

