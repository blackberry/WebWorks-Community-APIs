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
			mCameraHandle = CAMERA_HANDLE_INVALID;
	}

	QRCodeReaderNDK::~QRCodeReaderNDK() {}
    //need to refactor and allow for all possible errors to bubble up to JS layer
	//we need to return up all the possible errors
	char*
	getCameraErrorDesc(camera_error_t err)
		{
			char* ret;
			switch (err) {
			case CAMERA_EAGAIN:
				return "The specified camera was not available. Try again.";
			case CAMERA_EINVAL:
				return "The camera call failed because of an invalid parameter.";
			break;
			case CAMERA_ENODEV:
				return "No such camera was found.";
			break;
			case CAMERA_EMFILE:
				return "The camera called failed because of a file table overflow.";
			break;
			case CAMERA_EBADF:
				return "Indicates that an invalid handle to a @c camera_handle_t value was used.";
			break;
			case CAMERA_EACCESS:
				return "Indicates that the necessary permissions to access the camera are not available.";
			break;
			case CAMERA_EBADR:
				return "Indicates that an invalid file descriptor was used.";
			break;
			case CAMERA_ENOENT:
				return "Indicates that the access a file or directory that does not exist.";
			break;
			case CAMERA_ENOMEM:
				return "Indicates that memory allocation failed.";
			break;
			case CAMERA_EOPNOTSUPP:
				return "Indicates that the requested operation is not supported.";
			break;
			case CAMERA_ETIMEDOUT:
				return "Indicates an operation on the camera is already in progress. In addition, this error can indicate that an error could not be completed because i was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.";
			break;
			case CAMERA_EALREADY:
				return "Indicates an operation on the camera is already in progress. In addition,this error can indicate that an error could not be completed because it was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.";
			break;
			case CAMERA_EUNINIT:
				return "Indicates that the Camera Library is not initialized.";
			break;
			case CAMERA_EREGFAULT:
				return "Indicates that registration of a callback failed.";
			break;
			case CAMERA_EMICINUSE:
				return "Indicates that it failed to open because microphone is already in use.";
			break;
			}
			return NULL;
		}


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
			root["successful"] = true;
			root["error"] = EOK;
			root["reason"] = "";
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
		std::string event = "community.QRCodeReader.codeFoundCallback.native";
		Json::FastWriter writer;
		Json::Value root;

		camera_error_t err;

		err = camera_open(CAMERA_UNIT_REAR,CAMERA_MODE_RW | CAMERA_MODE_ROLL,&mCameraHandle);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
		   fprintf(stderr, " Ran into an issue when initializing the camera = %d\n ", err);

#endif
		   root["successful"] = false;
		   root["error"] = err;
		   root["reason"] = getCameraErrorDesc( err );
		   eventController->NotifyEvent(event + " " + writer.write(root));
		   return EIO;
		}

		err = camera_start_photo_viewfinder( mCameraHandle, &viewfinder_callback, NULL, NULL);

		if ( err != CAMERA_EOK) {
#ifdef DEBUG
		   fprintf(stderr, "Ran into a strange issue when starting up the camera viewfinder\n");
#endif
		   root["successful"] = false;
		   root["error"] = err;
		   root["reason"] = getCameraErrorDesc( err );
		   eventController->NotifyEvent(event + " " + writer.write(root));
		   return EIO;
		}
		return EOK;
	}

	int QRCodeReaderNDK::stopQRCodeRead(){
		camera_error_t err;
		Json::FastWriter writer;
		Json::Value root;
		std::string event = "community.QRCodeReader.disabledCallback.native";
		root["disabled"] =  "true";


		err = camera_stop_photo_viewfinder(mCameraHandle);
		if ( err != CAMERA_EOK)
		{
#ifndef DEBUG
		   fprintf(stderr, "Error with turning off the photo viewfinder \n");
#endif
		   root["successful"] = false;
		   root["error"] = err;
		   root["reason"] = getCameraErrorDesc( err );
		   eventController->NotifyEvent(event + " " + writer.write(root));
		   return EIO;
		}

		//check to see if the camera is open, if it is open, then close it
		err = camera_close(mCameraHandle);
		if ( err != CAMERA_EOK){
#ifndef DEBUG
		   fprintf(stderr, "Error with closing the camera \n");
#endif
		   root["successful"] = false;
		   root["error"] = err;
		   root["reason"] = getCameraErrorDesc( err );
		   eventController->NotifyEvent(event + " " + writer.write(root));
		   return EIO;
		}

		root["successful"] = true;
	    root["error"] = EOK;
	    root["reason"] = "";
	    mCameraHandle = CAMERA_HANDLE_INVALID;
	    eventController->NotifyEvent(event + " " + writer.write(root));
		return EOK;
	}

}

