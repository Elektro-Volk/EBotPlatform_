#pragma once
#include "../common.h"
#include "rapidjson/document.h"
#include "../lua/lJson.h"
#include <map>

namespace vk {
  class Request {
  private:
    string url;
  public:
    Request(string url);

    char* send();
  };

  extern string token;
  extern string version;

  void init();
  void start();
  string send(string method, map<string, string> params = {});
  rapidjson::Document jSend(string method, map<string, string> params = {});

  namespace lua {
    void initVK(lua_State *L);
    int __index(lua_State *L);
    int __indexMethod(lua_State *L);
    int __call(lua_State *L);
    int send(lua_State *L);
    int jSend(lua_State *L);
  }
}
