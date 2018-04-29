#include "filesystem.h"
#include <sys/stat.h>
#include <fstream>
#include <string>
#include "dirent.h"

bool fs::exists (string fname)
{
  struct stat buffer; // I don't know how it works
  return (stat ((bot_path + '/' + fname).c_str(), &buffer) == 0);
}


string fs::read(string fname)
{
  ifstream file(bot_path + '/' + fname);
  string data;
  for (string line; std::getline(file, line);) // =)
    data += line;
  file.close();
  return data;
}

vector<string> fs::readLines(string fname)
{
  vector<string> data;
  ifstream file(bot_path + '/' + fname);
  for (string line; std::getline(file, line);) // =)
    data.push_back(line);
  file.close();
  return data;
}

void fs::write(string fname, string data)
{
  ofstream file(bot_path + '/' + fname);
  file << data;
  file.close();
}

void fs::writeLines(string fname, vector<string> data)
{
  ofstream file(bot_path + '/' + fname);
  for(int i = 0; i < data.size(); i++)
    file << data[i] << std::endl;
  file.close();
}

vector<string> fs::dirList(string _dir)
{
  vector<string> data;
  DIR *dir = opendir((_dir[0] == '/' ? _dir : bot_path + '/' + _dir).c_str());
  if(dir)
  {
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL)
    {
      string f = ent->d_name;
      if(f != "." && f != "..")
      data.push_back(f);
    }
  }
  return data;
}
/*---------- LUA CODE -----------*/

int fs::lua::exists(lua_State *L)
{
  lua_pushboolean(L, fs::exists(luaL_checkstring(L, 1)));
  return 1;
}

int fs::lua::read(lua_State *L)
{
  lua_pushfstring(L, fs::read(luaL_checkstring(L, 1)).c_str());
  return 1;
}

int fs::lua::readLines(lua_State *L)
{
  vector<string> lines = fs::readLines(luaL_checkstring(L, 1));
  lua_newtable(L);
  for (int i = 0; i < lines.size(); i++) {
    lua_pushinteger(L, i + 1);
    lua_pushfstring(L, lines[i].c_str());
    lua_settable(L, -3);
  }
  return 1;
}

int fs::lua::write(lua_State *L)
{
  fs::write(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
  return 0;
}

int fs::lua::writeLines(lua_State *L)
{
  return 0; // TODO
}

int fs::lua::dirList(lua_State *L)
{
  vector<string> data = fs::dirList(luaL_checkstring(L, 1));
  lua_newtable(L);
  for (int i = 0; i < data.size(); i++) {
    lua_pushinteger(L, i + 1);
    lua_pushstring(L, data[i].c_str());
    lua_settable(L, -3);
  }
  return 1;
}
