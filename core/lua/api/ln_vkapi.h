/*
ln_vkapi.h
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

namespace ln_vkapi {
  void init_api(lua_State *L);

  int vk_send(lua_State *L);
  int vk_jSend(lua_State *L);
  // VK.type.method { ... }
  int __index(lua_State *L);
  int __indexMethod(lua_State *L);
  int __call(lua_State *L);
};
