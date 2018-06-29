#include "luawork.h"
#include "luai.h"
#include "rapidjson/document.h"
#include "cvars.h"
#include "console.h"
#include "cmd.h"
#include "luaModules.h"
#include "luapool.h"

bool luawork::isWorking = false;
lua_State *luawork::state;
cvar luawork::lua_script;

void luawork::init()
{
  lua_script = cvars::add("lua_script", "main.lua");
  cmd::add("relua", luawork::c_relua, "Reload lua scripts");
  cmd::add("mlist", luaModules::c_mlist, "Lua modules list");
  luapool::init();
}

void init_lua_api(lua_State *L);
void luawork::start()
{
    try {
        luaModules::load();
        state = luaL_newstate();
        luaL_openlibs(state);
        init_lua_api(state);

        luaModules::loadModules(state);
        if(luaL_loadfile(state, (bot_path + "/scripts/" + lua_script->getString()).c_str()))
            throw new string("lua[load] -> " + string(lua_tostring(state, -1)));
        if(lua_pcall(state, 0, LUA_MULTRET, 0))
            throw new string("lua[start] -> " + string(lua_tostring(state, -1)));
        luaModules::startModules(state);
        luapool::start();
        luawork::isWorking = true;
        con::log("Скрипты были успешно запущены.");
    }
    catch(string *str) {
        luawork::isWorking = false;
        con::error(*str);
        con::error("Исправьте все ошибки и перезагрузите скрипты. (relua)");
        delete str;
    }
}

void luawork::push(rapidjson::Value &msg)
{
  if(luawork::isWorking) luapool::add(msg);
}

bool luawork::safeCall(lua_State* L, int argnum, int retnum)
{
  if(lua_pcall(L, argnum, retnum, 0)) {
    con::error("lua -> " + string(lua_tostring(L, -1)));
    return false;
  }
  return true;
}

void luawork::close()
{
  luawork::isWorking = false;
  luapool::close();
  luaModules::clearModules();
  lua_close(state);
}

string luawork::c_relua(vector<string> _args)
{
  luawork::close();
  luawork::start();
  return "Lua has been reloaded.";
}
