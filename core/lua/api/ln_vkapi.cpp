/*
ln_vkapi.cpp
Copyright (C) 2018 Elektro-Volk
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#include "ln_vkapi.h"
#include "common.h"
#include "../lua_json.h"
#include "vk/vkapi.h"

void ln_vkapi::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_vkapi::vk_send, "send");
  luaapi_tablefunc(L, ln_vkapi::vk_jSend, "jSend");

  lua_setglobal(L, "vk");

  // VK.type.method { ... }
  luaL_newmetatable(L, "meta_VK");
	lua_pushcfunction(L, ln_vkapi::__index);
	lua_setfield(L, -2, "__index");

  lua_setglobal(L, "VK");
  lua_pop(L, 1);
  lua_getglobal(L, "VK");
  luaL_getmetatable(L, "meta_VK");
  lua_setmetatable(L, -2);

  luaL_newmetatable(L, "meta_VKMETHOD");
	lua_pushcfunction(L, ln_vkapi::__indexMethod);
	lua_setfield(L, -2, "__index");
  lua_pushcfunction(L, ln_vkapi::__call);
	lua_setfield(L, -2, "__call");
  lua_pop(L, 1);
}

/* string vk.send(method, params) */
int ln_vkapi::vk_send(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TSTRING);
  luaL_checktype(L, 2, LUA_TTABLE);

  lua_pushnil(L);
  map<string, string> args;
  while (lua_next(L, 2)) {
    lua_pushvalue(L, -2);
    args.insert(pair<string, string>(luaL_checkstring(L, -1), luaL_checkstring(L, -2)));
    lua_pop(L, 2);
  }

  lua_pushstring(L, vk::send(lua_tostring(L, 1), args).c_str());
  return 1;
}

/* table vk.jSend(method, params) */
int ln_vkapi::vk_jSend(lua_State *L)
{
  ln_vkapi::vk_send(L); lua_json::decode(L);
  return 1;
}

int ln_vkapi::__index(lua_State *L)
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

int ln_vkapi::__indexMethod(lua_State *L)
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

int ln_vkapi::__call(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TTABLE);

  lua_pushstring(L, "a");
  lua_gettable(L, 1);

  lua_remove(L, 1);
  lua_insert(L, 1);
  ln_vkapi::vk_jSend(L);

  return 1;
}
