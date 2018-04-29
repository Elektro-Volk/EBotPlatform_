#include "lAPI.h"
#include "luawork.h"
#include <vector>
// lAPI
#include "../cmd.h"
#include "../cvar.h"
#include "../console.h"
#include "../net.h"
#include "../vk/vkapi.h"
#include "../filesystem.h"
#include "../mysql_work.h"

struct rfunc {
  const char* name;
  int(*func)(lua_State *L);
};

void LoadNamespace(lua_State *L, const char *name, std::vector<rfunc> funcs)
{
  lua_getglobal(L, name);
  if (!lua_istable(L, -1))
  {
  	lua_createtable(L, 0, 1);
  	lua_setglobal(L, name);
  	lua_pop(L, 1);
  	lua_getglobal(L, name);
	}

  for (int i = 0; i < funcs.size(); i++){
    lua_pushstring(L, funcs[i].name);
  	lua_pushcfunction(L, funcs[i].func);
    lua_settable(L, -3);
  }
  lua_pop(L, 1);
}

void lapi::openApi(lua_State *L)
{
  // Cmd
  LoadNamespace(L, "cmd", {
    { "exeConfig", cmd::lua::exec },
    { "exists", cmd::lua::exists },
    { "exe", cmd::lua::exe },
    { "parse", cmd::lua::parse },
    { "data", cmd::lua::data }
  });
  // Cvars
  LoadNamespace(L, "cvars", {
    { "get", cvars::lua::get },
    { "set", cvars::lua::set },
    { "exists", cvars::lua::exists }
  });
  // Console
  LoadNamespace(L, "console", {
    { "log", con::lua::log },
    { "error", con::lua::error }
  });
  // VKAPI
  vk::lua::initVK(L);
  LoadNamespace(L, "vk", {
    { "send", vk::lua::send },
    { "jSend", vk::lua::jSend }
  });
  // String
  LoadNamespace(L, "string", {
    { "starts", lapi::funcs::str::starts },
    { "lower", lapi::funcs::str::lower },
		{ "upper", lapi::funcs::str::upper },
		//{ "regReplace", utils::lua::string::regReplace },
		{ "split", lapi::funcs::str::split }
  });

  // net
  LoadNamespace(L, "net", {
    { "send", net::lua::send },
    { "jSend", net::lua::jSend }
  });

  LoadNamespace(L, "fs", {
    { "exists", fs::lua::exists },
    { "read", fs::lua::read },
    { "readLines", fs::lua::readLines },
    { "write", fs::lua::write },
    { "dirList", fs::lua::dirList }
  });


  lua_pushstring(L, _version.c_str()); lua_setglobal(L, "_VERSION");
  lua_pushstring(L, bot_path.c_str()); lua_setglobal(L, "root");

  lua_register(L, "trand", lapi::funcs::randtable);
  lua_register(L, "uptime", lapi::funcs::uptime);
  lua_register(L, "relua", lapi::funcs::relua);
  lua_register(L, "getId", lapi::funcs::getId);
  lua_register(L, "addline", lapi::funcs::addline);
  lua_register(L, "connect", lapi::funcs::connect);
  lua_register(L, "isFlag", lapi::funcs::isFlag);
  lua_register(L, "getPeer", lapi::funcs::getPeer);
  lua_register(L, "resp", lapi::funcs::resp);
  lua_register(L, "isGroup", lapi::funcs::isGroup);
  lua_register(L, "getmsg", lapi::funcs::getmsg);
  mysqlwork::api(L);

  // JSON
  lua_register(L, "jDecode", lJson::decode); // jsonString -> table
}
