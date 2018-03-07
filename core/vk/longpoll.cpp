#include "longpoll.h"
#include "vkapi.h"
#include "../console.h"
#include "../lua/luawork.h"
#include "../net.h"
#include <thread>

longpoll::LongPoll* longpoll::lp;

void longpoll::start()
{
	longpoll::lp = new longpoll::LongPoll();
	lp->loop();
}

longpoll::LongPoll::LongPoll()
{
	getServer();
}

void longpoll::LongPoll::getServer()
{
	con::log("Getting longpoll server...");
	rapidjson::Value &data = vk::jSend("messages.getLongPollServer", {{ "lp_version", "2" }})["response"];

	server = data["server"].GetString();
	params = {
		{ "ts", to_string(data["ts"].GetInt()) },
		{ "key", data["key"].GetString() },
		{ "version", "2" },
		{ "act", "a_check" },
		{ "wait", "25" }
	};
}

void longpoll::LongPoll::onFailed(rapidjson::Document& data)
{
	/*int code = data["failed"];
	switch(code){
		//case 1: params["ts"] = to_string((int)data["ts"]); break;
		case 2: getServer(); break;
		case 3: getServer(); break;
		case 4: break;
	}*/
}

void longpoll::LongPoll::loop()
{
	con::log("LongPoll was successfully launched");
	while (true) {
		try {
			rapidjson::Document data;
			if (data.Parse(net::POST("https://" + server, params).c_str()).HasParseError())
        throw new string("Json parse error");
			if(data.HasMember("failed")) {
				onFailed(data);
				continue;
			}
			params["ts"] = to_string(data["ts"].GetInt());
			rapidjson::Value &updates = data["updates"];
			for (size_t i = 0; i < updates.Size(); i++) {
				if(updates[i][0] == 4)
					luawork::push(updates[i]);
			}
		}
		catch (string *err) {
			con::error(*err);
			delete err;
		}
	}
}
