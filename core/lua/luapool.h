#pragma once
#include <vector>
#include "rapidjson/document.h"
#include "luai.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#ifdef __MINGW32__
  #include "../mingwthreads/mingw.mutex.h"
  #include "../mingwthreads/mingw.condition_variable.h"
#endif

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

    extern cvar pool_count;

    void init();
    void start();
    void close();
    void add(rapidjson::Value &msg);
    string c_pool(vector<string> _args);
}
