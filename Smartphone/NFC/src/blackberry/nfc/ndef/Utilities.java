/*
* Copyright 2010 Research In Motion Limited.
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
package widgetpackage;

import net.rim.device.api.crypto.SHA1Digest;
import net.rim.device.api.system.EventLogger;

public class Utilities {

    private static final int UTF_16_TEXT = 0x80;
    private static final int IANA_LANGUAGE_CODE_LEN_MASK = 0x1F;
    private static long MYAPP_ID;

    public static void initLogging(long app_id, String app_name) {
        MYAPP_ID = app_id;
        EventLogger.register(MYAPP_ID, app_name, EventLogger.VIEWER_STRING);
        log("XXXX Logging initialised");
    }

    public static void log(String log_msg) {
        boolean ok = EventLogger.logEvent(MYAPP_ID, log_msg.getBytes(), EventLogger.INFORMATION);
    }

    public static long generateId(String source) {
        String strValue = source;
        SHA1Digest sha1Digest = new SHA1Digest();
        sha1Digest.update(strValue.getBytes());
        byte[] hashValBytes = sha1Digest.getDigest();
        long hashValLong = 0;
        for(int i = 0; i < 8; i++) {
            hashValLong |= ((long) (hashValBytes[i]) & 0x0FF) << (8 * i);
        }
        return hashValLong;
    }

    public static boolean isUtf16Encoded(int status_byte) {
        return(status_byte == (status_byte & UTF_16_TEXT));
    }

    public static int getIanaLanguageCodeLength(int status_byte) {
        return status_byte & IANA_LANGUAGE_CODE_LEN_MASK;
    }
}
