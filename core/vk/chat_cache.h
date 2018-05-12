/*
chat_cache.h
Copyright (C) 2018 Elektro-Volk
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#pragma once
#include "common.h"
#include <map>
#include <vector>

struct chat_info {
  int id;
  string type;
  string title;
  int admin_id;
  std::vector<int> users;
};

namespace chat_cache {
  extern std::map<int, chat_info> chats;

  void updateInfo(int id);
  chat_info *getChat(int id);

  string c_clist(vector<string> _args);
  string c_cupd(vector<string> _args);
  string c_crem(vector<string> _args);
  string c_cinfo(vector<string> _args);
};
