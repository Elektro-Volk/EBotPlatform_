#include "luaapi.h"
#include "common.h"
// Api INCLUDES
#include "ln_vkapi.h"
#include "ln_mysql.h"
#include "ln_cmd.h"
#include "ln_cvars.h"
#include "ln_console.h"
#include "lu_string.h"
#include "ln_net.h"
#include "ln_fs.h"
#include "lu_json.h"
#include "lu_other.h"

void init_lua_api(lua_State *L)
{
  ln_vkapi::init_api(L);
  ln_mysql::init_api(L);
  ln_cmd::init_api(L);
  ln_cvars::init_api(L);
  ln_console::init_api(L);
  lu_string::init_api(L);
  ln_net::init_api(L);
  ln_fs::init_api(L);
  lu_json::init_api(L);
  lu_other::init_api(L);

  lua_pushstring(L, _version.c_str()); lua_setglobal(L, "_VERSION");
  lua_pushstring(L, bot_path.c_str()); lua_setglobal(L, "root");
}
