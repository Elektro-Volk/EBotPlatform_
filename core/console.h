#pragma once
#include "common.h"
#include "lua/luai.h"

namespace col { 
  extern const string BOLD;
  extern const string red;
  extern const string RED;
  extern const string GREEN;
  extern const string green;
  extern const string YELLOW;
  extern const string yellow;
  extern const string blue;
  extern const string BLUE;
  extern const string MAGENTA;
  extern const string magenta;
  extern const string cyan;
  extern const string CYAN;
  extern const string NC;
}

namespace con {
  void init();
  void log(string data);
  void error(string data);
  namespace lua {
    int log(lua_State *L);
    int error(lua_State *L);
  }
}
