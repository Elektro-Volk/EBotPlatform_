/*
lu_string.h
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

namespace lu_string {
  void init_api(lua_State *L);

  int starts(lua_State *L);
  int lower(lua_State *L);
  int upper(lua_State *L);
  int split(lua_State *L);
}
