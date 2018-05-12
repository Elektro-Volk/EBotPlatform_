#include "luai.h"
#include "rapidjson/document.h"

namespace lua_json {
  int encode(lua_State *L);
  int decode(lua_State *L);

  void pushObject(lua_State *L, const rapidjson::Value &j);
  void pushArray(lua_State *L, const rapidjson::Value &array);
  bool pushValue(lua_State *L, const rapidjson::Value &value);
}
