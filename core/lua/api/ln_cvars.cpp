/*
ln_cvars.h
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
#include "ln_cvars.h"
#include "common.h"
#include "cvars.h"

void ln_cvars::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_cvars::get, "get");
  luaapi_tablefunc(L, ln_cvars::set, "set");
  luaapi_tablefunc(L, ln_cvars::exists, "exists");

  lua_setglobal(L, "cvars");
}

int ln_cvars::get(lua_State *L)
{
  lua_pushstring(L, cvars::get(luaL_checkstring(L, 1))->value.c_str());
  return 1;
}

int ln_cvars::set(lua_State *L)
{
  cvars::get(luaL_checkstring(L, 1))->setValue(luaL_checkstring(L, 2));
  return 0;
}

int ln_cvars::exists(lua_State *L)
{
  lua_pushboolean(L, cvars::exists(luaL_checkstring(L, 1)));
  return 0;
}
