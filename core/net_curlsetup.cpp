//std::lock_guard<std::mutex> lock(mutex);
/*
net_CurlObject.cpp
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
#include "net.h"
#include "net_curlsetup.h"
#include <thread>

using thread = std::thread;

thread::id main_thread_id  = std::this_thread::get_id();
Nextlist<thread::id, CURL*> handles;

curltuner::curltuner()
{
    handle = handles[std::this_thread::get_id()];
    net::setup_curl(handle, &buffer);
}

curltuner::~curltuner()
{
    curl_easy_reset(handle);
}
