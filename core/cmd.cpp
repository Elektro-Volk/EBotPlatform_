#include <cstring>
#include <fstream>
#include "cmd.h"
#include "console.h"
#include "cvars.h"
#include <thread>
#include <iostream>
#include <map>

using namespace cmd;

std::map<std::string, command> commands;

void cmd::init()
{
	cmd::add("exec", c_exec, "Execute config file");
	cmd::add("help", c_help, "Get command list");
}

void cmd::start()
{
	cmd::exec("config.cfg");
	std::thread(readLoop).detach();
}

void cmd::readLoop()
{
	for (string line;;) {
		std::getline(std::cin, line);
		logExe(line);
	}
}

void cmd::logExe(string line)
{
	try {
		con::log(cmd::exe(line));
	}
	catch (string* error) {
		con::error(*error);
		delete error;
	}
}

bool cmd::exists(const string cmd_name)
{
	return commands.find(cmd_name) != commands.end();
}

string cmd::c_exec(args cmd_args)
{
	if (cmd_args.size() != 2)
		throw new string("Use \"exec <config path>\"");
	exec(cmd_args[1]);
	return "executing " + cmd_args[1] + "...";
}

string cmd::c_help(args cmd_args)
{
	string ret = "\nEBP "+_version+" commands:";
	for (auto cmd : commands)
		ret += "\n"+cmd.first + " - " + cmd.second.desc;
	return ret;
}

void cmd::add(string cmd_name, func function, string cmd_desc)
{
	if (exists(cmd_name)) return con::error(cmd_name +" already defined");
	if (cvars::exists(cmd_name)) return con::error(cmd_name+" already defined as a cvar");
	commands[cmd_name] = { cmd_name, function, cmd_desc };
}

string cmd::exe(const string text)
{
	if (text == "") return "";
	args cmdArgs = parse(text);
	string cmdName = cmdArgs[0];

	if (exists(cmdName))
		return commands.find(cmdName)->second.function(cmdArgs);
	else if (cvars::exists(cmdName))
	{
		if (cmdArgs.size() == 2) {
			cvars::get(cmdName)->setValue(cmdArgs[1]);
			return cmdName + " = " + cmdArgs[1];
		}
		else
			return cmdName + " = " + cvars::get(cmdName)->value;
	}
	else
		return "Unknown command \"" + cmdName + "\"";
}

args cmd::parse(string text)
{
	bool nospace = 0;
	int index = 0;
	args cmd_args;

	if (text.empty()) return args();
	if (text.find(' ') == std::string::npos) {
		cmd_args.push_back(text);
		return cmd_args;
	}

	cmd_args.push_back("");
	for (int i = 0;i < strlen(text.c_str());i++) {
		char ch = text.c_str()[i];
		if (ch == ' ' && !nospace) {
			index++;
			cmd_args.push_back("");
		}
		else if (ch == '"')
			nospace = !nospace;
		else
			cmd_args[index] += ch;
	}
	return cmd_args;
}



void cmd::exec(std::string cpath)
{
  ifstream fs(bot_path + '/' + cpath);

	string data;
	string line;

	 while (getline(fs, line))
	 {
		if (line == "" || line.front() == '#')
			continue;
		exe(line);
	}
	fs.close();
}



string cmd::data(args cmd_args, int sub)
{
	if (cmd_args.size() < sub)
		return "";
	string ret = "";
	for (int i = sub-1; i < cmd_args.size(); i++)
	{
		ret += " " + cmd_args[i];
	}
	return ret.substr(1);
}
