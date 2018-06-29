#include "console.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

// Colors
const string col::BOLD = "\033[1m";
const string col::red = "\033[0;31m";
const string col::RED = "\033[1;31m";
const string col::GREEN = "\033[0;32m";
const string col::green = "\033[1;32m";
const string col::YELLOW = "\033[0;33m";
const string col::yellow = "\033[1;33m";
const string col::blue = "\033[0;34m";
const string col::BLUE = "\033[1;34m";
const string col::MAGENTA = "\033[0;35m";
const string col::magenta = "\033[1;35m";
const string col::cyan = "\033[0;36m";
const string col::CYAN = "\033[1;36m";
const string col::NC = "\033[0m";


string getLogTime()
{
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%H:%M:%S");
  return oss.str();
}

void con::init()
{
    #ifdef __MINGW32__
		SetConsoleOutputCP(CP_UTF8);
	#endif

	FILE *file = fopen((bot_path + "/ebp.log").c_str(), "w");
  if(file) fclose(file);

  con::log("EBotPlatform V" + _version);
  con::log("Copyright Elektro-volk 2016-2018");
}

string makeText(string data)
{
  #ifdef __linux__
    return col::blue + "[" + col::BLUE +  getLogTime() + col::blue + "] " + col::NC + data + "\n" + col::NC;
  #else
    return "[" +  getLogTime() + "] " + data + "\n";
  #endif

}

void con::log(string data)
{
  string text = makeText(data);

  std::cout << text;

  if (FILE *file = fopen((bot_path + "/ebp.log").c_str(), "a")) {
		fprintf(file, text.c_str());
		fclose(file);
	}
}

void con::error(string data)
{
  string text = makeText(col::RED + "Error: " + data);
  std::cerr << text;

  if (FILE *file = fopen((bot_path + "/ebp.log").c_str(), "a")) {
		fprintf(file, text.c_str());
		fclose(file);
	}
}
