#include "eapi.h"
#include "../vk/vkapi.h"
#include "../net.h"
#include "../cvar.h"
#include "../console.h"

string eapi::key = "-";
string eapi::url = "http://ebp.elektro-volk.ru/api/";

void eapi::init()
{
  //cvars::add("eapi_key", &key);
}

void eapi::start()
{
/*  con::log("Authorizing in EAPI...");
  auto data = nlohmann::json::parse(net::GET(url + "check.php?key=" + key));
	if (data.find("error") != data.end()) {
		con::error("Wrong eapi_token.");
    gentoken();
	}*/
}

void eapi::gentoken()
{
  con::log("Getting a new key...");
  vk::send(
    "messages.send",
    { { "peer_id", "-162493545" }, { "message", "GEN" + to_string(rand()) } }
  );
}
