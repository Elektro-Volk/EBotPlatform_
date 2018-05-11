/*
lu_other.h
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
#pragma once
#include "luaapi.h"

namespace lu_other {
  void init_api(lua_State *L);

  int randtable(lua_State *L);
  int uptime(lua_State *L);
  int relua(lua_State *L);
  void relua_thread();
  int getId(lua_State *L);
  int addline(lua_State *L);
  int connect(lua_State *L);
  int isFlag(lua_State *L);
  int getPeer(lua_State *L);
  int resp(lua_State *L);
  int isGroup(lua_State *L);
  int getmsg(lua_State *L);
}
