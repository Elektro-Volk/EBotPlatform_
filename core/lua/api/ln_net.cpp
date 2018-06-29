/*
ln_net.cpp
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
#include "ln_net.h"
#include "common.h"
#include "net.h"
#include <vector>
#include "../lua_json.h"

void ln_net::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_net::send, "send");
  luaapi_tablefunc(L, ln_net::jSend, "jSend");

  lua_setglobal(L, "net");
}

using namespace net;

// string net.send(url, [params|data])
int ln_net::send(lua_State* L)
{
  luaL_checktype(L, 1, LUA_TSTRING);
  if(lua_gettop(L) == 1) { // GET
    string data = ::net::GET(lua_tostring(L, 1));
    lua_pushlstring(L, data.c_str(), data.size());
  }
  else {
    if(lua_istable(L, 2)) { // Params
      lua_pushnil(L);
      map<string, string> args;
      while (lua_next(L, 2)) {
        lua_pushvalue(L, -2);
        args.insert(pair<string, string>(luaL_checkstring(L, -1), luaL_checkstring(L, -2)));
        lua_pop(L, 2);
      }

      auto result = ::net::POST(lua_tostring(L, 1), args);
      lua_pushlstring(L, result.c_str(), result.size());
    }
    else {
      auto result = ::net::POST(lua_tostring(L, 1), lua_tostring(L, 2));
      lua_pushlstring(L, result.c_str(), result.size());
    }
  }
  return 1;
}

// table net.jSend(method, [data])
int ln_net::jSend(lua_State *L)
{
  ln_net::send(L);
  lua_json::decode(L);
  return 1;
}
