#include "longpoll.h"
#include "vkapi.h"
#include "../console.h"
#include "../lua/luawork.h"
#include "../net.h"
#include <thread>

longpoll::LongPoll* longpoll::lp;

/**************************************
	longpoll::start()
	Creates a new LongPoll and starts it
**************************************/
void longpoll::start()
{
	longpoll::lp = new longpoll::LongPoll();
	lp->loop();
}

/**************************************
	longpoll::LongPoll::LongPoll()
	Constructor LongPoll
**************************************/
longpoll::LongPoll::LongPoll()
{
	this->params = { { "version", "2" }, { "act", "a_check" }, { "wait", "25" } };
	getServer();
}

/**************************************
	longpoll::LongPoll::getServer()
	Gets a longpoll server from VK
**************************************/
void longpoll::LongPoll::getServer()
{
	con::log("Getting longpoll server...");
	rapidjson::Document document = vk::jSend("messages.getLongPollServer", {{ "lp_version", "2" }});
	rapidjson::Value &data = document["response"];
	server = data["server"].GetString();
	params["key"] = data["key"].GetString();
	params["ts"] = to_string(data["ts"].GetInt());
}

/**************************************
	longpoll::LongPoll::loop()
	Main loop
**************************************/
void longpoll::LongPoll::loop()
{
	con::log("LongPoll was successfully launched");
	while (true) {
		try {
			rapidjson::Document data;
			data.Parse(net::POST("https://" + server, params).c_str());

			if(data.HasMember("failed")) throw data; // Failed
			params["ts"] = to_string(data["ts"].GetInt());

			rapidjson::Value &updates = data["updates"];
			for (int i = 0; i < updates.Size(); i++)
				if(updates[i][0] == 4) luawork::push(updates[i]); // Messages only
		}
		catch (rapidjson::Document &err) { // Failed
			con::log("LongPoll failed.");
			if (err["failed"].GetInt() != 1) getServer();
			else params["ts"] = to_string(err["ts"].GetInt());
		}
	}
}
