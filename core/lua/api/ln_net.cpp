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
      Request *request = new Request(lua_tostring(L, 1));
      vector<const char*> headers;
      map<string, string> _args;
      map<const char*, const char*> mimes;
      lua_pushnil(L);
      while (lua_next(L, 2)) {
        lua_pushvalue(L, -2);
        const char* param = luaL_checkstring(L, -1);
        if(param[0] == '!')
          headers.push_back(luaL_checkstring(L, -2));
        else if(param[0] == '#')
          mimes.insert(pair<const char*, const char*>(string(param).substr(1).c_str(), luaL_checkstring(L, -2)));
        else
          _args.insert(pair<string, string>(param, luaL_checkstring(L, -2)));
        lua_pop(L, 2);
      }

      const char* fields = makeFields(_args);
      request->setData(fields);
      if(headers.size() > 0) {
        request->headers = headers.data();
        request->nHeaders = headers.size();
      }
      request->mimes = mimes;

      auto res = request->send();
      lua_pushlstring(L, res.c_str(), res.size());
      delete request;
      delete fields;
    }
    else {
      auto res = ::net::POST(lua_tostring(L, 1), lua_tostring(L, 2));
      lua_pushlstring(L, res.c_str(), res.size());
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
