/*
chat_cache.cpp
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
#include "chat_cache.h"
#include "vkapi.h"

using namespace rapidjson;

std::map<int, chat_info> chat_cache::chats;

void chat_cache::updateInfo(int id)
{
  chat_info chat;
  chat.id = id;

	Document document = vk::jSend("messages.getChat", {{ "chat_id", to_string(id) }});
  if (document.HasMember("error")) {
    chats.insert(pair<int, chat_info>(id, chat));
    return;
  }
	Value &data = document["response"];

  chat.type = data["type"].GetString();
  chat.title = data["title"].GetString();
  chat.admin_id = data["admin_id"].GetInt();
  //chat.users;

  chats.insert(pair<int, chat_info>(id, chat));
  return;
}

chat_info *chat_cache::getChat(int id)
{
  if (chats.find(id) == chats.end()) chat_cache::updateInfo(id);
  return &chats[id];
}

string chat_cache::c_clist(vector<string> _args)
{
  string result = "Chat cache:\n";
  //for(chat_info chat : chats)
  //  result += to_string(chat)
  return result;
}

string chat_cache::c_cupd(vector<string> _args)
{
  //chat_cache::updateInfo(_args[1]);
  return "OK";
}

string chat_cache::c_crem(vector<string> _args)
{
  return "OK";
}

string chat_cache::c_cinfo(vector<string> _args)
{
  return "OK";
}
