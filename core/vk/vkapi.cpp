#include "vkapi.h"
#include "../net.h"
#include "../cvar.h"
#include "../console.h"
#include <chrono>
#include <thread>

cvars::Cvar *vk::token;
cvars::Cvar *vk::version;
cvars::Cvar *vk::groupmode;
cvars::Cvar *vk::groupid;

void vk::init()
{
  vk::token = cvars::add("vk_token", "-");
  vk::version = cvars::add("vk_version", "5.73");
  vk::groupmode = cvars::add("vk_group", "0");
  vk::groupid = cvars::add("vk_groupid", "0");
}

void vk::start()
{
  if (groupmode->getBool()) return;
  auto data = vk::jSend("stats.trackVisitor");
	if (data.HasMember("error")) {
		con::error("Wrong vk_token");
    con::error("Please, setting you config.cfg");
		HostEnd(-1);
	}
}

string vk::send(string method, map<string, string> params)
{
  //	if (method == "messages.send")// Send stats
  //		net::send("http://ebp.elektro-volk.ru/api/addstat.php", { {"key", cvar::get ("eapi_key")} });
  if(params.find("access_token") == params.end()) params["access_token"] = token->value;
  params["v"] = version->value;
  return net::POST("https://api.vk.com/method/" + method, params);
}

rapidjson::Document vk::jSend(string method, map<string, string> params)
{
  rapidjson::Document data;
  data.Parse(vk::send(method, params).c_str());
  if (data.HasMember("error") && data["error"]["error_code"].GetInt() == 6) { // error 6
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return vk::jSend(method, params);
  }
  return data;
}

/* LUA */
// VK(method, data)
void vk::lua::initVK(lua_State *L)
{
  luaL_newmetatable(L, "meta_VK");
	lua_pushcfunction(L, vk::lua::__index);
	lua_setfield(L, -2, "__index");

  lua_setglobal(L, "VK");
  lua_pop(L, 1);
  lua_getglobal(L, "VK");
  luaL_getmetatable(L, "meta_VK");
  lua_setmetatable(L, -2);

  luaL_newmetatable(L, "meta_VKMETHOD");
	lua_pushcfunction(L, vk::lua::__indexMethod);
	lua_setfield(L, -2, "__index");
  lua_pushcfunction(L, vk::lua::__call);
	lua_setfield(L, -2, "__call");
  lua_pop(L, 1);
}

int vk::lua::__index(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TSTRING);

  const char *str = luaL_checkstring(L, 2);

  lua_createtable(L, 0, 1);
  lua_pushstring(L, "type");
  lua_pushstring(L, str);
  lua_settable(L, -3);
  luaL_getmetatable(L, "meta_VKMETHOD");
  lua_setmetatable(L, -2);
  return 1;
}

int vk::lua::__indexMethod(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TSTRING);

  const char *method = luaL_checkstring(L, 2);
  lua_pushstring(L, "type");
  lua_gettable(L, 1);
  const char *type = luaL_checkstring(L, -1);

  lua_createtable(L, 0, 1);
  lua_pushstring(L, "a");
  lua_pushstring(L, (string(type) + "." + string(method)).c_str());
  lua_settable(L, -3);
  luaL_getmetatable(L, "meta_VKMETHOD");
  lua_setmetatable(L, -2);
  return 1;
}

int vk::lua::__call(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TTABLE);

  lua_pushstring(L, "a");
  lua_gettable(L, 1);

  lua_remove(L, 1);
  lua_insert(L, 1);
  vk::lua::jSend(L);

  return 1;
}

// string vk.send(method, data)
int vk::lua::send(lua_State *L)
{
  map<string, string> _args;

  luaL_checktype(L, 1, LUA_TSTRING);
  luaL_checktype(L, 2, LUA_TTABLE);

  lua_pushnil(L);
  while (lua_next(L, 2)) {
    lua_pushvalue(L, -2);
    _args.insert(pair<string, string>(luaL_checkstring(L, -1), luaL_checkstring(L, -2)));
    lua_pop(L, 2);
  }

  lua_pushstring(L, ::vk::send(lua_tostring(L, 1), _args).c_str());
  return 1;
}

// table vk.jSend(method, data)
int vk::lua::jSend(lua_State *L)
{
  send(L);
  lJson::decode(L);
  return 1;
}
