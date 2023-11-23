#include <cassert>
#include <any>
#include <int2048.h>
#include <Utils.h>
#include <Constant.h>
#include <VarOperator.h>
#include <BuiltinFuncs.h>

sjtu::int2048 ToInt(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
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
    else { return sjtu::int2048(s.substr(0, s.find('.'))); }
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
  else
  {
    std::cerr << "Current data can't be converted to int." << std::endl;
    exit(301);
  }
}

double ToFloat(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
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
    return x? (double)1.0:(double)0.0;
  }
  else if (val.type() == typeid(std::string))
  {
    auto s = std::any_cast<std::string>(val);
    if (!IsNumber(s)) assert(false);
    return std::stod(s);
  }
  else
  {
    std::cerr << "Current data can't be converted to float." << std::endl;
    exit(302);
  }
}

std::string ToString(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
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
  else
  {
    std::cerr << "Current data can't be converted to string." << std::endl;
    exit(303);
  }
}

bool ToBool(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
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
  else
  {
    {
      std::cerr << "Current data can't be converted to bool." << std::endl;
      exit(304);
    }
  }
}

void PrintAtom(const std::any &val)
{
  if (GetFloat(val))
  {
    auto x = std::any_cast<double>(val);
    printf("%.6f", x);
  }
  else if (GetInt(val))
  {
    auto x = std::any_cast<sjtu::int2048>(val);
    x.print();
  }
  else if (GetBool(val))
  {
    auto x = std::any_cast<bool>(val);
    printf(x? "True":"False");
  }
  else if (GetStr(val))
  {
    auto s = std::any_cast<std::string>(val);
    printf("%s", s.c_str());
  }
  else if (GetNone(val)) { printf("None"); }
  else if (GetTuple(val))
  {
    std::cerr << "Do not support print() for tuples!" << std::endl;
    exit(305);
    // TODO: delete this/remove assert
    auto x = std::any_cast<std::vector<std::any>>(val);
    for (int i = 0; i < x.size(); ++i)
    {
      PrintAtom(x[i]);
      if (i != x.size() - 1) printf(" ");
    }
  }
  else
  {
    std::cerr << "Current type doesn't support print()" << std::endl;
    exit(306);
  }
}

void Print(const std::vector<std::pair<std::string, std::any>> &val)
{
  for (const auto &i: val)
  {
    PrintAtom(i.second);
    printf(" ");
  }
  printf("\n");
}

bool IsBuiltin(const std::string &name)
{
  if (name == "print") return true;
  if (name == "int") return true;
  if (name == "float") return true;
  if (name == "str") return true;
  if (name == "bool") return true;
  return false;
}