#include "vkapi.h"
#include "net.h"
#include "cvars.h"
#include "console.h"
#include <chrono>
#include <thread>

cvars::Cvar *vk::token;
cvars::Cvar *vk::version;
cvars::Cvar *vk::groupmode;
cvars::Cvar *vk::groupid;

void vk::init()
{
  vk::token = cvars::add("vk_token", "-");
  vk::version = cvars::add("vk_version", "5.73");
  vk::groupmode = cvars::add("vk_group", "0");
  vk::groupid = cvars::add("vk_groupid", "0");
}

void vk::start()
{
  if (groupmode->getBool()) return;
  auto data = vk::jSend("stats.trackVisitor");
	if (data.HasMember("error")) {
		con::error("Wrong vk_token");
    con::error("Please, setting you config.cfg");
		HostEnd(-1);
	}
}

string vk::send(string method, map<string, string> params)
{
  //	if (method == "messages.send")// Send stats
  //		net::send("http://ebp.elektro-volk.ru/api/addstat.php", { {"key", cvar::get ("eapi_key")} });
  if(params.find("access_token") == params.end()) params["access_token"] = token->value;
  params["v"] = version->value;
  return net::POST("https://api.vk.com/method/" + method, params);
}

rapidjson::Document vk::jSend(string method, map<string, string> params)
{
  rapidjson::Document data;
  data.Parse(vk::send(method, params).c_str());
  if (data.HasMember("error") && data["error"]["error_code"].GetInt() == 6) { // error 6
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    return vk::jSend(method, params);
  }
  return data;
}
