/*
lu_string.cpp
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
#include "lu_string.h"
#include "common.h"
#include "strutils.h"
#include <cstring>

void lu_string::init_api(lua_State *L)
{
  lua_getglobal(L, "string");

  luaapi_tablefunc(L, lu_string::starts, "starts");
  luaapi_tablefunc(L, lu_string::lower, "lower");
  luaapi_tablefunc(L, lu_string::upper, "upper");
  luaapi_tablefunc(L, lu_string::split, "split");

  lua_pop(L, 1);
}

// bool string.starts(str, pref)
int lu_string::starts(lua_State *L)
{
  lua_pushboolean(L, strutils::starts(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
  return 1;
}

// string string.lower(str)
int lu_string::lower(lua_State *L)
{
  lua_pushstring(L, strutils::toLower(luaL_checkstring(L, 1)).c_str());
  return 1;
}

// string string.upper(str)
int lu_string::upper(lua_State *L)
{
  lua_pushstring(L, strutils::toUpper(luaL_checkstring(L, 1)).c_str());
  return 1;
}


// table string.split(str, separator)
int lu_string::split(lua_State *L)
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
