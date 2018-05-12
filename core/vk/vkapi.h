#pragma once
#include "../common.h"
#include "rapidjson/document.h"
#include "../lua/lua_json.h"
#include <map>

namespace vk {
  class Request {
  private:
    string url;
  public:
    Request(string url);

    char* send();
  };

  extern cvars::Cvar *token;
  extern cvars::Cvar *version;
  extern cvars::Cvar *groupmode;
  extern cvars::Cvar *groupid;
  extern cvars::Cvar *groupid;

  void init();
  void start();
  string send(string method, map<string, string> params = {});
  rapidjson::Document jSend(string method, map<string, string> params = {});
}
