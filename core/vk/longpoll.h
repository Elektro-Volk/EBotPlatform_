#pragma once
#include "common.h"
#include "LongPollConnection.h"

namespace longpoll {
  extern cvar lp_debug;
  extern LongPollConnection* con;

  void init();
  void start();
}
