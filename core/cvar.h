#pragma once
#include "common.h"
#include <map>
#include "lua/luai.h"

namespace cvars {
  class Cvar {
  public:
    string value;
    string name;
    string* stringCallback = nullptr;

    Cvar(string name, string value);
    void setValue(string new_value);
    string getValue();
  };
  extern map<string, Cvar*> cvars;

  Cvar *add(string name, string* value);
  Cvar *add(string name, string value);
  Cvar *get(string name);
  bool exists(string name);
  void remove(Cvar *cvar);
  namespace lua {
    int set(lua_State *L);
    int get(lua_State *L);
    int exists(lua_State *L);
  }
}
