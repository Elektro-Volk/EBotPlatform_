#include "longpoll.h"
#include "vkapi.h"
#include "../console.h"
#include "../lua/luawork.h"
#include "../net.h"
#include <thread>

cvar longpoll::lp_debug = cvars::add("lp_debug", "0");
cvar longpoll::lp_ignore_myself = cvars::add("lp_ignore_myself", "1");

LongPollConnection* longpoll::con;

void longpoll::start()
{
	con = vk::groupmode->getBool() ? new GroupLongPollConnection() : new LongPollConnection();
	con->getServer();
	con->loop();
}
