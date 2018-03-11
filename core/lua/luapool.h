#pragma once
#include <thread>
#include <vector>
#include "rapidjson/document.h"
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
  // Settings
  extern int poolsCount;
  extern int poolsSleep;

  void init();
  void start();
  void close();
  void add(rapidjson::Value &msg);
  string c_pools(vector<string> _args);
}
