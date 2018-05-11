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

  class CurlObject {
  private:
      LSECT _lock;
  public:
      char* name;
      CURL *handle;

      CurlObject()
      {
        _lock = lock_new();
        handle = curl_easy_init();
      }

      ~CurlObject()
      {
        curl_easy_cleanup(handle);
        lock_free(_lock);
      }

      void lock()
      {
          lock_lock(_lock);
      }

      void unlock()
      {
          lock_unlock(_lock);
      }

      void flush()
      {
          curl_easy_reset(handle);
      }
  };

  class Request {
  private:
    string url;
    const char* data = nullptr;

  public:
    map<const char*, const char*> mimes;
    const char** headers = nullptr;
    char nHeaders = 0;

    Request(string url);
    void setData(const char* data);
    string send();
  };

  extern std::thread::id main_thread_id;
  extern Nextlist<thread::id, net::CurlObject*> curls;

  void init();
  CurlObject *getCurlObject();
  size_t _curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *data);
  char* makeFields(map<string, string> &fields);
  string GET(string url);
  string POST(string url, const char* data);
  string POST(string url, map<string, string> &fields);
}
