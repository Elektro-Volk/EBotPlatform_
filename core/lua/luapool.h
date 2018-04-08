#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "rapidjson/document.h"
#include "luai.h"

namespace luapool {
  class Worker {
    char id[1];
    bool busy;
    bool enabled;
    lua_State* L;
    void loop();
  public:
    Worker();
    ~Worker();
    void start();
    bool isBusy();
    void add(rapidjson::Value &msg);
  private:
    std::condition_variable		cv;
    std::mutex				mutex;
    std::thread				thread;
  };

  extern char nWorkers;
  extern std::vector<Worker*> workers;
  // Settings
  extern int workersCount;

  void init();
  void start();
  void close();
  void add(rapidjson::Value &msg);
  string c_pool(vector<string> _args);
}
