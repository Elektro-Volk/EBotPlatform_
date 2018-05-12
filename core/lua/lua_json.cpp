#include "lua_json.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "../common.h"
#include "../console.h"

using namespace rapidjson;

int lua_json::encode(lua_State *L)
{
  return 0; // TODO
}

// --------------- DECODER ---------------
void lua_json::pushObject(lua_State *L, const rapidjson::Value &j)
{
  lua_newtable(L);
  for (Value::ConstMemberIterator it = j.MemberBegin(); it != j.MemberEnd(); ++it) {
    lua_pushstring(L, it->name.GetString());
    if(pushValue(L, it->value)) lua_settable(L, -3); else lua_pop(L, 1);
  }
}

void lua_json::pushArray(lua_State *L, const rapidjson::Value &array)
{
  lua_newtable(L);
  for (int i = 0; i < array.Size(); i++) {
    lua_pushinteger(L, i + 1);
    if(pushValue(L, array[i]))
      lua_settable(L, -3);
    else
      lua_pop(L, 1);
  }
}

bool lua_json::pushValue(lua_State *L, const rapidjson::Value &value)
{
  switch (value.GetType()) {
    case 1: { lua_pushboolean(L, false); break; }
    case 2: { lua_pushboolean(L, true ); break; }
    case 3: { pushObject(L, value); break; }
    case 4: { pushArray(L, value); break; }
    case 5: { lua_pushstring(L, value.GetString()); break; }
    case 6: {
      if(value.IsDouble())
        lua_pushnumber(L, value.GetDouble());
      else
        lua_pushinteger(L, value.GetInt());
      break;
    }
    default: { return false; break; }
  }
  return true;
}

int lua_json::decode(lua_State *L)
{
  rapidjson::Document j;
  rapidjson::ParseResult ok = j.Parse(luaL_checkstring(L, -1));
  if(!ok) {
     luaL_error(L, "JSON parse error: %s (%d)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
     return 1;
  }
  lua_pop(L, 1);
  lua_json::pushValue(L, j);
  return 1;
}
