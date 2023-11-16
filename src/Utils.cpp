#include <int2048.h>
#include <Constant.h>
#include <cassert>
#include <any>
#include <Utils.h>
bool IsNumber(const std::string &val)
{
  for (auto i : val)
    if (i != '-' && i != '.' && (i < '0' || i > '9')) return false;
  return true;
}

bool IsFloat(const std::string &val)
{
  if (!IsNumber(val)) return false;
  for (auto i : val)
    if (i == '.') return true;
  return false;
}

python_consts::ktype GetType(const std::any &x)
{
  if (x.type() == typeid(double))
  {
    return python_consts::FLOAT;
  }
  else if (x.type() == typeid(sjtu::int2048))
  {
    return python_consts::INT;
  }
  else if (x.type() == typeid(bool))
  {
    return python_consts::BOOL;
  }
  else if (x.type() == typeid(std::string))
  {
    return python_consts::STR;
  }
  else if (x.type() == typeid(std::vector<std::any>))
  {
    return python_consts::TUPLE;
  }
  else if (!x.has_value())
  {
    return python_consts::NONE;
  }
  else if (x.type() == typeid(std::pair<bool, std::any>))
  {
    return python_consts::VAR;
  }
  else { assert(false); }
}

bool GetFloat(const std::any &x)
{
  return GetType(x) == python_consts::FLOAT;
}

bool GetInt(const std::any &x)
{
  return GetType(x) == python_consts::INT;
}

bool GetBool(const std::any &x)
{
  return GetType(x) == python_consts::BOOL;
}

bool GetStr(const std::any &x)
{
  return GetType(x) == python_consts::STR;
}

bool GetTuple(const std::any &x)
{
  return GetType(x) == python_consts::TUPLE;
}

bool GetNone(const std::any &x)
{
  return GetType(x) == python_consts::NONE;
}

bool GetVar(const std::any &x)
{
  return GetType(x) == python_consts::VAR;
}

std::any* GetVarAddr(const std::any &x)
{
  if (!GetVar(x)) assert(false);
  auto tmp = std::any_cast<std::pair<bool, std::any>>(x).second;
  return std::any_cast<std::any*>(tmp);
}