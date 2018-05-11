#pragma once
#include <string>
#include <map>
#include "lua/luai.h"

using namespace std;

namespace cvars {
  class Cvar {
  public:
    string value;
    string name;
    string* stringCallback = nullptr;

    Cvar(string name, string value);
    void setValue(string new_value);
    // Getters
    bool getBool();
  };
  extern map<string, Cvar*> cvars;

  Cvar *add(string name, string* value);
  Cvar *add(string name, string value);
  Cvar *get(string name);
  bool exists(string name);
  void remove(Cvar *cvar);
}

typedef cvars::Cvar* cvar;
