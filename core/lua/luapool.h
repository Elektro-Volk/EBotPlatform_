#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "rapidjson/document.h"
#include "luai.h"

namespace luapool {
  class pool {
    char id;
    bool have;
    bool enabled;
    lua_State* L;
    void loop();
  public:
    pool();
    ~pool()
  	{
  		enabled = false;
  		cv.notify_one();
  		thread.join();
  	}
    void start();
    bool isFree();
    void sRemove();
    void add(rapidjson::Value &msg);
  private:
    std::condition_variable		cv;
    std::mutex				mutex;
    std::thread				thread;	
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
