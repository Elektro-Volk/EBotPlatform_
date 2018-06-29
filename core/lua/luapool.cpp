#include "luawork.h"
#include "luapool.h"
#include "luai.h"
#include "lua_json.h"
#include "cvars.h"
#include "console.h"
#include "cmd.h"
#include "net.h"

extern Nextlist<thread::id, CURL*> handles;

char luapool::nWorkers;
std::vector<luapool::Worker*> luapool::workers;

cvar luapool::pool_count;

void luapool::init()
{
    pool_count = cvars::add("pool_count", "2");
    cmd::add("pool", luapool::c_pool, "Lua pool list");
}

void luapool::start()
{
    nWorkers = 0;
    for(int i = 0; i < pool_count->getInt(); i++)
        workers.push_back(new Worker());
}

string luapool::c_pool(vector<string> _args)
{
    string ret = "\nLua pool:";
	for (int i = 0; i < workers.size(); i++)
	{
		ret += "\n" + to_string(i) + " - " + (workers[i]->isBusy() ? "busy" : "free");
	}
	return ret;
}

void luapool::close()
{
    for(int i = 0; i < workers.size(); i++) delete workers[i];
    workers.clear();
}

void luapool::add(rapidjson::Value &msg)
{
    // find free pool and add msg then
    Worker *freeWorker = nullptr;
    while(!freeWorker){
        for(int i = 0; i < workers.size(); i++) {
            if(workers[i]->isBusy()) continue;
            freeWorker = workers[i];
            break;
        }
    }
    freeWorker->add(msg);
}

bool luapool::Worker::isBusy()
{
    return busy;
}

void luapool::Worker::add(rapidjson::Value &msg)
{
    std::unique_lock<std::mutex> locker(mutex);
    busy = true;

    lua_unlock(L);
    lua_settop(L, 0);
    lua_getglobal(L, "NewMessage");
    lua_json::pushValue(L, msg);
    lua_lock(L);
    cv.notify_one();
}

void luapool::Worker::loop()
{
    while (enabled) {
        std::unique_lock<std::mutex> locker(mutex);
		cv.wait(locker, [&](){ return busy || !enabled; });

		if (!busy) continue;
        lua_unlock(L);
        luawork::safeCall(L, 1);
        lua_lock(L);
        busy = false;
	}
}

/***** Worker class *****/
luapool::Worker::Worker()
:enabled(true), busy(false), thread(&luapool::Worker::loop, this)
{
    handles.push(thread.get_id(), curl_easy_init());

    id[0] = nWorkers++;
	L = lua_newthread(luawork::state);
	lua_setfield(luawork::state, LUA_REGISTRYINDEX, id);
}

luapool::Worker::~Worker()
{
    enabled = false;
    while(busy){}
    curl_easy_cleanup(handles[thread.get_id()]);
    handles.pop(thread.get_id());
    cv.notify_one();
    thread.join();
}
