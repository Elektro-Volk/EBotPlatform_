#include "luawork.h"
#include "luapool.h"
#include "luai.h"
#include "lJson.h"
#include "lAPI.h"
#include "../cvar.h"
#include "../console.h"
#include "../cmd.h"
#include "../net.h"
//#include "../../lua/lua_user.h" //lua_lock, lua_unlock

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

void luapool::add(rapidjson::Value &msg)
{
  // find free pool and add msg then
  pool *freePool = nullptr;
  while(!freePool){
    for(int i = 0; i < pools.size(); i++) {
      if(pools[i]->isBusy()) continue;
      freePool = pools[i];
      break;
    }
  }
  freePool->add(msg);
}


// Class
luapool::pool::pool()
:enabled(true), busy(false), thread(&luapool::pool::loop, this)
{
  net::curls.push(thread.get_id(), new net::CurlObject());

  id[0] = nPools++;
	L = lua_newthread(luawork::state);
	lua_setfield(luawork::state, LUA_REGISTRYINDEX, id);
}

luapool::pool::~pool()
{
  enabled = false;
  delete net::curls[thread.get_id()];
  net::curls.pop(thread.get_id());
  cv.notify_one();
  thread.join();
}

bool luapool::pool::isBusy()
{
  return busy;
}

bool pushValue(lua_State *L, const rapidjson::Value &value);
void luapool::pool::add(rapidjson::Value &msg)
{
  std::unique_lock<std::mutex> locker(mutex);
  lua_unlock(L);
  lua_settop(L, 0);
  lua_getglobal(L, "NewMessage");
  pushValue(L, msg);
  lua_lock(L);
  this->have = true;
  cv.notify_one();
}

void luapool::pool::loop()
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
