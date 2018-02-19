#include "luai.h"

namespace lapi {
  void openApi(lua_State *L);

  namespace funcs {
    int uptime(lua_State *L);
    int relua(lua_State *L);
    int getId(lua_State *L);
    int addline(lua_State *L);
    int connect(lua_State *L);
    int isFlag(lua_State *L);
    int getPeer(lua_State *L);
    int resp(lua_State *L);

    namespace str {
      int starts(lua_State *L);
      int lower(lua_State *L);
      int upper(lua_State *L);
      int split(lua_State *L);
    }
  }
}
