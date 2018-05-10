#pragma once
#include "common.h"
#include "LongPollConnection.h"
#include "GroupLongPollConnection.h"

namespace longpoll {
  extern cvar lp_debug;
  extern cvar lp_ignore_myself;
  extern LongPollConnection* con;

  void start();
}
