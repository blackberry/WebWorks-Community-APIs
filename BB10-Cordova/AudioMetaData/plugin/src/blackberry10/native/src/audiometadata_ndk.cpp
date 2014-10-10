/*
 * Copyright (c) 2013 BlackBerry Limited
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

#include <pthread.h>
#include "audiometadata_ndk.hpp"
#include "audiometadata_js.hpp"
#include <vector>
#include <unicode/unistr.h>

#define BUF_LEN 512

extern "C" {
#include "id3v2lib/include/id3v2lib.h"
}

using namespace std;

namespace {
    vector<wchar_t> constructUtf16String(uint16_t* string, int numOfBytes, int &strSize) {
        vector<wchar_t> res;
        int i = 1, counter = 2; // Skip the BOM
        while(1) {
            if(numOfBytes > 0 && counter >= numOfBytes) {
                break;
            }

            if(string[i] == 0x0000) {
                break;
            }

            if(string[i] == 0xFFFE || string[i] == 0xFEFF) { //skiping addition bom markers
                continue;
            }

            res.push_back(string[i]);
            i++;
            counter += 2;
        }
        res.push_back(0x0000);
        strSize = (i-1); //not null terminated
        return res;
    }
}

namespace webworks {

AudioMetaData_NDK::AudioMetaData_NDK(AudioMetaData_JS *parent):
    m_pParent(parent) {
    m_pParent->getLog()->info("AudioMetaData Created");
}

AudioMetaData_NDK::~AudioMetaData_NDK() {
}

// Asynchronous callback with mp3 path to get metadata
void AudioMetaData_NDK::audioMetaDataGetMetaData(const std::string& callbackId, const std::string& inputString) {
    m_pParent->getLog()->debug("starting meta-data extraction...");
    Json::FastWriter writer;
    Json::Value root = parseMp3ForMetaData(inputString.c_str());
    m_pParent->NotifyEvent(callbackId + " " + writer.write(root));
}

// Extract metadata from MP3
Json::Value AudioMetaData_NDK::parseMp3ForMetaData(const char* path) {
    Json::Value res;
    ID3v2_tag *tag = load_tag(path);
    if (tag == NULL) {
        res["result"] = "no id3 metadata";
    } else {
        // Currently frames and tags are not deallocated automagically
        // thus if the getters of tags and frames successful
        // the memory must be deallocated manually to avoid mem leak
        ID3v2_frame_text_content* content;
        ID3v2_frame *artist = tag_get_artist(tag);
        if (artist) {
            content = parse_text_frame_content(artist);
            if (content) {
                if (content->size > 0) {
                    res["artist"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(artist);
        }
        ID3v2_frame *name = tag_get_title(tag);
        if (name) {
            content = parse_text_frame_content(name);
            if (content) {
                if (content->size > 0) {
                    res["title"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(name);
        }
        ID3v2_frame *year = tag_get_year(tag);
        if (year) {
            content = parse_text_frame_content(year);
            if (content) {
                if (content->size > 0) {
                    res["year"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(year);
        }
        ID3v2_frame *album = tag_get_album(tag);
        if (album) {
            content = parse_text_frame_content(album);
            if (content) {
                if (content->size > 0) {
                    res["album"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(album);
        }
        ID3v2_frame *genre = tag_get_genre(tag);
        if (genre) {
            content = parse_text_frame_content(genre);
            if (content) {
                if (content->size > 0) {
                    res["genre"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(genre);
        }
        ID3v2_frame *track = tag_get_track(tag);
        if (track) {
            content = parse_text_frame_content(track);
            if (content) {
                if (content->size > 0) {
                    res["track"] = getProperString(content->data, content->size, content->encoding);
                }
                free(content);
            }
            free(track);
        }
        free(tag);
    }
    return res;
}


string AudioMetaData_NDK::getProperString(char* strArray, int size, char encoding) {
    string result = "";
    if (encoding == UTF_16_ENCODING) {
        int utf16StrSize = 0;
        char utf8Buff[BUF_LEN];
        memset(utf8Buff, 0, sizeof(utf8Buff));

        //Convert provided Unicode(UCS-2) encoded str into printable format
        vector<wchar_t> utf16Vec = constructUtf16String((uint16_t*)strArray, size, utf16StrSize);
        // UTF-32 casting increases the scope of character coverage
        UnicodeString str = UnicodeString::fromUTF32((UChar32*)&utf16Vec[0], utf16Vec.size());
        CheckedArrayByteSink byte_buff(utf8Buff, sizeof(utf8Buff));
        str.toUTF8(byte_buff); // convert UTF-16 to UTF-8 to be able to stringify the result

        result = string(utf8Buff);
    } else {
        result = string(strArray, size);
    }
    return result;
}

} /* namespace webworks */
