#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H
#include <cstring>
#include <int2048.h>
#include <Constant.h>
bool IsNumber(const std::string &val)
{
  for (auto i : val)
    if (i != '-' && i != '.' && (i < '0' || i > '9')) return false;
  return true;
}

python_consts::kTYPE GetType(const std::any &x)
{
  if (x.type() == typeid(double)) { return python_consts::FLOAT; }
  else if (x.type() == typeid(sjtu::int2048)) { return python_consts::INT; }
  else if (x.type() == typeid(bool)) { return python_consts::BOOL; }
  else if (x.type() == typeid(std::string)) { return python_consts::STR; }
  else if (x.type() == typeid(std::vector<std::any>)) { return python_consts::TUPLE; }
  else if (!x.has_value()) { return python_consts::NONE; }
  else { assert(false); }
}

#endif //PYTHON_INTERPRETER_UTILS_H
