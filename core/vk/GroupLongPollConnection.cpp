/*
GroupLongPollConnection.cpp
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
#include "GroupLongPollConnection.h"
#include "longpoll.h"
#include "console.h"
#include "vkapi.h"
#include "lua/luawork.h"
#include "net.h"

using namespace rapidjson;

GroupLongPollConnection::GroupLongPollConnection()
{
	params = { { "version", "3" }, { "act", "a_check" }, { "wait", "25" }, { "mode", "10" } };
}

void GroupLongPollConnection::getServer()
{
	con::log("Getting bots longpoll server...");

  string method = "groups.getLongPollServer";
	Document document = vk::jSend(method, {{ "group_id", vk::groupid->value }});
	Value &data = document["response"];

	server = data["server"].GetString();
	params["key"] = data["key"].GetString();
	params["ts"] = to_string(data["ts"].GetInt());
}

void GroupLongPollConnection::loop()
{
	con::log("Bots LongPoll was successfully launched");
	while (true) {
		Document data;
		data.Parse(net::POST(server, params).c_str());
    // Check data
		if(!data.IsObject()) continue;
		if(data.HasMember("failed")) { processError(data); continue; }
    // Get value
		params["ts"] = data["ts"].GetString();
    // Process updates
		Value &updates = data["updates"];
		for (int i = 0; i < updates.Size(); i++) processMessage(updates[i]);
	}
}

void GroupLongPollConnection::processError(rapidjson::Document &err)
{
  con::log("LongPoll failed: " + to_string(err["failed"].GetInt()));
  if (err["failed"].GetInt() != 1) getServer();
  else params["ts"] = to_string(err["ts"].GetInt());
}

void GroupLongPollConnection::processMessage(rapidjson::Value &upd)
{
  if(upd["type"] != "message_new") return; // This is not a message
  if(longpoll::lp_ignore_myself->getBool() && upd["object"]["out"].GetInt() == 1) return;
  if(longpoll::lp_debug->getBool()) con::log("LongPoll message: " + to_string(upd["object"]["id"].GetInt()));
  luawork::push(upd["object"]);
}
