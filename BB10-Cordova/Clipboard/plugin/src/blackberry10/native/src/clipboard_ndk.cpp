/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#include <string>
#include <sstream>
#include <json/reader.h>
#include <screen/screen.h>
#include "clipboard_js.hpp"
#include "clipboard_ndk.hpp"
#include <clipboard/clipboard.h>

namespace webworks {

ClipboardNDK::ClipboardNDK(ClipboardJS *parent) {
	m_pParent = parent;
}

ClipboardNDK::~ClipboardNDK() {
}

// Take in JSON of value (bool) and handle (string) and return a message when complete
std::string ClipboardNDK::setText(const std::string& inputString) {
    Json::Reader reader;
    Json::Value root;
    bool parse = reader.parse(inputString, root);

    if (!parse) {
      return "Cannot parse JSON object for Prevent Sleep";
    }
    string arg=root["value"].asCString();

  if (get_clipboard_can_write() == 0) {
    //could write clip board.

    //always empty clip board.
    empty_clipboard();

    //write to the clip board
    int ret=set_clipboard_data("text/plain", arg.size(), arg.c_str());
    stringstream ss;
    ss<<ret;
    return ss.str();
  } else {
    return "-1";
  }
}

// Get the prevent sleep state
std::string ClipboardNDK::getText() {
  char* pbuffer=0;
  int ret=get_clipboard_data("text/plain", &pbuffer);
  if (ret>0){
    //return the clip board string.
    std::string data(pbuffer);
    free(pbuffer);
    return data;
  }else{
    //something bad happened,return empty string.
    free(pbuffer);
    return "";
  }
}

} /* namespace webworks */
