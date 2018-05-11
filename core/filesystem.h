#pragma once
#include "common.h"
#include <vector>
#include "lua/luai.h"

namespace fs
{
  bool exists(string fname);
  string read(string fname);
  vector<string> readLines(string fname);
  void write(string fname, string data);
  void writeLines(string fname, vector<string> data);
  vector<string> dirList(string dir);
}
