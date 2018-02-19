#pragma once
#include <mysql/mysql.h>
#include "common.h"
#include "lua/luai.h"

namespace mysqlwork {
  class Connection {
  public:
    MYSQL *mysql;
  };

  void api(lua_State* L);
  int connect(lua_State* L);
  int execute(lua_State* L);
}
