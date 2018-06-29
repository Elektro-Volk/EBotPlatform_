/*
ln_console.cpp
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
#include "ln_console.h"
#include "common.h"
#include "console.h"
#include "lua/luawork.h"

void ln_console::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_console::log, "log");
  luaapi_tablefunc(L, ln_console::error, "error");

  lua_setglobal(L, "console");
}

// void console.log(text)
int ln_console::log(lua_State *L)
{
    lua_getglobal(L, "string");
	lua_getfield(L,-1, "format");
    lua_insert(L, 1);
    luawork::safeCall(L, lua_gettop(L) - 1, 1);
    con::log(luaL_checkstring(L, 1));
    return 0;
}

// void console.log(text)
int ln_console::error(lua_State *L)
{
  con::error(luaL_checkstring(L, 1));
  return 0;
}
