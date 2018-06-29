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
#ifdef __linux__
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
#else
  string search_path = (_dir[0] == '/' ? _dir : bot_path + '/' + _dir) + "/*.*";
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if(hFind != INVALID_HANDLE_VALUE) {
      do {
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                data.push_back(fd.cFileName);
            }
      }while(::FindNextFile(hFind, &fd));
      ::FindClose(hFind);
    }
#endif
  return data;
}
