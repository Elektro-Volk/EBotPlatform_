#pragma once
#include "../common.h"
#include "rapidjson/document.h"
#include <map>

namespace longpoll {
  class LongPoll {
  private:
    string server;
    map<string, string> params;
    void getServer();
  public:
    LongPoll();
    void loop();
  };

  extern LongPoll* lp;

  void start();
}
