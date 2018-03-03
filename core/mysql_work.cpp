/*
  Filename: mysql_work.cpp
  Description:
    Wrapper around MYSQL to lua

  Elektro-Volk 2017-2018
  <elektro-volk@yandex.ru>
*/
#include "mysql_work.h"
#include "lock.h"
#include "console.h"
#include <cstring>
#include "lua/luawork.h"

class MSCON {
public:
	LSECT sect;
	MYSQL *con;
	MSCON() {
		sect = lock_new();
		con = mysql_init(NULL);
	}

	~MSCON() {
		lock_free(sect);
		if(con) mysql_close(con);
	}
};

void mysqlwork::api(lua_State* L)
{
	lua_register(L, "mysql", mysqlwork::connect);
  // mysql_connection
	luaL_newmetatable(L, "mysql_connection");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	lua_pushstring(L, "__call");
	lua_pushcfunction(L, mysqlwork::execute);
	lua_settable(L, -3);

  lua_pop(L, 1);
}

int mysqlwork::connect(lua_State* L)
{
	// Init
	MSCON* M = new MSCON();
  if (!M->con) {
    luaL_error(L, mysql_error(M->con));
		delete M;
		return 0;
	}
	// Connect
	const char* server   = luaL_checkstring(L, 1);
	const char* username = luaL_checkstring(L, 2);
	const char* password = luaL_checkstring(L, 3);
	const char* database = luaL_checkstring(L, 4);
	if (!mysql_real_connect(M->con, server, username, password, database, 0, 0, 0)) {
		luaL_error(L, "Connection error: %s", mysql_error(M->con));
		delete M;
		return 0;
  }
	// Push lua
	lua_pushlightuserdata(L, M);
  luaL_getmetatable(L, "mysql_connection");
  lua_setmetatable(L, -2);
  return 1;
}

int mysqlwork::execute(lua_State* L)
{
	MSCON *m = (MSCON*)luaL_checkudata(L, 1, "mysql_connection");
  MYSQL *con = m->con;
	// Make SQL query (format string)
	const char* query = luaL_checkstring(L, 2);
	int nStrings = lua_gettop(L) - 1; // -table
	lua_getglobal(L, "string");
	lua_getfield(L,-1, "format");
	lua_pushstring(L, query);
	for (int i = 1; i < nStrings; i++) {
		const char *from = luaL_checkstring(L, i + 2);
    char to[256];
    unsigned long len = mysql_real_escape_string(con, to, from, strlen(from));
		to[len] = '\0';
		lua_pushstring(L, to);
	}
	luawork::safeCall(L, nStrings, 1);
	query = luaL_checkstring(L, -1);
	lua_settop(L, 0);

  // Do SQL query
	lock_lock(m->sect);
  if (mysql_query(con, query)) {
		lock_unlock(m->sect);
		 luaL_error(L, mysql_error(con));
	}
  MYSQL_RES *res = mysql_store_result(con);
	lock_unlock(m->sect);

	if (!res)  return 0; // null response
  // Get values
  MYSQL_FIELD *fields = mysql_fetch_fields(res);
  lua_newtable(L);
  for(int n = 1;MYSQL_ROW row = mysql_fetch_row(res);n++) {
    lua_pushinteger(L, n);
    lua_newtable(L);
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(res);
    for (int i = 0; i < mysql_num_fields(res); i++) {
      lua_pushstring(L, fields[i].name);
			switch(fields[i].type) {
				case MYSQL_TYPE_FLOAT: lua_pushnumber(L, stoi(row[i])); break;
				case MYSQL_TYPE_DOUBLE: lua_pushnumber(L, stoi(row[i])); break;
				case MYSQL_TYPE_LONG: lua_pushinteger(L, stoi(row[i])); break;
      	default: lua_pushlstring(L, row[i], lengths[i]); break;
			}
      lua_settable(L, -3);
    }
    lua_settable(L, -3);
   }
    mysql_free_result(res);
    return 1;
}
