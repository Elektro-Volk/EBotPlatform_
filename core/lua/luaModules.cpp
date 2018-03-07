#include "luawork.h"
#include "luaModules.h"
#include "../console.h"
#include "../filesystem.h"
#include "lAPI.h"

vector<luaModules::Module> luaModules::list;

string luaModules::c_mlist(vector<string> cmd_args)
{
	std::string ret = "\nLua modules:";
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		ret += "\n" + it->name + " - " + (it->isLoaded ? "working" : "error");
	}
	return ret;
}

void luaModules::loadModules(lua_State *L)
{
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		lua_pushfstring(L, "scripts/modules/%s", it->name.c_str());
		if (lapi::funcs::connect(L)) {
			it->isLoaded = !lua_isstring(L, -1);
			if (!it->isLoaded)
					con::error("lua[Loading  " + it->name + "] -> " + lua_tostring(L, -1));
			lua_setglobal(L, it->name.c_str());
		}
		lua_settop(L, 0);
	}
}

void luaModules::startModules(lua_State *L)
{
  for (auto it = list.begin(); it != list.end(); ++it)
	{
		lua_getglobal(L, it->name.c_str());
    lua_getfield(L, -1, "Start");
    if(!lua_isnil(L, -1))
      if(lua_pcall(L, 0, LUA_MULTRET, 0))
        con::error("lua[Starting " + it->name + "] -> " + string(lua_tostring(L, -1)));
		lua_settop(L, 0);
	}
}


void luaModules::load()
{
	vector<string> loaddata = fs::dirList("scripts/modules");
	for (auto e : loaddata) {
		string name = e;
		name.erase( name.end() - 1 );
		name.erase( name.end() - 1 );
		name.erase( name.end() - 1 );
		name.erase( name.end() - 1 );// TODO
		list.push_back({ name, false });
	}
}
