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

#include <json/reader.h>
#include <json/writer.h>
#include <pthread.h>
#include "audiometadata_ndk.hpp"
#include "audiometadata_js.hpp"

extern "C" {
#include "id3v2lib/include/id3v2lib.h"
}

namespace {
    // Extract metadata from MP3
    Json::Value parseMp3ForMetaData(const char* path) {
        Json::Value res;
        ID3v2_tag *tag = load_tag(path);
        if (tag == NULL) {
            res["result"] = "no-data";
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
                        res["artist"] = string(content->data, content->size);
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
                        res["title"] = string(content->data, content->size);
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
                        res["year"] = string(content->data, content->size);
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
                        res["album"] = string(content->data, content->size);
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
                        res["genre"] = string(content->data, content->size);
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
                        res["track"] = string(content->data, content->size);
                    }
                    free(content);
                }
                free(track);
            }
            free(tag);
        }
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

} /* namespace webworks */
