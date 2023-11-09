#ifndef PYTHON_INTERPRETER_BUILTINFUNCS_H
#define PYTHON_INTERPRETER_BUILTINFUNCS_H
#include <cstring>
#include <cassert>
#include <any>
#include <int2048.h>
#include <Utils.h>
#include <Constant.h>
sjtu::int2048 ToInt(const std::any &val)
{
  if (val.type() == typeid(sjtu::int2048))
  {
    auto x = std::any_cast<sjtu::int2048>(val);
    return x;
  }
  else if (val.type() == typeid(std::string))
  {
    auto s = std::any_cast<std::string>(val);
    if (!IsNumber(s)) assert(false);
    if (s.find('.') == std::string::npos) { return sjtu::int2048(s); }
    else { return sjtu::int2048(s.substr(0, s.find('.')));}
  }
  else if (val.type() == typeid(bool))
  {
    auto x = std::any_cast<bool>(val);
    return x? 1:0;
  }
  else if (val.type() == typeid(double))
  {
    auto x = std::any_cast<double>(val);
    return (long long) x;
  }
  else { assert(false); }
}
double ToFloat(const std::any &val)
{
  if (val.type() == typeid(double))
  {
    auto x = std::any_cast<double>(val);
    return x;
  }
  else if (val.type() == typeid(sjtu::int2048))
  {
    auto x = std::any_cast<sjtu::int2048>(val);
    return x.ToDouble();
  }
  else if (val.type() == typeid(bool))
  {
    auto x = std::any_cast<bool>(val);
    return x? 1.0:0.0;
  }
  else if (val.type() == typeid(std::string))
  {
    auto s = std::any_cast<std::string>(val);
    if (!IsNumber(s)) assert(false);
    return std::stod(s);
  }
  else { assert(false); }
}
std::string ToString(const std::any &val)
{
  std::ostringstream oss;
  if (val.type() == typeid(std::string))
  {
    auto s = std::any_cast<std::string>(val);
    return s;
  }
  else if (val.type() == typeid(sjtu::int2048))
  {
    auto x = std::any_cast<sjtu::int2048>(val);
    oss << x;
    return oss.str();
  }
  else if (val.type() == typeid(bool))
  {
    auto x = std::any_cast<bool>(val);
    return x? "True":"False";
  }
  else if (val.type() == typeid(float))
  {
    auto x = std::any_cast<double>(val);
    oss << x;
    return oss.str();
  }
  else { assert(false); }
}
bool ToBool(const std::any &val)
{
  if (val.type() == typeid(bool))
  {
    auto x = std::any_cast<bool>(val);
    return x;
  }
  else if (val.type() == typeid(sjtu::int2048))
  {
    auto x = std::any_cast<sjtu::int2048>(val);
    return (x != 0);
  }
  else if (val.type() == typeid(double))
  {
    auto x = std::any_cast<double>(val);
    return std::abs(x) >= python_consts::kEPS;
  }
  else if (val.type() == typeid(std::string))
  {
    auto s = std::any_cast<std::string>(val);
    return s.empty();
  }
  else { assert(false); }
}
#endif //PYTHON_INTERPRETER_BUILTINFUNCS_H
