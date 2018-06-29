#pragma once
#include "../common.h"
#include "luai.h"
#include <vector>
#include "rapidjson/document.h"

namespace luawork {
  extern lua_State *state;
  extern bool isWorking;
  extern cvar lua_script;

  void init();
  void start();
  void push(rapidjson::Value &msg);
  bool safeCall(lua_State* L, int argnum = 0, int retnum = 0);
  void close();
  string c_relua(vector<string> _args);
}
