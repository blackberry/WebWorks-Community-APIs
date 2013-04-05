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

#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/MultiFormatReader.h>
#include <img/img.h>
#include <stdio.h>

#include "barcodescanner_ndk.hpp"
#include "barcodescanner_js.hpp"

#include <sstream>

using namespace zxing;

namespace webworks {

BarcodeScannerJS* eventDispatcher = NULL;
static int filecounter = 0;
#define TMP_PATH "tmp/"
static uint32_t rotation = 0;

    /*
     * getCameraErrorDesc
     *
     * Returns a descriptive error message for a given camera error code
     */
    const char* getCameraErrorDesc(camera_error_t err) {
        char* ret;
        switch (err) {
        case CAMERA_EAGAIN:
            return "The specified camera was not available. Try again.";
        case CAMERA_EINVAL:
            return "The camera call failed because of an invalid parameter.";
        case CAMERA_ENODEV:
            return "No such camera was found.";
        case CAMERA_EMFILE:
            return "The camera called failed because of a file table overflow.";
        case CAMERA_EBADF:
            return "Indicates that an invalid handle to a @c camera_handle_t value was used.";
        case CAMERA_EACCESS:
            return "Indicates that the necessary permissions to access the camera are not available.";
        case CAMERA_EBADR:
            return "Indicates that an invalid file descriptor was used.";
        case CAMERA_ENOENT:
            return "Indicates that the access a file or directory that does not exist.";
        case CAMERA_ENOMEM:
            return "Indicates that memory allocation failed.";
        case CAMERA_EOPNOTSUPP:
            return "Indicates that the requested operation is not supported.";
        case CAMERA_ETIMEDOUT:
            return "Indicates an operation on the camera is already in progress. In addition, this error can indicate that an error could not be completed because i was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.";
        case CAMERA_EALREADY:
            return "Indicates an operation on the camera is already in progress. In addition,this error can indicate that an error could not be completed because it was already completed. For example, if you called the @c camera_stop_video() function but the camera had already stopped recording video, this error code would be returned.";
        case CAMERA_EUNINIT:
            return "Indicates that the Camera Library is not initialized.";
        case CAMERA_EREGFAULT:
            return "Indicates that registration of a callback failed.";
        case CAMERA_EMICINUSE:
            return "Indicates that it failed to open because microphone is already in use.";
        }
        return NULL;
    }


    /*
     * viewfinder_callback
     *
     * This callback is invoked when an image frame from the camera viewfinder becomes available.
     * The frame is analyzed to determine if a barcode can be matched.
     * Frames come in NV12 format which makes code analysis very fast.
     */
    void viewfinder_callback(camera_handle_t handle,camera_buffer_t* buf,void* arg) {
        camera_frame_nv12_t* data = (camera_frame_nv12_t*)(&(buf->framedesc));
        uint8_t* buff = buf->framebuf;
        int stride = data->stride;
        int width = data->width;
        int height = data->height;

        try {
            Ref<LuminanceSource> source(new GreyscaleLuminanceSource((unsigned char *)buff, stride, height, 0,0,width,height));

            Ref<Binarizer> binarizer(new HybridBinarizer(source));
            Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
            Ref<Result> result;

            // setup the code reader
            MultiFormatReader *reader = new MultiFormatReader();
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

			// attempt to decode and retrieve a valid QR code from the image bitmap
			result = reader->decode(bitmap, *hints);

            std::string newBarcodeData = result->getText()->getText().data();

            Json::FastWriter writer;
            Json::Value root;
            root["value"] = newBarcodeData;
            std::string event = "community.barcodescanner.codefound.native";

            // notify caller that a valid QR code has been decoded
            if ( eventDispatcher != NULL){
            	 eventDispatcher->NotifyEvent(event + " " + writer.write(root));
            }


#ifdef DEBUG
            fprintf(stderr, "This is the detected Barcode : %s\n", newBarcodeData.c_str());
#endif
        }
        catch (const std::exception& ex)
        {
#ifdef DEBUG
            fprintf( stderr, "error occured : \%s \n", ex.what());
#endif
        }
    }

    std::string convertIntToString(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
	}

    /*
     * image_callback
     *
     * handles the burst frames from the camera, which are standard JPEG images.
     * These will be sent to the front end for display.
     */
    void image_callback(camera_handle_t handle,camera_buffer_t* buf,void* arg) {

       	if (buf->frametype == CAMERA_FRAMETYPE_JPEG) {
			fprintf(stderr, "still image size: %lld\n", buf->framedesc.jpeg.bufsize);

			Json::FastWriter writer;
			Json::Value root;

			// saving temporary files barcode0.jpg to barcode9.jpg
			std::string tempFileName = "barcode" + convertIntToString(filecounter) + ".jpg";
			if (++filecounter >=10) {
				filecounter = 0;
			}
			// saving in the /tmp directory of the application which gets cleaned out when the app exits
			std::string tempFilePath = std::string(getcwd(NULL, 0)) + "/" + TMP_PATH + tempFileName;
			FILE* fp = fopen(tempFilePath.c_str(), "wb");
			if (fp!= NULL) {
				fwrite((const unsigned char *)buf->framebuf, buf->framedesc.jpeg.bufsize, 1, fp);
				fclose(fp);
			}

			// QC8960 based devices create jpegs with exif orientation and need rotating
			// We'll also scale down as much as possible to reduce file size.
			img_lib_t ilib;
			int rc;
			if ((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
				fprintf(stderr, "img_lib_attach() failed: %d\n", rc);
			}

			img_t img;
			if (rotation == 0 || rotation == 2) {
				img.w = 240;
				img.flags = IMG_W;
			} else {
				img.h = 240;
				img.flags = IMG_H;
			}
			int resizeResult = img_load_resize_file( ilib, tempFilePath.c_str(), NULL, &img );
			img_t dst;
			img_fixed_t angle = 0;
			switch (rotation) {
			case 1:
				angle = IMG_ANGLE_90CCW;
				break;
			case 2:
				angle = IMG_ANGLE_180;
				break;
			case 3:
				angle = IMG_ANGLE_90CW;
				break;
			default:
				break;
			}
			if (angle != 0) {
				int err = img_rotate_ortho(&img, &dst, angle);
			} else {
				dst = img;
			}
			int writeResult = img_write_file( ilib, tempFilePath.c_str(), NULL, &dst );

			img_lib_detach(ilib);

			// Send the file path for loading in the front end since JNEXT only handles strings
			root["frame"]  = tempFilePath;
			std::string event = "community.barcodescanner.frameavailable.native";
			if ( eventDispatcher != NULL ){
				 eventDispatcher->NotifyEvent(event + " " + writer.write(root));
			}
       	}

    }

    /*
     * Constructor for Barcode Scanner NDK class
     */
    BarcodeScannerNDK::BarcodeScannerNDK(BarcodeScannerJS *parent) {
        m_pParent     = parent;
        eventDispatcher = parent;
        mCameraHandle = CAMERA_HANDLE_INVALID;
    }

    BarcodeScannerNDK::~BarcodeScannerNDK() {}


    /*
     * BarcodeScannerNDK::startRead
     *
     * This method is called to start a QR code read. A connection is opened to the device camera
     * and the photo viewfinder is started.
     */
    int BarcodeScannerNDK::startRead() {

        std::string errorEvent = "community.barcodescanner.errorfound.native";
        Json::FastWriter writer;
        Json::Value root;

        camera_error_t err;
        // Open the camera first before running any operations on it
        err = camera_open(CAMERA_UNIT_REAR,CAMERA_MODE_RW | CAMERA_MODE_ROLL,&mCameraHandle);
        if ( err != CAMERA_EOK){
#ifdef DEBUG
            fprintf(stderr, " Ran into an issue when initializing the camera = %d\n ", err);
#endif
            root["state"] = "Open Camera";
            root["error"] = err;
            root["description"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
            return EIO;
        }

        // We want maximum framerate from the viewfinder which will scan for codes
        int numRates = 0;
		err = camera_get_photo_vf_framerates(mCameraHandle, true, 0, &numRates, NULL, NULL);
		double* camFramerates = new double[numRates];
		bool maxmin = false;
		err = camera_get_photo_vf_framerates(mCameraHandle, true, numRates, &numRates, camFramerates, &maxmin);

		// QC8960 doesn't allow for changing the rotation, so we'll just take note of it here and rotate later.
		uint32_t* rotations = new uint32_t[8];
		int numRotations = 0;
		bool nonsquare = false;
		err = camera_get_photo_rotations(mCameraHandle, CAMERA_FRAMETYPE_JPEG, true, 8, &numRotations, rotations, &nonsquare);
		rotation = rotations[0] / 90;

		// We're going to turn on burst mode for the camera and set maximum framerate for the viewfinder
		err = camera_set_photovf_property(mCameraHandle,
			CAMERA_IMGPROP_BURSTMODE, 1,
			CAMERA_IMGPROP_FRAMERATE, camFramerates[0]);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
			fprintf(stderr, " Ran into an issue when configuring the camera viewfinder = %d\n ", err);
#endif
			root["state"] = "Set VF Props";
			root["error"] = err;
			root["description"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
			return EIO;
		}

		// The actual camera will get frames at a slower rate than the viewfinder
		err = camera_set_photo_property(mCameraHandle,
			CAMERA_IMGPROP_BURSTDIVISOR, (double) 3.0);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
			fprintf(stderr, " Ran into an issue when configuring the camera properties = %d\n ", err);
#endif
			root["state"] = "Set Cam Props";
			root["error"] = err;
			root["description"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
			return EIO;
		}

		// Starting viewfinder up which will call the viewfinder callback - this gets the NV12 images for scanning
        err = camera_start_photo_viewfinder( mCameraHandle, &viewfinder_callback, NULL, NULL);
        if ( err != CAMERA_EOK) {
#ifdef DEBUG
            fprintf(stderr, "Ran into a strange issue when starting up the camera viewfinder\n");
#endif
            root["state"] = "ViewFinder Start";
            root["error"] = err;
            root["description"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
            return EIO;
        }

        // Focus mode can't be set until the viewfinder is started. We need Continuous Macro for barcodes
        err = camera_set_focus_mode(mCameraHandle, CAMERA_FOCUSMODE_CONTINUOUS_MACRO);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
			fprintf(stderr, " Ran into an issue when setting focus mode = %d\n ", err);
#endif
			root["state"] = "Set Focus Mode";
			root["error"] = err;
			root["description"] =  getCameraErrorDesc( err );
			m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
			return EIO;
		}

		// Now start capturing burst frames in JPEG format for sending to the front end.
		err = camera_start_burst(mCameraHandle, NULL, NULL, NULL, &image_callback, NULL);
		if ( err != CAMERA_EOK) {
#ifdef DEBUG
			fprintf(stderr, "Ran into an issue when starting up the camera in burst mode\n");
#endif
			root["state"] = "Start Camera Burst";
			root["error"] = err;
			root["description"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
			return EIO;
		}

        std::string successEvent = "community.barcodescanner.started.native";
        root["successful"] = true;
        m_pParent->NotifyEvent(successEvent + " " + writer.write(root));
        return EOK;
    }


    /*
     * BarcodeScannerNDK::stopRead
     *
     * This method is called to clean up following successful detection of a barcode.
     * Calling this method will stop the viewfinder and close an open connection to the device camera.
     */
    int BarcodeScannerNDK::stopRead() {

    	std::string errorEvent = "community.barcodescanner.errorfound.native";
		Json::FastWriter writer;
		Json::Value root;
        camera_error_t err;

        err = camera_stop_burst(mCameraHandle);
		if ( err != CAMERA_EOK)
		{
#ifdef DEBUG
			fprintf(stderr, "Error with turning off the burst \n");
#endif
			root["error"] = err;
			root["description"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
			return EIO;
		}

        err = camera_stop_photo_viewfinder(mCameraHandle);
        if ( err != CAMERA_EOK)
        {
#ifdef DEBUG
            fprintf(stderr, "Error with turning off the photo viewfinder \n");
#endif
            root["error"] = err;
		    root["description"] = getCameraErrorDesc( err );
		    m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
            return EIO;
        }

        //check to see if the camera is open, if it is open, then close it
        err = camera_close(mCameraHandle);
        if ( err != CAMERA_EOK){
#ifdef DEBUG
            fprintf(stderr, "Error with closing the camera \n");
#endif
            root["error"] = err;
            root["description"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(errorEvent + " " + writer.write(root));
            return EIO;
        }

        std::string successEvent = "community.barcodescanner.ended.native";
        root["successful"] = true;
        mCameraHandle = CAMERA_HANDLE_INVALID;
        m_pParent->NotifyEvent(successEvent + " " + writer.write(root));

        return EOK;
    }

}
