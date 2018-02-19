#pragma once
#include "common.h"
#include "nextlist.hpp"
#include "lua/luai.h"

namespace cvars {
  class Cvar {
  private:
    string value;
  public:
    string name;
    int* intCallback = nullptr;
    float* floatCallback = nullptr;
    string* stringCallback = nullptr;
    bool* boolCallback = nullptr;
    void(*funcCallback)(Cvar* cvar, string new_value) = nullptr;

    Cvar(string name, string value);
    void setValue(string new_value);
    string getValue();
  };
  extern Nextlist<string, Cvar*> cvars;

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
