#include "lAPI.h"
#include "lJson.h"
#include "luawork.h"
#include "../console.h"
#include "../vk/vkapi.h"
#include "../strutils.h"
#include <map>

// int uptime()
int lapi::funcs::uptime(lua_State *L)
{
  lua_pushinteger(L, time(0)-startTime);
  return 1;
}

int lapi::funcs::relua(lua_State *L)
{
  con::log("Relua from lua");
  luawork::c_relua(vector<string>());
  return 0;
}

// int uptime()
int lapi::funcs::getId(lua_State *L)
{
  std::string url = luaL_checkstring(L, 1);
	if (url.find("/") != std::string::npos) {
		auto tokens = strutils::split(url, '/');
		url = tokens[tokens.size() - 1];
	}

	nlohmann::json result = vk::jSend("utils.resolveScreenName",
	{ { "screen_name", url } }).at("response");
	if (result.find("object_id") == result.end())
		lua_pushnil(L);
	else
		lua_pushinteger(L, (int)result.at("object_id"));
	return 1;
}

// int uptime()
int lapi::funcs::addline(lua_State *L)
{
  const char* line = luaL_checkstring(L, 2);
	lua_getfield(L, 1, "message"); // Get message
	if (lua_isnil(L, -1))
	{
		lua_pushstring(L, line);
	}
	else {
		lua_pushfstring(L, "%s\n%s", luaL_checkstring(L, -1), line);
	}
	lua_setfield(L, 1, "message");
	return 0;
}

// int uptime()
int lapi::funcs::connect(lua_State *L)
{
  const char *path = lua_tostring(L, 1);
	lua_getglobal(L, "require");
	lua_pushfstring(L, "%s/%s", bot_path.c_str(), path);
	lua_pcall(L, 1, -1, 0);
	return 1;
}

// int uptime()
int lapi::funcs::isFlag(lua_State *L)
{
  lua_pushboolean(L, lua_tointeger(L, 1) & lua_tointeger(L, 2));
	return 1;
}

// int uptime()
int lapi::funcs::getPeer(lua_State *L)
{
  luaL_checktype(L, -1, LUA_TTABLE);
	lua_getfield(L, -1, "chat_id");
	if (lua_isnil(L, -1)) {
		lua_getfield(L, -2, "user_id");
		lua_pushstring(L, to_string(lua_tointeger(L, -1)).c_str());
		lua_remove(L, -2);
	}
	else
		lua_pushfstring(L, to_string(2000000000 + lua_tointeger(L, -1)).c_str());
	lua_remove(L, -2);
	return 1;
}

// int uptime()
int lapi::funcs::resp(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_isstring(L, 2)) {
		lua_newtable(L);
		lua_insert(L, 2);
		lua_setfield(L, -2, "message");
	}
	lua_insert(L, 1);
	getPeer(L);
	lua_setfield(L, 1, "peer_id");
	lua_remove(L, -1);

	lua_pushstring(L, "messages.send");
	lua_insert(L, -2);
	return vk::lua::jSend(L);
}


// bool string.starts(str, pref)
int lapi::funcs::str::starts(lua_State *L)
{
  lua_pushboolean(L, strutils::starts(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
  return 1;
}

// string string.lower(str)
int lapi::funcs::str::lower(lua_State *L)
{
  lua_pushstring(L, strutils::toLower(luaL_checkstring(L, 1)).c_str());
  return 1;
}

// string string.upper(str)
int lapi::funcs::str::upper(lua_State *L)
{
  lua_pushstring(L, strutils::toUpper(luaL_checkstring(L, 1)).c_str());
  return 1;
}


// table string.split(str, separator)
int lapi::funcs::str::split(lua_State *L)
{
  const char *s = luaL_checkstring(L, 1);
	const char *sep = luaL_checkstring(L, 2);
	const char *e;
	int i = 1;
	lua_newtable(L);
	while ((e = strchr(s, *sep)) != NULL) {
		lua_pushlstring(L, s, e - s);
		lua_rawseti(L, -2, i++);
		s = e + 1;
	}
	lua_pushstring(L, s);
	lua_rawseti(L, -2, i);
	return 1;
}
