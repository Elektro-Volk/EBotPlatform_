/*
  Filename: net.cpp
  Description:
    Wrapper around CURL to work with Internet requests

  Elektro-Volk 2017-2018
  <elektro-volk@yandex.ru>
*/
#ifdef __MINGW32__
  #include <windows.h>
#endif
#include "net.h"
#include "net_curlsetup.h"
#include <stdlib.h>
#include <stdio.h>// test
#include <cstring>
#include "lock.h"
#include <vector>
#include "console.h"

extern thread::id main_thread_id;
extern Nextlist<thread::id, CURL*> handles;

void net::init()
{
    curl_global_init(CURL_GLOBAL_ALL);
    handles.push(main_thread_id, curl_easy_init());
}

void net::setup_curl(CURL *handle, string *buffer)
{
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, net::_curlWriteCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)buffer);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "EBP/0.9");
}

size_t net::_curlWriteCallback(char *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  ((string*)data)->append(ptr, realsize);
  return realsize;
}

/* Encode string to url */
string urlEncode(string str)
{
	char* esc_text = curl_easy_escape(NULL, str.c_str(), str.length());
	if (!esc_text) throw runtime_error("Can not convert string to URL");
	string result = esc_text;
	curl_free(esc_text);
	return result;
}

string net::makeFields(map<string, string> &fields)
{
  if(fields.size() == 0) return "";

	string paramline;
	for (auto iter = fields.begin(); iter != fields.end(); iter++) {
		paramline += iter->first + "=" + urlEncode(iter->second) + "&";
	}

	return paramline;
}

/* Simple GET Request */
string net::GET(string url)
{
    curltuner tuner = curltuner();

    curl_easy_setopt(tuner.handle, CURLOPT_URL, url.c_str());

    CURLcode result_code = curl_easy_perform(tuner.handle);
    if(result_code != CURLE_OK) throw new net::Exception(result_code);

    //con::log(tuner.buffer);
    return tuner.buffer;
}

string net::POST(string url, const char* data)
{
  curltuner tuner = curltuner();

  curl_easy_setopt(tuner.handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(tuner.handle, CURLOPT_POST, 1);
  curl_easy_setopt(tuner.handle, CURLOPT_POSTFIELDS, data);

  CURLcode result_code = curl_easy_perform(tuner.handle);
  if(result_code != CURLE_OK) throw new net::Exception(result_code);

  //con::log(tuner.buffer);
  return tuner.buffer;
}

string net::POST(string url, map<string, string> &fields)
{
  return POST(url, makeFields(fields).c_str());
}
