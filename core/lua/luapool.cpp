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

int luapool::poolsCount = 3;
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
  for(int i=pools.size(); i<pools.size(); i++) {
    pools[i]->sRemove();
    while(true) if(!pools[i]->active) break;
    delete pools[i];
  }
  pools.clear();
  con::log("Pools closed.");
}

bool pushValue(lua_State *L, const rapidjson::Value &value);
void luapool::add(rapidjson::Value &msg)
{
  // find free pool and add msg then
  pool *freePool = nullptr;
  while(!freePool){
    for(int i = 0; i < pools.size(); i++) {
      if(!pools[i]->isFree()) continue;
      freePool = pools[i];
      break;
    }
  }

  pushValue(luawork::state, msg);
  freePool->add();
}


// Class
luapool::pool::pool()
{
  // params
  this->id = nPools++;
  this->free = true;
	this->toremove = false;
  this->have = false;
  this->active = true;
  // Lua thread
	//lua_lock(G_L);
	this->L = lua_newthread(luawork::state);
	lua_setfield(luawork::state, LUA_REGISTRYINDEX, &this->id);
	//lua_unlock(G_L);
  // C thread
	std::thread t(&luapool::pool::loop, this);
	t.detach();
}

bool luapool::pool::isFree()
{
  return this->free;
}

void luapool::pool::sRemove()
{
  toremove = true;
}

void luapool::pool::add()
{
  this->free = false;
  lua_settop(L, 0);
  lua_xmove (luawork::state, L, 1);
  this->have = true;
}

void luapool::pool::loop()
{
  auto ms = std::chrono::milliseconds(poolsSleep);
  while (!toremove) {
		if (this->have && !this->free) {
      lua_getglobal(L, "CheckMessage");
      lua_pushvalue (L, 1);
      luawork::safeCall(L, 1, 1);
      if(!lua_isnil(L, -1)) {
        lua_getglobal(L, "NewMessage");
        lua_pushvalue (L, 2);
        luawork::safeCall(L, 1);
      }
      lua_settop(L, 0);
    }

		this->free = true;
    this->have = false;
		std::this_thread::sleep_for(ms);
	}
  active = false;
}
