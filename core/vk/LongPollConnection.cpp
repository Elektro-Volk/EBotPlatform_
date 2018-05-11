/*
LongPollConnection.cpp
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
#include "LongPollConnection.h"
#include "longpoll.h"
#include "console.h"
#include "vkapi.h"
#include "lua/luawork.h"
#include "net.h"

using namespace rapidjson;

LongPollConnection::LongPollConnection()
{
	params = { { "version", "3" }, { "act", "a_check" }, { "wait", "25" }, { "mode", "10" } };
}

void LongPollConnection::getServer()
{
	con::log("Getting longpoll server...");;

  string method = "messages.getLongPollServer";
	Document document = vk::jSend(method, {{ "lp_version", "3" }});
	Value &data = document["response"];

	server = data["server"].GetString();
	params["key"] = data["key"].GetString();
	params["ts"] = to_string(data["ts"].GetInt());
}

void LongPollConnection::loop()
{
	con::log("LongPoll was successfully launched");
	while (true) {
		while(!luawork::isWorking){}
		Document data;
		data.Parse(net::POST("https://" + server, params).c_str());
    // Check data
		if(!data.IsObject()) continue;
		if(data.HasMember("failed")) { processError(data); continue; }
    // Get value
		params["ts"] = to_string(data["ts"].GetInt());
    // Process updates
		Value &updates = data["updates"];
		for (int i = 0; i < updates.Size(); i++) processMessage(updates[i]);
	}
}

void LongPollConnection::processError(rapidjson::Document &err)
{
  con::log("LongPoll failed: " + to_string(err["failed"].GetInt()));
  if (err["failed"].GetInt() != 1) getServer();
  else params["ts"] = to_string(err["ts"].GetInt());
}

void LongPollConnection::processMessage(rapidjson::Value &msg)
{
  if(msg[0] != 4) return; // This is not a message
  if(longpoll::lp_ignore_myself->getBool() && msg[2].GetInt()&2) return;
  if(longpoll::lp_debug->getBool()) con::log("LongPoll message: " + to_string(msg[1].GetInt()));

  if (msg[7].HasMember("attach1_type")) { // Have attachments
    Document result = vk::jSend("messages.getById", {{"message_ids", to_string(msg[1].GetInt())}});
  	luawork::push(result["response"]["items"][0]);
  	return;
  }

  // Build message
  int peer_id = msg[3].GetInt();
  bool is_chat = peer_id > 2000000000;
  int user_id = is_chat ? stoi(msg[6]["from"].GetString()) : peer_id;
  int flags = msg[2].GetInt();

  Document d(kObjectType);
  {
      d.AddMember("id", msg[1], d.GetAllocator());
      d.AddMember("user_id", Value().SetInt(user_id), d.GetAllocator());
      d.AddMember("from_id", Value().SetInt(peer_id), d.GetAllocator());
      d.AddMember("date", msg[4], d.GetAllocator());
      d.AddMember("read_state", Value().SetInt(flags&1 ? 0 : 1), d.GetAllocator());
      d.AddMember("out", Value().SetInt(flags&2 ? 1 : 0), d.GetAllocator());
      d.AddMember("title", Value().SetString("TODO"), d.GetAllocator());
      d.AddMember("body", msg[5], d.GetAllocator());
      if (is_chat) {
        d.AddMember("chat_id", Value().SetInt(peer_id - 2000000000), d.GetAllocator());
      }
  }

  luawork::push(d);
}
