#include "common.h"
#include <locale> // setlocale
#include "console.h" // con::init, con::log
#include "vk/longpoll.h" // longpoll::start
#include "vk/vkapi.h" // vk::init, vk::start
#include "eapi/eapi.h"
#include "lua/luawork.h" // luawork::init, luawork::start
#include "net.h" // net::init
#include "cmd.h" // cmd::init, cmd::exec, cmd::start

string bot_path = "bot";
const string _version = "0.6.1.2";
int startTime = time(0);

int main(int argc, char* argv[])
{
	//setlocale(LC_ALL, "");
	// Initialization
	con::init();
	con::log("Initialization EBotPlatform V" + _version + "...");
	//eapi::init();
	net::init();
	vk::init();
	luawork::init();
	cmd::init();
	con::log("Copyright Elektro-volk 2016-2018");
	// Starting
	cmd::exec("config.cfg");
	cmd::start();
	vk::start();
	//eapi::start();
	luawork::start();
	longpoll::start(); // Main loop

	return 1;
}

void HostEnd(int code)
{
	exit(code);
}
