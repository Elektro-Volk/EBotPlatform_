#include "strutils.h"
#include "console.h"
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include <sstream>
#include "utf8cases.h"

std::wstring to_wstring(std::string const& s)
{
#ifdef __linux__
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(s);
#else
	std::wstring buf(s.size(),0);
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.length(), const_cast<wchar_t*>(buf.data()), s.size());
  return buf;
#endif
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
    c = std::tolower(c);
		for (int j = 0; j < UTF8CASES_SIZE; j++)
			if (c == cases[j][0]) { c = cases[j][1]; break; }
  }
  return to_string(ss);
}

string strutils::toUpper(string str)
{
	auto ss = to_wstring(str);
  for (auto& c : ss) {
    c = std::toupper(c);
		for (int j = 0; j < UTF8CASES_SIZE; j++)
			if (c == cases[j][1]) { c = cases[j][0]; break; }
  }
  return to_string(ss);
}

void strutils::replace(std::string& str, const std::string& old, const std::string& repl) {
size_t pos = 0;
while ((pos = str.find(old, pos)) != std::string::npos) {
str.replace(pos, old.length(), repl);
pos += repl.length();
}
}

vector<string> strutils::split(string str, char sep)
{
  vector<string> tokens;
	istringstream ss(str);

	for (string item; std::getline(ss, item, sep);)
		tokens.push_back(item);

	return tokens;
}
