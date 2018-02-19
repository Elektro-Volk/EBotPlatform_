#pragma once
#include <thread>
#include <vector>
#include "../json.hpp"
#include "luai.h"

namespace luapool {
  class pool {
    char id;
    bool free;
    bool have;
    bool toremove;
    lua_State* L;
    void loop();
  public:
    bool active;

    pool();
    void start();
    bool isFree();
    void sRemove();
    void add();
  };

  extern char nPools;
  extern std::vector<pool*> pools;

  void start();
  void close();
  void add(nlohmann::json msg);
}
