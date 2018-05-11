/*
ln_mysql.cpp
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
#include "ln_mysql.h"
#include <mysql/mysql.h>
#include "lock.h"
#include "lua/luawork.h"

struct mysql_object {
	LSECT lock_section;
	MYSQL *mysql;
};

void ln_mysql::init_api(lua_State* L)
{
	lua_register(L, "mysql", ln_mysql::connect);
  // mysql_connection
	luaL_newmetatable(L, "mysql_connection");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	lua_pushstring(L, "__call");
	lua_pushcfunction(L, ln_mysql::execute);
	lua_settable(L, -3);

  lua_pop(L, 1);
}

int ln_mysql::connect(lua_State* L)
{
	const char* server   = luaL_checkstring(L, 1);
	const char* username = luaL_checkstring(L, 2);
	const char* password = luaL_checkstring(L, 3);
	const char* database = luaL_checkstring(L, 4);

	// Init mysql
	MYSQL *mysql = mysql_init(NULL);
	if (!mysql) luaL_error(L, mysql_error(mysql));

	// Connect
	if (!mysql_real_connect(mysql, server, username, password, database, 0, 0, 0))
		luaL_error(L, "Connection error: %s", mysql_error(mysql));

	// Create structure
	mysql_object* object = new mysql_object();
	object->lock_section = lock_new();
	object->mysql = mysql;

	// Push lua
	lua_pushlightuserdata(L, object);
  luaL_getmetatable(L, "mysql_connection");
  lua_setmetatable(L, -2);

  return 1;
}

int ln_mysql::execute(lua_State* L)
{
	mysql_object *object = (mysql_object*)luaL_checkudata(L, 1, "mysql_connection");
	const char* query = luaL_checkstring(L, 2);

  MYSQL *mysql = object->mysql;
	int nStrings = lua_gettop(L) - 1; // -table
	lua_getglobal(L, "string");
	lua_getfield(L,-1, "format");
	lua_pushstring(L, query);
	for (int i = 1; i < nStrings; i++) {
		const char *from = luaL_checkstring(L, i + 2);
    char to[256];
    unsigned long len = mysql_real_escape_string(mysql, to, from, strlen(from));
		to[len] = '\0';
		lua_pushstring(L, to);
	}
	luawork::safeCall(L, nStrings, 1);
	query = luaL_checkstring(L, -1);
	lua_settop(L, 0);

  // Do SQL query
	lock_lock(object->lock_section);
  if (mysql_query(mysql, query)) {
		lock_unlock(object->lock_section);
		luaL_error(L, mysql_error(mysql));
	}

  MYSQL_RES *res = mysql_store_result(mysql);
	lock_unlock(object->lock_section);

	if (!res) return 0; // null response
  // Get values
  MYSQL_FIELD *fields = mysql_fetch_fields(res);
  lua_newtable(L);
  for(int n = 1; MYSQL_ROW row = mysql_fetch_row(res); n++) {
    lua_pushinteger(L, n);
    lua_newtable(L);
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(res);
		for (int i = 0; i < mysql_num_fields(res); i++) {
      lua_pushstring(L, fields[i].name);
			switch(fields[i].type) {
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
										lua_pushinteger(L, stoi(row[i])); break;
      	default: lua_pushlstring(L, row[i], lengths[i]); break;
			}
      lua_settable(L, -3);
    }
    lua_settable(L, -3);
   }
    mysql_free_result(res);

    return 1;
}
