#include "luawork.h"
#include "luaModules.h"
#include "../console.h"
#include "../json.hpp"
#include "../filesystem.h"
#include "lAPI.h"


using json = nlohmann::json;

vector<luaModules::Module> luaModules::list;

string luaModules::c_madd(vector<string> cmd_args)
{
	if (cmd_args.size() != 2)
		throw new string("use: madd <name>");
	list.push_back({cmd_args[1], false});
	save();
	return "Module has been added to list";
}

string luaModules::c_mrem(vector<string> cmd_args)
{
	if (cmd_args.size() != 2)
		throw new string("use: mrem <name>");
	for (int i = 0; i < list.size(); i++)
		if(list[i].name == cmd_args[1]) {
			list.erase(list.begin() + i);
			break;
		}
	save();
	return "Module has been removed from list";
}

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
	if (fs::exists("scripts/modules/list.json")) {
		json loaddata = json::parse(fs::read("scripts/modules/list.json"));
		for (auto e = loaddata.begin(); e != loaddata.end(); ++e) {
			string name = *e;
			list.push_back({ name, false });
		}
	}
}

void luaModules::save()
{
	json savedata;
	for (int i = 0; i < list.size(); i++)
		savedata.push_back(list[i].name);
	fs::write("scripts/modules/list.json", savedata.dump());
}
