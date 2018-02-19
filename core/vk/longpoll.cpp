#include "longpoll.h"
#include "vkapi.h"
#include "../console.h"
#include "../lua/luawork.h"
#include "../net.h"
#include <thread>

using json = nlohmann::json;

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
	json data = vk::jSend("messages.getLongPollServer", {{ "lp_version", "2" }})["response"];

	string _key = data["key"];
	string _server = data["server"];// Json bug
	server = _server;

	params = {
		{ "ts", to_string((int)data["ts"]) },
		{ "key", _key },
		{ "version", "2" },
		{ "act", "a_check" },
		{ "wait", "25" }
	};
}

void longpoll::LongPoll::onFailed(json data)
{
	int code = data["failed"];
	switch(code){
		case 1: params["ts"] = to_string((int)data["ts"]); break;
		case 2: getServer(); break;
		case 3: getServer(); break;
		case 4: break;
	}
}

void longpoll::LongPoll::loop()
{
	con::log("LongPoll was successfully launched");
	while (true) {
		try {
			json data = json::parse(net::POST("https://" + server, params));
			if(data.find("failed") != data.end()) {
				onFailed(data);
				continue;
			}
			params["ts"] = to_string((int)data["ts"]);
			json updates = data["updates"];
			for (json update : updates) {
				if((int)update.at(0) == 4)
        	luawork::push(update);
			}
		}
		catch (nlohmann::detail::parse_error e) {
			con::error(e.what());
		}
		catch (string *err) {
			con::error(*err);
			delete err;
		}
	}
}
