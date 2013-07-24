/*
 * Copyright 2011-2012 Research In Motion Limited.
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

/**
 * List of the URL prefixes. 
 */
public class URLPrefixes {
    private final static String [] prefixes;
    
    private static void add(int position, String value) {
        if (position < 0 || position > 255) {
            log("Error in initialisation: " + position);
        }
        prefixes[position] = value;
    }

    static {
        prefixes = new String[0x24];
        
        add(0,"");
        add(0x01,"http://www.");
        add(0x02,"https://www.");
        add(0x03,"http://");
        add(0x04,"https://");
        add(0x05,"tel:");
        add(0x06,"mailto:");
        add(0x07,"ftp://anonymous:anonymous@");
        add(0x08,"ftp://ftp.");
        add(0x09,"ftps://");
        add(0x0A,"sftp://");
        add(0x0B,"smb://");
        add(0x0C,"nfs://");
        add(0x0D,"ftp://");
        add(0x0E,"dav://");
        add(0x0F,"news:");
        add(0x10,"telnet://");
        add(0x11,"imap:");
        add(0x12,"rtsp://");
        add(0x13,"urn:");
        add(0x14,"pop:");
        add(0x15,"sip:");
        add(0x16,"sips:");
        add(0x17,"tftp:");
        add(0x18,"btspp://");
        add(0x19,"btl2cap://");
        add(0x1A,"btgoep://");
        add(0x1B,"tcpobex://");
        add(0x1C,"irdaobex://");
        add(0x1D,"file://");
        add(0x1E,"urn:epc:id:");
        add(0x1F,"urn:epc:tag:");
        add(0x20,"urn:epc:pat:");
        add(0x21,"urn:epc:raw:");
        add(0x22,"urn:epc:");
        add(0x23,"urn:nfc:");
    }
    
    
    public static String getPrefix(int number) {
        if (number<0 || number>= prefixes.length) {
            log("Bad index - returning null: " + number);
            return null;
        }
        String toReturn = prefixes[number];
        if (toReturn==null) {
            log("Warning: unknown / invalid prefix requested :" + number);
            return "";
        }
        return toReturn;
    }
    
    private static void log(String message) {
        Utilities.log(URLPrefixes.class.getName() + ": " + message);
    }
}
