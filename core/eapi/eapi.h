#pragma once
#include "../common.h"
#include "../lua/luai.h"
#include <map>

namespace eapi {
  extern string key;
  extern string url;

  void init();
  void start();
  void gentoken();

  namespace lua {
    void getstats(lua_State *L);
  }
}
