#pragma once
#include "../common.h"
#include "../json.hpp"
#include <map>

namespace longpoll {
  class LongPoll {
  private:
    string server;
    map<string, string> params;
    void getServer();
    void onFailed(nlohmann::json data);
  public:
    LongPoll();
    void loop();
  };

  extern LongPoll* lp;

  void start();
}
