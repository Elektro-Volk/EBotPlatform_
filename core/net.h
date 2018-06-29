#pragma once
#include "common.h"
#include <map>
#include <curl/curl.h>
#include "lua/luai.h"
#include "nextlist.hpp"
#include <thread>

namespace net {
  struct MemBuf {
    char *memory;
    size_t size;
  };

  class Exception {
  public:
      CURLcode errcode;

      Exception(CURLcode code) { errcode = code; }
      const char* what() { return curl_easy_strerror(errcode); }
  };

  void init();
  void setup_curl(CURL *handle, string *buffer);
  size_t _curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *data);
  string makeFields(map<string, string> &fields);
  string GET(string url);
  string POST(string url, const char* data);
  string POST(string url, map<string, string> &fields);
}
