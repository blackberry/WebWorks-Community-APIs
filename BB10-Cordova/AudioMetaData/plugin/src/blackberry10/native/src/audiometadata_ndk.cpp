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
#include <cctype> // isdigit()
#include <algorithm> // remove_if()
#include <unicode/unistr.h>

#define BUF_LEN 512

extern "C" {
#include "id3v2lib/include/id3v2lib.h"
}

using namespace std;

/*
 *  Genre strings for ID3 tags, most used in ID3v1 but can be used in v2. Currently contains 0 - 79
 *  for more information visit http://en.wikipedia.org/wiki/ID3.
 *
 */
static const string genre_strings[] = {
        "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop",
        "Jazz", "Metal", "New Age", "Oldies", "Other", "Pop", "Rhythm and Blues", "Rap",
        "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
        "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz & Funk", "Fusion", "Trance",
        "Classical", "Instrumental", "Acid", "House", "Game", "Sound Clip", "Gospel", "Noise",
        "Alternative Rock", "Bass", "Soul", "Punk rock", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
        "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream",
        "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
        "Native American", "Cabaret", "New Wave", "Psychedelic", "Rave", "Showtunes", "Trailer", "Lo-Fi",
        "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll", "Hard Rock"
};

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

            if(string[i] == 0xFFFE || string[i] == 0xFEFF) { //skiping additional bom markers
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

    bool isNotDigit(char ch) {
        return !isdigit(ch);
    }

    /*
     * not the same as converting string to number. Since we are parsing the specials characters
     * from the string first.
     */
    int extractNumberFromString(const string& str) {
        string temp = str;
        temp.erase(std::remove_if(temp.begin(), temp.end(), isNotDigit), temp.end());
        return (temp.length() > 0) ? atoi(temp.c_str()) : -1; // if temp is empty, then no number is found
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
    m_pParent->getLog()->debug(inputString.c_str());
    Json::FastWriter writer;
    Json::Value root = parseMp3ForMetaData(inputString.c_str());
    m_pParent->getLog()->debug(writer.write(root).c_str());
    m_pParent->NotifyEvent(callbackId + " " + writer.write(root));
}

string AudioMetaData_NDK::audioMetaDataSetTagData(const std::string& inputString) {
    m_pParent->getLog()->debug("Setting ID3 metadata");
    Json::Reader reader;
    Json::Value root;
    bool parsed = reader.parse(inputString, root);

    if (parsed) {
        const char* path = "";
        Json::Value pathVal = root["path"];
        if (pathVal.isArray()) {
            path = (*pathVal.begin()).asString().c_str();
        } else {
            path = pathVal.asString().c_str();
        }
        setTagData(path, root);
        return "metadata set";
    } else {
        return "failed to parsed input json data";
    }
}

string AudioMetaData_NDK::audioMetaDataRemoveTag(const std::string& inputString) {
    m_pParent->getLog()->debug("removing ID3 metadata");
    const char* path = inputString.c_str();
    ID3v2_tag *tag = load_tag(path);
    if (tag == NULL) {
        return "No metadata found";
    } else {
        remove_tag(path);
        free(tag);
        return "successfully removed metadata";
    }
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
                    string genreStr = getProperString(content->data, content->size, content->encoding);
                    int genreNum = extractNumberFromString(genreStr); //checking for numeric genre
                    res["genre"] = (genreNum != -1) ? genre_strings[genreNum] : genreStr;
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

void AudioMetaData_NDK::setTagData(const char* path, const Json::Value &data) {
    Json::Value res;
    Json::FastWriter writer;

    // check if tag already exists on the file
    ID3v2_tag *tag = load_tag(path);
    if (tag == NULL) {
        tag = new_tag();
    }

    // iterate through the input data to see what needs to be changed
    Json::Value::iterator it = data.begin();
    for (; it != data.end(); it++) {

        const char* key = it.memberName();
        // note: due to the fact that a path can be passed in as
        // a string or a JS object, to avoid errors in "const char* value = (*it).asString().c_str();"
        // line, this check is needed.
        if (strcmp(key, "path") == 0) {
            continue;
        }

        const char* value = (*it).asString().c_str();
        m_pParent->getLog()->debug(value);
        int size = strlen(value);
        char buff[size+1];
        memset(buff,0,size+1);
        strncpy(buff, value, size);

        //todo: handle different encodings

        if (strcmp(key, "title") == 0) {
            tag_set_title(buff, ISO_ENCODING, tag);
        } else if (strcmp(key, "artist") == 0) {
            tag_set_artist(buff, ISO_ENCODING, tag);
        } else if (strcmp(key, "genre") == 0) {
            tag_set_genre(buff, ISO_ENCODING, tag);
        } else if (strcmp(key, "year") == 0) {
            tag_set_year(buff, ISO_ENCODING, tag);
        } else if (strcmp(key, "album") == 0) {
            tag_set_album(buff, ISO_ENCODING, tag);
        } else if (strcmp(key, "track") == 0) {
            tag_set_track(buff, ISO_ENCODING, tag);
        }
    }
    set_tag(path, tag);
    free(tag);
}

} /* namespace webworks */
