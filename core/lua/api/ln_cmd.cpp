/*
ln_cmd.cpp
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
#include "ln_cmd.h"
#include "common.h"
#include "../lua_json.h"
#include "cmd.h"

void ln_cmd::init_api(lua_State *L)
{
  lua_createtable(L, 0, 1);

  luaapi_tablefunc(L, ln_cmd::exec, "exec");
  luaapi_tablefunc(L, ln_cmd::exists, "exists");
  luaapi_tablefunc(L, ln_cmd::exe, "exe");
  luaapi_tablefunc(L, ln_cmd::parse, "parse");
  luaapi_tablefunc(L, ln_cmd::data, "data");

  lua_setglobal(L, "cmd");
}

// void exec (path)
int ln_cmd::exec(lua_State * L)
{
	const char *path = luaL_checkstring(L, 1);
	cmd::exec(path);
	return 0;
}

// bool exists (cmd_name)
int ln_cmd::exists(lua_State * L)
{
	const char *cmd_name = luaL_checkstring(L, 1);
	lua_pushboolean(L, cmd::exists(cmd_name));
	return 1;
}

// string exe (cmd_name)
int ln_cmd::exe(lua_State * L)
{
	const char *cmd_name = luaL_checkstring(L, 1);
	lua_pushstring(L, cmd::exe(cmd_name).c_str());
	return 1;
}

// table parse (line)
int ln_cmd::parse(lua_State * L)
{
	const char *line = luaL_checkstring(L, 1);
	auto parsed = cmd::parse(line);

	lua_newtable(L);
	for (int i = 0; i < parsed.size(); i++) {
		lua_pushnumber(L, i+1); // Key
		lua_pushstring(L, parsed[i].c_str()); // Value
		lua_settable(L, -3);
	}
	return 1;
}

// string data (table)
int ln_cmd::data(lua_State * L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  int offset = luaL_checkinteger(L, 2);

	vector<string> args;
	lua_pushnil(L);
	while (lua_next(L, 1))
	{
		lua_pushvalue(L, -1);
		args.push_back(luaL_checkstring(L, -1));
		lua_pop(L, 2);
	}
	lua_pushstring(L, cmd::data(args, offset).c_str());

	return 1;
}
