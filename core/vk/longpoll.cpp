#include "longpoll.h"

LongPollConnection* longpoll::con;
cvar longpoll::lp_debug;

void longpoll::init()
{
	lp_debug = cvars::add("lp_debug", "0");
}

void longpoll::start()
{
	con = new LongPollConnection();
	con->getServer(); 
}
