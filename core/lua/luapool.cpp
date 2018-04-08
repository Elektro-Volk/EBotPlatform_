#include "luawork.h"
#include "luapool.h"
#include "luai.h"
#include "lJson.h"
#include "lAPI.h"
#include "../cvar.h"
#include "../console.h"
#include "../cmd.h"


char luapool::nPools;
std::vector<luapool::pool*> luapool::pools;

int luapool::poolsCount = 2;
int luapool::poolsSleep = 10;

void luapool::init()
{
  //cvars::add("pl_count", "2");//->intCallback = &luapool::poolsCount;
 // cvars::add("pl_sleep", "10");//->intCallback = &luapool::poolsSleep;
  //cvars::add("a", "adad");
  cmd::add("pools", luapool::c_pools, "Lua pools list");
}

void luapool::start()
{
  for(int i = 0; i < poolsCount; i++)
    pools.push_back(new pool());
}

string luapool::c_pools(vector<string> _args)
{
  std::string ret = "\nLua pools:";
	for (int i = 0; i < luapool::pools.size(); i++)
	{
		ret += "\n" + to_string(i) + " - " + (luapool::pools[i]->isFree() ? "free" : "busy");
	}
	return ret;
}

void luapool::close()
{
  con::log("Closing pools...");
  for(int i=0; i<pools.size(); i++)
    delete pools[i];
  pools.clear();
  con::log("Pools closed.");
}

bool pushValue(lua_State *L, const rapidjson::Value &value);
void luapool::add(rapidjson::Value &msg)
{
  // find free pool and add msg then
  auto ms = std::chrono::milliseconds(1);
  pool *freePool = nullptr;
  while(!freePool){
    for(int i = 0; i < pools.size(); i++) {
      std::this_thread::sleep_for(ms);
      if(!pools[i]->isFree()) continue;
      freePool = pools[i];
      break;
    }
  }
  freePool->add(msg);
}


// Class
luapool::pool::pool()
:enabled(true), have(false), thread(&luapool::pool::loop, this)
{
  // params
  this->id = nPools++;
  // Lua thread
	//lua_lock(G_L);
	this->L = lua_newthread(luawork::state);
	lua_setfield(luawork::state, LUA_REGISTRYINDEX, &this->id);
	//lua_unlock(G_L);
}

bool luapool::pool::isFree()
{
  return !this->have;
}

void luapool::pool::add(rapidjson::Value &msg)
{
  std::unique_lock<std::mutex> locker(mutex);
  pushValue(L, msg);
  this->have = true;
  cv.notify_one();
}

void luapool::pool::loop()
{
  while (enabled) {
    std::unique_lock<std::mutex> locker(mutex);
		cv.wait(locker, [&](){ return have || !enabled; });

		if (!this->have) continue;
    this->have = false;

    locker.unlock();
    lua_getglobal(L, "NewMessage");
    lua_pushvalue (L, 1);
    luawork::safeCall(L, 1);
    lua_settop(L, 0);
    locker.lock();
	}
}
