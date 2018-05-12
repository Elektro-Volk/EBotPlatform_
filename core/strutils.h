#pragma once
#include <vector>
#include "common.h"

namespace strutils {
  bool starts(string str, string token);
  string toLower(string str);
  string toUpper(string str);
  void replace(string& str, const string& old, const string& repl);
  vector<string> split (string str, char sep);
}
