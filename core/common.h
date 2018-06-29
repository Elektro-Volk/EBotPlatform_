#pragma once
#include <string>
#include "cvars.h"
// To mingw32
#ifdef __MINGW32__
  #include "../mingwthreads/mingw.thread.h"
#endif

using namespace std;

extern string bot_path;
extern const string _version;
extern const int startTime;

void HostEnd(int code);
