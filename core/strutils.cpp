#include "strutils.h"
#include <codecvt>
#include <locale>
#include <iostream>
#include <string>
#include <sstream>
#include <codecvt>

#ifdef __linux__
	std::locale const utf8("en_US.UTF-8");
#elif _WIN32
	std::locale const utf8("rus");
#endif

std::wstring to_wstring(std::string const& s) {
  std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
  return conv.from_bytes(s);
}

std::string to_string(std::wstring const& s) {
  std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
  return conv.to_bytes(s);
}

bool strutils::starts(string str, string token)
{
  return str.rfind(token, 0) == 0;
}

string strutils::toLower(string str)
{
  auto ss = to_wstring(str);
  for (auto& c : ss) {
    c = std::tolower(c, utf8);
  }
  return to_string(ss);
}

string strutils::toUpper(string str)
{
  auto ss = to_wstring(str);
  for (auto& c : ss) {
    c = std::toupper(c, utf8);
  }
  return to_string(ss);
}
 
vector<string> strutils::split(string str, char sep)
{
  vector<string> tokens;
	istringstream ss(str);

	for (string item; std::getline(ss, item, sep);)
		tokens.push_back(item);

	return tokens;
}
