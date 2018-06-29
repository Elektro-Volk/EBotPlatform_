#include "host.h"
// Subsystems
#include "console.h"
#include "cmd.h"
#include "net.h"
#include "vk/vkapi.h"
#include "vk/longpoll.h"
#include "lua/luawork.h"

int host::runApplication()
{
    host::initSystems();
    host::startSystems();

    longpoll::con->loop();
	return 1;
}

void host::initSystems()
{
    con::init();
	net::init();
	vk::init();
	luawork::init();
	longpoll::init();
	cmd::init();
}

void host::startSystems()
{
    cmd::start();
	vk::start();
	luawork::start();
	longpoll::start();
}

// TODO
void HostEnd(int code)
{
	//exit(code);
}
