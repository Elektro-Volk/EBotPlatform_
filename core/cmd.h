#pragma once
#include "common.h"
#include <vector>
#include "lua/luai.h"

namespace cmd
{
	typedef std::vector<string> args;

	typedef string(*func)(vector<string> cmd_args);
	struct command
	{
		string name;
		func function;
		string desc;
	};

	void init();
	void start();
	void readLoop();
	void logExe(string line);
	void add(string cmd_name, func function, string cmd_desc);
	string exe(string text);
	vector<string> parse(string line);
	bool exists(const string cmd_name);
	void exec(const string cpath);
	string c_exec(args cmd_args);
	string c_help(args cmd_args);
	string data(args cmd_args, int sub);
}
