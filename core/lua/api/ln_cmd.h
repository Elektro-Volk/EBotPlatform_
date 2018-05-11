/*
ln_cmd.h
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

namespace ln_cmd {
  void init_api(lua_State *L);

  int exec(lua_State *L);
  int exists(lua_State *L);
  int exe(lua_State *L);
  int parse(lua_State *L);
  int data(lua_State *L);
}
