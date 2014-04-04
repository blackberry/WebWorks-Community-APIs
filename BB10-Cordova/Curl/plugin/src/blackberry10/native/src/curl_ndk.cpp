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

#include <json/reader.h>
#include "curl_js.hpp"
#include "curl_ndk.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <sstream>

#include <slog2.h>

namespace webworks {

  CurlNDK::CurlNDK(CurlJS *parent) {
    m_pParent = parent;
  }

  CurlNDK::~CurlNDK() {
  }
  struct sstring {
    char *ptr;
    size_t len;
  };

  void
  init_string(struct sstring *s) {
    s->len = 0;
    s->ptr = (char*) malloc(s->len + 1);
    if (s->ptr == NULL) {
      fprintf(stderr, "malloc() failed\n");
      exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
  }

  size_t
  writefunc(void *ptr, size_t size, size_t nmemb, struct sstring *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = (char*) realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
      fprintf(stderr, "realloc() failed\n");
      exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
  }

// Take in JSON of value (bool) and handle (string) and return a message when complete
  std::string
  CurlNDK::get(const std::string& inputString) {
    Json::Reader reader;
    Json::Value root;
    bool parse = reader.parse(inputString, root);

    if (!parse) {
      return "Cannot parse JSON object for Prevent Sleep";
    }
    string arg = root["value"].asCString();
    //log
    slog2_buffer_set_config_t buffer_config;
    slog2_buffer_t buffer_handle[2];
    int some_number = 5108;
    buffer_config.buffer_set_name = "CURLJS";
    buffer_config.num_buffers = 1;
    buffer_config.verbosity_level = SLOG2_INFO;
    buffer_config.buffer_config[0].buffer_name = "INFO";
    buffer_config.buffer_config[0].num_pages = 7;
    if (-1 == slog2_register(&buffer_config, buffer_handle, 0)) {
      return "Error registering slogger2 buffer!";
    }

    slog2c(buffer_handle[0], 0, SLOG2_INFO, "----CURLJS LOG START----");
    //end log

    slog2c(buffer_handle[0], 0, SLOG2_INFO, arg.c_str());

    CURL* curl; //our curl object
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
      struct sstring s;
      init_string(&s);

      curl_easy_setopt(curl, CURLOPT_URL, arg.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
      //curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3L);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
        slog2c(buffer_handle[0], 0, SLOG2_INFO, curl_easy_strerror(res));
        free(s.ptr);
        curl_easy_cleanup(curl);
        return curl_easy_strerror(res);
      }
      else {
        stringstream ss;
        ss << s.ptr;
        free(s.ptr);
        curl_easy_cleanup(curl);
        //slog2c(buffer_handle[0], 0, SLOG2_INFO, ss.str().c_str());
        return ss.str();
      }
    }
    else {
      return "CURL INIT ERROR";
    }
  }
} /* namespace webworks */
