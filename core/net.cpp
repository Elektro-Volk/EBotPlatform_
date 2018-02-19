/*
  Filename: net.cpp
  Description:
    Wrapper around CURL to work with Internet requests

  Elektro-Volk 2017-2018
  <elektro-volk@yandex.ru>
*/
#include "net.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <curl/curl.h>
#include "lock.h"
#include "lua/lJson.h"
#include <vector>
#include "console.h"

/* CURL CROSSTHREAD */
CURLSH* share;
LSECT net_lock;

void CURLlock(CURL *handle, curl_lock_data data, curl_lock_access access,void *useptr ){
  lock_lock(net_lock);
}

/* unlock callback */
void CURLunlock(CURL *handle, curl_lock_data data, void *useptr ){
lock_unlock(net_lock);
}


net::Request::Request(string url)
{
  this->url = url;
}

void net::Request::setData(const char* data)
{
  this->data = data;
}

string net::Request::send()
{
  string buffer;

  curl_global_init(CURL_GLOBAL_ALL); // TOMOVE
  CURL *curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_SHARE, share);
  curl_easy_setopt(curl_handle, CURLOPT_URL, this->url.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, net::_curlWriteCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buffer);
  //curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  if(headers) {
    struct curl_slist *chunk = NULL;
    for(int i = 0; i < nHeaders; i++)
      chunk = curl_slist_append(chunk, headers[i]);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
  }
  if(mimes.size() > 0) {
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    for (auto mime = mimes.begin(); mime != mimes.end(); mime++) {
      curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, mime->first,
              CURLFORM_FILE, mime->second, CURLFORM_CONTENTTYPE, "multipart/form-data", CURLFORM_END);
  	}
     curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
  }
  if(data){
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
  	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
  }

  CURLcode res = curl_easy_perform(curl_handle);
  if(res != CURLE_OK) {
    curl_easy_cleanup(curl_handle);
    throw new string("curl_easy_perform(" + this->url + ") failed: " + string(curl_easy_strerror(res)));
  }
  else {
    curl_easy_cleanup(curl_handle);
    return buffer;
  }
}

void net::init()
{
  share = curl_share_init();
  net_lock = lock_new();

  curl_share_setopt( share, CURLSHOPT_LOCKFUNC, CURLlock);
  curl_share_setopt( share, CURLSHOPT_UNLOCKFUNC, CURLunlock);
  curl_share_setopt(share,CURLSHOPT_SHARE,CURL_LOCK_DATA_DNS);
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

char* net::makeFields(map<string, string> &fields)
{
  if(fields.size() > 0) {
	string paramline;
	for (auto iter = fields.begin(); iter != fields.end(); iter++) {
		paramline += iter->first + "=" + urlEncode(iter->second) + "&";
	}
  char *data = new char[paramline.size()];
  memcpy(data, paramline.c_str(), paramline.size());
	return data;
  }
  else
   return NULL;
}

/* Simple GET Request */
string net::GET(string url)
{
  Request *request = new Request(url);
  string data = request->send();
  delete request;
  return data;
}

string net::POST(string url, const char* data)
{
  Request *request = new Request(url);
  request->setData(data);
  string _data = request->send();
  delete request;
  return _data;
}

string net::POST(string url, map<string, string> &fields)
{
  const char* data = makeFields(fields);
  string _data = POST(url, data);
  delete data;
  return _data;
}

/* LUA */

// string net.send(url, [params|data])
int net::lua::send(lua_State* L)
{
  luaL_checktype(L, 1, LUA_TSTRING);
  if(lua_gettop(L) == 1) { // GET
    string data = ::net::GET(lua_tostring(L, 1));
    lua_pushlstring(L, data.c_str(), data.size());
  }
  else {
    if(lua_istable(L, 2)) { // Params
      Request *request = new Request(lua_tostring(L, 1));
      vector<const char*> headers;
      map<string, string> _args;
      map<const char*, const char*> mimes;
      lua_pushnil(L);
      while (lua_next(L, 2)) {
        lua_pushvalue(L, -2);
        const char* param = luaL_checkstring(L, -1);
        if(param[0] == '!')
          headers.push_back(luaL_checkstring(L, -2));
        else if(param[0] == '#')
          mimes.insert(pair<const char*, const char*>(string(param).substr(1).c_str(), luaL_checkstring(L, -2)));
        else
          _args.insert(pair<string, string>(param, luaL_checkstring(L, -2)));
        lua_pop(L, 2);
      }

      const char* fields = makeFields(_args);
      request->setData(fields);
      if(headers.size() > 0) {
        request->headers = headers.data();
        request->nHeaders = headers.size();
      }
      request->mimes = mimes;

      lua_pushstring(L, request->send().c_str());
      delete request;
      delete fields;
    }
    else {
      lua_pushstring(L, ::net::POST(lua_tostring(L, 1), lua_tostring(L, 2)).c_str());
    }
  }
  return 1;
}

// table net.jSend(method, [data])
int net::lua::jSend(lua_State *L)
{
  send(L);
  lJson::decode(L);
  return 1;
}
