/*
ln_fs.cpp
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
#include "ln_fs.h"
#include "common.h"
#include "filesystem.h"

void ln_fs::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_fs::exists, "exists");
  luaapi_tablefunc(L, ln_fs::read, "read");
  luaapi_tablefunc(L, ln_fs::readLines, "readLines");
  luaapi_tablefunc(L, ln_fs::write, "write");
  luaapi_tablefunc(L, ln_fs::writeLines, "writeLines");
  luaapi_tablefunc(L, ln_fs::dirList, "dirList");

  lua_setglobal(L, "fs");
}


int ln_fs::exists(lua_State *L)
{
  lua_pushboolean(L, fs::exists(luaL_checkstring(L, 1)));
  return 1;
}

int ln_fs::read(lua_State *L)
{
  lua_pushfstring(L, fs::read(luaL_checkstring(L, 1)).c_str());
  return 1;
}

int ln_fs::readLines(lua_State *L)
{
  vector<string> lines = fs::readLines(luaL_checkstring(L, 1));
  lua_newtable(L);
  for (int i = 0; i < lines.size(); i++) {
    lua_pushinteger(L, i + 1);
    lua_pushfstring(L, lines[i].c_str());
    lua_settable(L, -3);
  }
  return 1;
}

int ln_fs::write(lua_State *L)
{
  fs::write(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
  return 0;
}

int ln_fs::writeLines(lua_State *L)
{
  return 0; // TODO
}

int ln_fs::dirList(lua_State *L)
{
  vector<string> data = fs::dirList(luaL_checkstring(L, 1));
  lua_newtable(L);
  for (int i = 0; i < data.size(); i++) {
    lua_pushinteger(L, i + 1);
    lua_pushstring(L, data[i].c_str());
    lua_settable(L, -3);
  }
  return 1;
}
