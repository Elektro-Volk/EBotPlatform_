#pragma once
#include "../common.h"
#include "luai.h"
#include <vector>

namespace luaModules {
		struct Module {
			string name;
			bool isLoaded;
		};

		extern vector<Module> list;

		string c_madd(vector<string> cmd_args);
		string c_mrem(vector<string> cmd_args);
		string c_mlist(vector<string> cmd_args);
		void loadModules(lua_State *L);
		void load();
		void save();
	};
