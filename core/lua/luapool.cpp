#include "luawork.h"
#include "luapool.h"
#include "luai.h"
#include "lJson.h"
#include "lAPI.h"
#include "../cvar.h"
#include "../console.h"
#include "../cmd.h"
#include "../net.h"

char luapool::nWorkers;
std::vector<luapool::Worker*> luapool::workers;

int luapool::workersCount = 2;

void luapool::init()
{
  //cvars::add("pl_count", "2");//->intCallback = &luapool::poolsCount;
 // cvars::add("pl_sleep", "10");//->intCallback = &luapool::poolsSleep;
  //cvars::add("a", "adad");
  cmd::add("pool", luapool::c_pool, "Lua pool list");
}

void luapool::start()
{
  nWorkers = 0;
  for(int i = 0; i < workersCount; i++)
    workers.push_back(new Worker());
}

string luapool::c_pool(vector<string> _args)
{
  string ret = "\nLua pools:";
	for (int i = 0; i < workers.size(); i++)
	{
		ret += "\n" + to_string(i) + " - "
    + (workers[i]->isBusy() ? "busy" : "free");
	}
	return ret;
}

void luapool::close()
{
  for(int i = 0; i < workers.size(); i++)
    delete workers[i];
  workers.clear();
}

void luapool::add(rapidjson::Value &msg)
{
  // find free pool and add msg then
  Worker *freeWorker = nullptr;
  while(!freeWorker){
    for(int i = 0; i < workers.size(); i++) {
      if(workers[i]->isBusy()) continue;
      freeWorker = workers[i];
      break;
    }
  }
  freeWorker->add(msg);
}

/***** Worker class *****/
luapool::Worker::Worker()
:enabled(true), busy(false), thread(&luapool::Worker::loop, this)
{
  net::curls.push(thread.get_id(), new net::CurlObject());

  id[0] = nWorkers++;
	L = lua_newthread(luawork::state);
	lua_setfield(luawork::state, LUA_REGISTRYINDEX, id);
}

luapool::Worker::~Worker()
{
  enabled = false;
  delete net::curls[thread.get_id()];
  net::curls.pop(thread.get_id());
  cv.notify_one();
  thread.join();
}

bool luapool::Worker::isBusy()
{
  return busy;
}

bool pushValue(lua_State *L, const rapidjson::Value &value);
void luapool::Worker::add(rapidjson::Value &msg)
{
  std::unique_lock<std::mutex> locker(mutex);
  lua_unlock(L);
  lua_settop(L, 0);
  lua_getglobal(L, "NewMessage");
  pushValue(L, msg);
  lua_lock(L);
  busy = true;
  cv.notify_one();
}

void luapool::Worker::loop()
{
  while (enabled) {
    std::unique_lock<std::mutex> locker(mutex);
		cv.wait(locker, [&](){ return busy || !enabled; });

		if (!busy) continue;
    busy = false;

    lua_unlock(L);
    luawork::safeCall(L, 1);
    lua_lock(L);
	}
}
