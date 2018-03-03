#include "lJson.h"
#include "../json.hpp"
#include "../common.h"
#include "../console.h"

using json = nlohmann::json;

void pushObject(lua_State *L, json &j);
void pushArray(lua_State *L, json &array);
bool pushValue(lua_State *L, json &value);

void stackDump (lua_State *L) {
          int i=lua_gettop(L);
          printf(" ----------------  Stack Dump ----------------\n" );
          while(  i   ) {
            int t = lua_type(L, i);
            switch (t) {
              case LUA_TSTRING:
                printf("%d:`%s'\n", i, lua_tostring(L, i));
              break;
              case LUA_TBOOLEAN:
                printf("%d: %s\n",i,lua_toboolean(L, i) ? "true" : "false");
              break;
              case LUA_TNUMBER:
                printf("%d: %g\n",  i, lua_tonumber(L, i));
             break;
             default: printf("%d: %s\n", i, lua_typename(L, t)); break;
            }
           i--;
          }
         printf("--------------- Stack Dump Finished ---------------\n" );
    }

int lJson::encode(lua_State *L)
{
return 0; // TODO
}

// --------------- DECODER ---------------

void pushObject(lua_State *L, json &j)
{
  lua_newtable(L);
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    string v = it.key();
    lua_pushstring(L, v.c_str());
    if(pushValue(L, it.value()))
      lua_settable(L, -3);
    else
      lua_pop(L, 1);
  }
}

void pushArray(lua_State *L, json &array)
{
  lua_newtable(L);
  for (int i = 0; i < array.size(); i++) {
    lua_pushinteger(L, i + 1);
    if(pushValue(L, array[i]))
      lua_settable(L, -3);
    else
      lua_pop(L, 1);
  }
}

bool pushValue(lua_State *L, json &value)
{
  switch (value.type()) {
    case json::value_t::object: { pushObject(L, value); break; }
    case json::value_t::array:  { pushArray(L, value); break; }
    case json::value_t::string: { string v = value; lua_pushstring(L, v.c_str()); break; }
    case json::value_t::number_integer: { lua_pushinteger(L, (int)value); break; }
    case json::value_t::number_unsigned: { lua_pushinteger(L, (int)value); break; }
    case json::value_t::number_float: { lua_pushnumber(L, (float)value); break; }
    case json::value_t::boolean: { lua_pushboolean(L, (bool)value); break; }
    default: { return false; break; }
  }
  return true;
}

int lJson::decode(lua_State *L)
{
  try {
    json j = json::parse(luaL_checkstring(L, -1));
    lua_pop(L, 1);
    pushValue(L, j);
  }
  catch (nlohmann::detail::parse_error e) {
		luaL_error(L, e.what());
	}
  return 1;
}
