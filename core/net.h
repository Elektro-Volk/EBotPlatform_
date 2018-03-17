#pragma once
#include "common.h"
#include <map>
#include "lua/luai.h"

namespace net {
  struct MemBuf {
    char *memory;
    size_t size;
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

  void init();
  char* makeFields(map<string, string> &fields);
  string GET(string url);
  string POST(string url, const char* data);
  string POST(string url, map<string, string> &fields);

  namespace lua {
    int send(lua_State *L);
    int jSend(lua_State *L);
    int getParams(lua_State *L);
    int UrlEncode(lua_State *L);
  }
}
