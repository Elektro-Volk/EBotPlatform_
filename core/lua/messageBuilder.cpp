#include "lua_json.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include "../common.h"
#include "../console.h"
#include "../vk/vkapi.h"

using namespace rapidjson;

bool pushValue(lua_State *L, const rapidjson::Value &value);
void buildMessage(lua_State *L, rapidjson::Value& longpoll)
{
  if (vk::groupmode->getBool()) {
    pushValue(L, longpoll);
    return;
  }

  if (longpoll[7].HasMember("attach1_type")) { // Have attachments
    rapidjson::Document result = vk::jSend("messages.getById", {{"message_ids", to_string(longpoll[1].GetInt())}});
  	pushValue(L, result["response"]["items"][0]);
  	return;
  }

  int peer_id = longpoll[3].GetInt();
  bool is_chat = peer_id > 2000000000;
  int user_id = is_chat ? stoi(longpoll[6]["from"].GetString()) : peer_id;
  int flags = longpoll[2].GetInt();

  Document d(kObjectType);
  {
      d.AddMember("id", Value().SetInt(longpoll[1].GetInt()), d.GetAllocator());
      d.AddMember("user_id", Value().SetInt(user_id), d.GetAllocator());
      d.AddMember("from_id", Value().SetInt(peer_id), d.GetAllocator());
      d.AddMember("date", Value().SetInt(longpoll[4].GetInt()), d.GetAllocator());
      d.AddMember("read_state", Value().SetInt(flags&1 ? 0 : 1), d.GetAllocator());
      d.AddMember("out", Value().SetInt(flags&2 ? 1 : 0), d.GetAllocator());
      d.AddMember("title", Value().SetString("TODO"), d.GetAllocator());
      d.AddMember("body", longpoll[5], d.GetAllocator());
      if (is_chat) {
        d.AddMember("chat_id", Value().SetInt(peer_id - 2000000000), d.GetAllocator());
      }
  }

  pushValue(L, d);
}
