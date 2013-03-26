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
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <screen/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>
#include <bps/navigator.h>
#include <pthread.h>
#include <stdio.h>
#include <img/img.h>

#include <base64/base64.h>

#include "qrcodereader_ndk.hpp"
#include "qrcodereader_js.hpp"

#include <sstream>

using namespace zxing;
using namespace zxing::qrcode;

namespace webworks {

QRCodeReaderJS* QRCodeReaderNDK::m_pParent = NULL;
camera_handle_t QRCodeReaderNDK::mCameraHandle = CAMERA_HANDLE_INVALID;
static screen_context_t screen_ctx;
static screen_window_t vf_win = NULL;
static const char vf_group[] = "viewfinder_window_group";
static int main_bps_chid = -1;
static int m_thread = 0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static bool threadHalt = true;
static int zOrder = 2000;
static int filecounter = 0;
#define TMP_PATH "tmp/"
static uint32_t rotation = 0;

static img_lib_t ilib = NULL;

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
     * The frame is analyzed to determine if a QR code can be matched.
     */
    void viewfinder_callback(camera_handle_t handle,camera_buffer_t* buf,void* arg) {
        camera_frame_nv12_t* data = (camera_frame_nv12_t*)(&(buf->framedesc));
        uint8_t* buff = buf->framebuf;
        int stride = data->stride;
        int width = data->width;
        int height = data->height;



//        std::string bitmapHeader = getBMPHeader(width, height);
//
//        Json::FastWriter writer;
//        Json::Value root;
//        root["header"] = base64_encode((const unsigned char *)bitmapHeader.c_str(), bitmapHeader.length());
//        root["frame"]  = base64_encodeRGB888((const unsigned char *)buff, width * height);
//        std::string event = "community.QRCodeReader.codeFoundCallback.native";

        // push encoded frame back to caller
//        QRCodeReaderNDK::m_pParent->NotifyEvent(event + " " + writer.write(root));

        try{
            Ref<LuminanceSource> source(new GreyscaleLuminanceSource((unsigned char *)buff, stride, height, 0,0,width,height));

            Ref<Binarizer> binarizer(new HybridBinarizer(source));
            Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
            Ref<Result> result;

            // setup the QR code reader
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


            Json::FastWriter writer;
            Json::Value root;

            // Rotate bitmap if the preview frames are in landscape for 1D codes
            if (buf->frameorientation != 0 || buf->frameorientation != 180) {
            	bitmap->rotateCounterClockwise();
            	root["rotated"] = true;
            }

            // attempt to decode and retrieve a valid QR code from the image bitmap
            result = reader->decode(bitmap, *hints);
            std::string newBarcodeData = result->getText()->getText().data();

            root["successful"] = true;
            root["error"] = EOK;
            root["reason"] = "";
            root["value"] = newBarcodeData;
            root["orientation"] = buf->frameorientation;
            root["frametype"] = buf->frametype;
            std::string event = "community.QRCodeReader.codeFoundCallback.native";

            // notify caller that a valid QR code has been decoded
            QRCodeReaderNDK::m_pParent->NotifyEvent(event + " " + writer.write(root));
            QRCodeReaderNDK::stopQRCodeRead();

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

    std::string convertIntToString(int i) {
		stringstream ss;
		ss << i;
		return ss.str();
    }

    void image_callback(camera_handle_t handle,camera_buffer_t* buf,void* arg) {
    	camera_frame_jpeg_t* data = (camera_frame_jpeg_t*)(&(buf->framedesc));
    	uint8_t* buff = buf->framebuf;

    	if (buf->frametype == CAMERA_FRAMETYPE_JPEG) {
			fprintf(stderr, "still image size: %lld\n", buf->framedesc.jpeg.bufsize);

//			std::string bitmapHeader = getBMPHeader(width, height);

			Json::FastWriter writer;
			Json::Value root;
//			root["header"] = base64_encode((const unsigned char *)bitmapHeader.c_str(), bitmapHeader.length());
//			root["frame"]  = base64_encode((const unsigned char *)buf->framebuf, buf->framedesc.jpeg.bufsize);

//

			std::string tempFileName = "barcode" + convertIntToString(filecounter) + ".jpg";
			if (++filecounter >=10) {
				filecounter = 0;
			}
			std::string tempFilePath = std::string(getcwd(NULL, 0)) + "/" + TMP_PATH + tempFileName;
			FILE* fp = fopen(tempFilePath.c_str(), "wb");
			if (fp!= NULL) {
				fwrite((const unsigned char *)buf->framebuf, buf->framedesc.jpeg.bufsize, 1, fp);
				fclose(fp);
			}
//
//
			img_t img;
			if (rotation == 0 || rotation == 2) {
				img.w = 720;
				img.flags = IMG_W;
			} else {
				img.h = 720;
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

			root["frame"]  = tempFilePath;
//			root["frame"]  = base64_encode(dst.access.direct.data, dst.h * dst.access.direct.stride);
			root["orientation"] = buf->frameorientation;
			root["frametype"] = buf->frametype;
			root["rotation"] = rotation;
			std::string event = "community.QRCodeReader.codeFoundCallback.native";
			QRCodeReaderNDK::m_pParent->NotifyEvent(event + " " + writer.write(root));

			/*
			// if we have enough room left in our 1000-frame (!!!) buffer list...
			if (burst_frame_count+1 < sizeof(burst_bufs) / sizeof(*burst_bufs)) {
				// what we are going to do is make a copy of the jpeg buffer in memory.
				// we will save all of the copies to disk after the burst capture is complete.
				// this is because we don't know if we will be able to write to disk fast enough to keep up
				// with the incoming frames.
				burst_sizes[burst_frame_count] = buf->framedesc.jpeg.bufsize;
				burst_bufs[burst_frame_count] = malloc(burst_sizes[burst_frame_count]);
				if (!burst_bufs[burst_frame_count]) {
					fprintf(stderr, "could not malloc a buffer to cache frame %d\n", burst_frame_count+1);
				} else {
					memcpy(burst_bufs[burst_frame_count], buf->framebuf, burst_sizes[burst_frame_count]);
					burst_frame_count++;
					fprintf(stderr, "cached burst frame %d\n", burst_frame_count);
				}
			}
			*/
    	}
    }


    /*
     * Constructor for QR reader NDK class
     */
    QRCodeReaderNDK::QRCodeReaderNDK(QRCodeReaderJS *parent) {
    	m_pParent     = parent;
    	mCameraHandle = CAMERA_HANDLE_INVALID;
    }

    QRCodeReaderNDK::~QRCodeReaderNDK() {}

    static void handle_navigator_event(bps_event_t *event) {
        switch (bps_event_get_code(event)) {
        case NAVIGATOR_SWIPE_DOWN:
            fprintf(stderr,"Swipe down event\n");
            break;
        case NAVIGATOR_EXIT:
            fprintf(stderr,"Exit event\n");
//            shutdown = true;
            break;
        default:
            break;
        }
    }

    static void handle_screen_event(bps_event_t *event)
    {
        int screen_val;

        screen_event_t screen_event = screen_event_get_event(event);
        screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

        Json::FastWriter writer;
		Json::Value root;
		root["msg"] = "Screen Event";
		root["event"] = screen_val;
		std::string jsEvent = "community.QRCodeReader.codeFoundCallback.native";
		QRCodeReaderNDK::m_pParent->NotifyEvent(jsEvent + " " + writer.write(root));

        switch (screen_val) {
        case SCREEN_EVENT_MTOUCH_TOUCH:
            fprintf(stderr,"Touch event\n");
//            touch = true;
            break;
        case SCREEN_EVENT_MTOUCH_MOVE:
            fprintf(stderr,"Move event\n");
            break;
        case SCREEN_EVENT_MTOUCH_RELEASE:
            fprintf(stderr,"Release event\n");
            break;
        case SCREEN_EVENT_CREATE:
            // in a more involved application, it may be wise to actually check the window name to ensure
            // that we are processing the viewfinder window here, and not some other window
            if (screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void **)&vf_win) == -1) {
//                perror("screen_get_event_property_pv(SCREEN_PROPERTY_WINDOW)");
            	fprintf(stderr,"Didn't get VF Window\n");
            } else {
                fprintf(stderr,"viewfinder window found!\n");
                Json::FastWriter writer;
				Json::Value root;
				root["msg"] = "vf window found";
				std::string event = "community.QRCodeReader.codeFoundCallback.native";
				QRCodeReaderNDK::m_pParent->NotifyEvent(event + " " + writer.write(root));

				// mirror viewfinder if this is the front-facing camera
//                int i = (shouldmirror?1:0);
//                screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_MIRROR, &i);
                // place viewfinder in front of the black application background window.
                // note that a child window's ZORDER is relative to it's parent.
                // if we wanted to draw a UI on the application window, we could place the
                // viewfinder behind it and rely on transparency.  or alternately, another
                // child window could be overlaid on top of the viewfinder.
                int i = zOrder+1;
                screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_ZORDER, &i);
                // make viewfinder window visible
                i = 1;
                screen_set_window_property_iv(vf_win, SCREEN_PROPERTY_VISIBLE, &i);
                screen_flush_context(screen_ctx, 0);
                // we should now have a visible viewfinder
                // other things we could do here include rotating the viewfinder window (screen rotation),
                // or adjusting the size & position of the window.
                // some properties are immutable for security reasons since the window was actually created
                // in another process.  anything related to presentation should be modifiable.
//                touch = false;
//                state = STATE_VIEWFINDER;
            }
            break;
        default:
            break;
        }
    }

    static void handle_event() {
		int rc, domain;

		bps_event_t *event = NULL;
		rc = bps_get_event(&event, -1);
	//        assert(rc == BPS_SUCCESS);

		Json::FastWriter writer;
		Json::Value root;
		root["msg"] = "Handle Event";
		std::string jsEvent = "community.QRCodeReader.codeFoundCallback.native";
		QRCodeReaderNDK::m_pParent->NotifyEvent(jsEvent + " " + writer.write(root));
		if (event) {
			domain = bps_event_get_domain(event);
			if (domain == navigator_get_domain()) {
				handle_navigator_event(event);
			} else if (domain == screen_get_domain()) {
				handle_screen_event(event);
			}
		}
	}

    void* TemplateThread(void* parent) {
    QRCodeReaderNDK *pParent = static_cast<QRCodeReaderNDK *>(parent);

    // Loop calls the callback function and continues until stop is set
    while (true /*!pParent->isThreadHalt()*/) {
    	handle_event();
    }

    return NULL;
    }

    // Starts the thread and returns a message on status
    std::string QRCodeReaderNDK::templateStartThread() {

    	Json::FastWriter writer;
		Json::Value root;
		root["msg"] = "starting thread";
		std::string event = "community.QRCodeReader.codeFoundCallback.native";
		QRCodeReaderNDK::m_pParent->NotifyEvent(event + " " + writer.write(root));
//    if (!m_thread) {
    int rc;
    rc = pthread_mutex_lock(&mutex);
    threadHalt = false;
    rc = pthread_cond_signal(&cond);
    rc = pthread_mutex_unlock(&mutex);

    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&m_thread, &thread_attr, TemplateThread,
    static_cast<void *>(this));
    pthread_attr_destroy(&thread_attr);
    return "Thread Started";
//    } else {
//    return "Thread Running";
//    }
    }

    // Sets the stop value
    std::string QRCodeReaderNDK::templateStopThread() {
    int rc;
    // Request thread to set prevent sleep to false and terminate
    rc = pthread_mutex_lock(&mutex);
    threadHalt = true;
    rc = pthread_cond_signal(&cond);
    rc = pthread_mutex_unlock(&mutex);

        // Wait for the thread to terminate.
        void *exit_status;
        rc = pthread_join(m_thread, &exit_status) ;

    // Clean conditional variable and mutex
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    m_thread = 0;
    threadHalt = true;
    return "Thread stopped";
    }


    /*
     * QRCodeReaderNDK::startQRCodeRead
     *
     * This method is called to start a QR code read. A connection is opened to the device camera
     * and the photo viewfinder is started.
     */
    int QRCodeReaderNDK::startQRCodeRead(const std::string& inputString) {

//    	const int usage = SCREEN_USAGE_NATIVE;
//    	screen_window_t screen_win;
//		screen_buffer_t screen_buf = NULL;
//		int rect[4] = { 0, 0, 0, 0 };

//		// create an application window which will just act as a background
//		screen_create_context(&screen_ctx, 0);
//		screen_create_window(&screen_win, screen_ctx);
//		screen_create_window_group(screen_win, vf_group);
//		screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
//		screen_create_window_buffers(screen_win, 1);
//		screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
//		screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
//
//		// fill the window with black
//		int attribs[] = { SCREEN_BLIT_COLOR, 0x00000000, SCREEN_BLIT_END };
//		screen_fill(screen_ctx, screen_buf, attribs);
//		screen_post_window(screen_win, screen_buf, 1, rect, 0);
//		// position the window at an arbitrary z-order
//		int i = zOrder;
//		screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ZORDER, &i);
//
//		// Signal bps library that navigator and screen events will be requested
//		bps_initialize();
//		main_bps_chid = bps_channel_get_active();
//		screen_request_events(screen_ctx);
//		navigator_request_events(0);
		int rc;
		if ((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
			fprintf(stderr, "img_lib_attach() failed: %d\n", rc);
		}


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
            root["state"] = "Open Camera";
            root["error"] = err;
            root["reason"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(event + " " + writer.write(root));
            return EIO;
        }

        int handle = (int) strtol(inputString.c_str(), NULL, 10);
        screen_window_t window = (screen_window_t) handle;
        int z = -1;
        char* windowGroup = new char[100];
        char* windowName = new char[100];
        rc = screen_get_window_property_cv(window, SCREEN_PROPERTY_GROUP, 100, windowGroup );
        rc = screen_get_window_property_cv(window, SCREEN_PROPERTY_ID_STRING, 100, windowName );
        rc = screen_get_window_property_iv(window, SCREEN_PROPERTY_ZORDER, &z );

		root["WinGroup"] = windowGroup;
		root["WinName"] = windowName;
		root["zorder"] = z;
		m_pParent->NotifyEvent(event + " " + writer.write(root));

		uint numRes = 0;
		err = camera_get_photo_output_resolutions(mCameraHandle, CAMERA_FRAMETYPE_JPEG, 0, &numRes, NULL);
		camera_res_t* camResolutions = new camera_res_t[numRes];
		err = camera_get_photo_output_resolutions(mCameraHandle, CAMERA_FRAMETYPE_JPEG, numRes, &numRes, camResolutions);
		for (uint x=0; x<numRes; x++) {
			root["res w"] = camResolutions[x].width;
			root["res y"] = camResolutions[x].height;
			m_pParent->NotifyEvent(event + " " + writer.write(root));
		}


		int numRates = 0;
		err = camera_get_photo_vf_framerates(mCameraHandle, true, 0, &numRates, NULL, NULL);
		double* camFramerates = new double[numRates];
		bool maxmin = false;
		err = camera_get_photo_vf_framerates(mCameraHandle, true, numRates, &numRates, camFramerates, &maxmin);
		for (int x=0; x<numRates; x++) {
			root["framerate"] = camFramerates[x];
			root["maxmin"] = maxmin;
			m_pParent->NotifyEvent(event + " " + writer.write(root));
		}

		uint32_t* rotations = new uint32_t[8];
		int numRotations = 0;
		bool nonsquare = false;
		err = camera_get_photo_rotations(mCameraHandle, CAMERA_FRAMETYPE_UNSPECIFIED, true, 8, &numRotations, rotations, &nonsquare);
		rotation = rotations[0] / 90;
		root["rotation"] = rotations[0];
		root["numRotations"] = numRotations;
		m_pParent->NotifyEvent(event + " " + writer.write(root));

        err = camera_set_photovf_property(mCameraHandle,
//        	CAMERA_IMGPROP_WIN_GROUPID, vf_group,
//        	CAMERA_IMGPROP_WIN_ID, "barcodevf"
//        	CAMERA_IMGPROP_ISEMBEDDED, 1,
//        	CAMERA_IMGPROP_WIN_ID, windowName
			CAMERA_IMGPROP_BURSTMODE, 1,
			CAMERA_IMGPROP_FRAMERATE, camFramerates[0]
//			CAMERA_IMGPROP_CREATEWINDOW, 1
			);
        if ( err != CAMERA_EOK){
#ifdef DEBUG
            fprintf(stderr, " Ran into an issue when configuring the camera viewfinder = %d\n ", err);
#endif
            root["successful"] = false;
            root["state"] = "Set VF Props";
            root["WinGroup"] = windowGroup;
            root["WinName"] = windowName;
            root["error"] = err;
            root["reason"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(event + " " + writer.write(root));
            return EIO;
        }

        err = camera_set_photo_property(mCameraHandle,
//			CAMERA_IMGPROP_WIDTH, camResolutions[numRes-1].width,
//			CAMERA_IMGPROP_HEIGHT, camResolutions[numRes-1].height,
			CAMERA_IMGPROP_BURSTDIVISOR, camFramerates[0]
			);
        if ( err != CAMERA_EOK){
#ifdef DEBUG
			fprintf(stderr, " Ran into an issue when configuring the camera properties = %d\n ", err);
#endif
			root["successful"] = false;
			root["state"] = "Set Cam Props";
			root["error"] = err;
			root["reason"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(event + " " + writer.write(root));
			return EIO;
		}


//        err = camera_start_video_viewfinder( mCameraHandle, &viewfinder_callback, NULL, NULL);
        err = camera_start_photo_viewfinder( mCameraHandle, &viewfinder_callback, NULL, NULL);

        if ( err != CAMERA_EOK) {
#ifdef DEBUG
            fprintf(stderr, "Ran into a strange issue when starting up the camera viewfinder\n");
#endif
            root["successful"] = false;
            root["state"] = "VF Start";
            root["error"] = err;
            root["reason"] =  getCameraErrorDesc( err );
            m_pParent->NotifyEvent(event + " " + writer.write(root));
            return EIO;
        }

        err = camera_set_focus_mode(mCameraHandle, CAMERA_FOCUSMODE_CONTINUOUS_MACRO);
		if ( err != CAMERA_EOK){
#ifdef DEBUG
			fprintf(stderr, " Ran into an issue when setting focus mode = %d\n ", err);
#endif
			root["successful"] = false;
			root["state"] = "Set Focus";
			root["error"] = err;
			root["reason"] =  getCameraErrorDesc( err );
			m_pParent->NotifyEvent(event + " " + writer.write(root));
			return EIO;
		}

		err = camera_start_burst(mCameraHandle, NULL, NULL, NULL, &image_callback, NULL);

		if ( err != CAMERA_EOK) {
#ifdef DEBUG
			fprintf(stderr, "Ran into an issue when starting up the camera in burst mode\n");
#endif
			root["successful"] = false;
			root["state"] = "Start Burst";
			root["error"] = err;
			root["reason"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(event + " " + writer.write(root));
			return EIO;
		}
//		templateStartThread();

		return EOK;

    }


    /*
     * QRCodeReaderNDK::stopQRCodeRead
     *
     * This method is called to clean up following successful detection of a QR code.
     * Calling this method will stop the viewfinder and close an open connection to the device camera.
     */
    int QRCodeReaderNDK::stopQRCodeRead() {
        camera_error_t err;
        Json::FastWriter writer;
        Json::Value root;
        std::string event = "community.QRCodeReader.disabledCallback.native";
        root["disabled"] = "true";

        err = camera_stop_burst(mCameraHandle);
		if ( err != CAMERA_EOK)
		{
#ifdef DEBUG
			fprintf(stderr, "Error with turning off the burst \n");
#endif
			root["successful"] = false;
			root["error"] = err;
			root["reason"] = getCameraErrorDesc( err );
			m_pParent->NotifyEvent(event + " " + writer.write(root));
			return EIO;
		}

//        err = camera_stop_video_viewfinder(mCameraHandle);
        err = camera_stop_photo_viewfinder(mCameraHandle);
        if ( err != CAMERA_EOK)
        {
#ifdef DEBUG
            fprintf(stderr, "Error with turning off the photo viewfinder \n");
#endif
            root["successful"] = false;
            root["error"] = err;
            root["reason"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(event + " " + writer.write(root));
            return EIO;
        }

        //check to see if the camera is open, if it is open, then close it
        err = camera_close(mCameraHandle);
        if ( err != CAMERA_EOK){
#ifdef DEBUG
            fprintf(stderr, "Error with closing the camera \n");
#endif
            root["successful"] = false;
            root["error"] = err;
            root["reason"] = getCameraErrorDesc( err );
            m_pParent->NotifyEvent(event + " " + writer.write(root));
            return EIO;
        }

        img_lib_detach(ilib);

        root["successful"] = true;
        root["error"] = EOK;
        root["reason"] = "";
        mCameraHandle = CAMERA_HANDLE_INVALID;
        m_pParent->NotifyEvent(event + " " + writer.write(root));
        return EOK;
    }

    std::string getBMPHeader(const unsigned int width, const unsigned int height) {

    	std::stringstream ss (std::stringstream::in);
    	ss << hex << (width * height);
    	std::string numFileBytes = ss.str();

        ss.str("");
        ss.clear();
    	ss << hex << (width);
    	std::string w = ss.str();

        ss.str("");
        ss.clear();
    	ss << hex << (height);
    	std::string h = ss.str();

    	return "BM" +                    // Signature
    		numFileBytes +            // size of the file (bytes)*
    	    "\x00\x00" +              // reserved
    	    "\x00\x00" +              // reserved
    	    "\x36\x00\x00\x00" +      // offset of where BMP data lives (54 bytes)
    	    "\x28\x00\x00\x00" +      // number of remaining bytes in header from here (40 bytes)
    	    w +                       // the width of the bitmap in pixels*
    	    h +                       // the height of the bitmap in pixels*
    	    "\x01\x00" +              // the number of color planes (1)
    	    "\x20\x00" +              // 32 bits / pixel
    	    "\x00\x00\x00\x00" +      // No compression (0)
    	    "\x00\x00\x00\x00" +      // size of the BMP data (bytes)*
    	    "\x13\x0B\x00\x00" +      // 2835 pixels/meter - horizontal resolution
    	    "\x13\x0B\x00\x00" +      // 2835 pixels/meter - the vertical resolution
    	    "\x00\x00\x00\x00" +      // Number of colors in the palette (keep 0 for 32-bit)
    	    "\x00\x00\x00\x00";       // 0 important colors (means all colors are important)
    }
}
