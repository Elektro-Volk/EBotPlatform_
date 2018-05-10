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
