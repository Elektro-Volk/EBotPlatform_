#include "cvars.h"
#include <map>

map<string, cvars::Cvar*> cvars::cvars;

cvars::Cvar::Cvar(string name, string value)
{
    this->value = value;
    this->name = name;
}

void cvars::Cvar::setValue(string new_value)
{
  this->value = new_value;
  if(stringCallback)
    *stringCallback = new_value;
}

bool cvars::Cvar::getBool()
{
  return value == "1";
}

int cvars::Cvar::getInt()
{
  return stoi(value);
}

string cvars::Cvar::getString()
{
  return value;
}

cvars::Cvar *cvars::add(string name, string value)
{
  Cvar *cvar = new Cvar(name, value);
  cvars.insert ( pair<string,cvars::Cvar*>(name, cvar));
  return cvar;
}

cvars::Cvar *cvars::add(string name, string* value)
{
  Cvar *cvar = new Cvar(name, *value);
  cvar->stringCallback = value;
  cvars.insert ( pair<string,cvars::Cvar*>(name, cvar));
  return cvar;
}

cvars::Cvar *cvars::get(string name)
{
  return cvars[name];
}

bool cvars::exists(string name)
{
  return cvars.find(name) != cvars.end();
}

void cvars::remove(cvars::Cvar *cvar)
{
  //cvars.pop(cvar->name);
}
