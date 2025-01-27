#include <cassert>
#include <any>
#include <iomanip>
#include <int2048.h>
#include <Utils.h>
#include <Constant.h>
#include <VarOperator.h>
#include <BuiltinFuncs.h>

sjtu::int2048 ToInt(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
  if (GetInt(val))
  {
    auto x = *std::any_cast<sjtu::int2048>(&val);
    return x;
  }
  else if (GetStr(val))
  {
    auto s = *std::any_cast<std::string>(&val);
    if (!IsNumber(s)) assert(false);
    if (s.find('.') == std::string::npos) { return sjtu::int2048(s); }
    else { return sjtu::int2048(s.substr(0, s.find('.'))); }
  }
  else if (GetBool(val))
  {
    auto x = *std::any_cast<bool>(&val);
    return x? 1:0;
  }
  else if (GetFloat(val))
  {
    auto x = *std::any_cast<double>(&val);
    return (long long) x;
  }
  else { assert(false); }
}

double ToFloat(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
  if (GetFloat(val))
  {
    auto x = *std::any_cast<double>(&val);
    return x;
  }
  else if (GetInt(val))
  {
    auto x = *std::any_cast<sjtu::int2048>(&val);
    return x.ToDouble();
  }
  else if (GetBool(val))
  {
    auto x = *std::any_cast<bool>(&val);
    return x? (double)1.0:(double)0.0;
  }
  else if (GetStr(val))
  {
    auto s = *std::any_cast<std::string>(&val);
    if (!IsNumber(s)) assert(false);
    return std::stod(s);
  }
  else { assert(false); }
}

std::string ToString(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
  std::ostringstream oss;
  if (GetStr(val))
  {
    auto s = *std::any_cast<std::string>(&val);
    return s;
  }
  else if (GetInt(val))
  {
    auto x = *std::any_cast<sjtu::int2048>(&val);
    oss << x;
    return oss.str();
  }
  else if (GetBool(val))
  {
    auto x = *std::any_cast<bool>(&val);
    return x? "True":"False";
  }
  else if (GetFloat(val))
  {
    auto x = *std::any_cast<double>(&val);
    oss << std::fixed << std::setprecision(6) << x;
    return oss.str();
  }
  else if (GetNone(val))
  {
    return "None";
  }
  else { assert(false); }
}

bool ToBool(const std::any &x)
{
  auto val = x;
  ToRightVal(val);
  if (GetBool(val))
  {
    auto x = *std::any_cast<bool>(&val);
    return x;
  }
  else if (GetInt(val))
  {
    auto x = *std::any_cast<sjtu::int2048>(&val);
    return (x != 0);
  }
  else if (GetFloat(val))
  {
    auto x = *std::any_cast<double>(&val);
    return std::abs(x) >= python_consts::kEPS;
  }
  else if (GetStr(val))
  {
    auto s = *std::any_cast<std::string>(&val);
    return !s.empty();
  }
  else if (GetNone(val))
  {
    return false;
  }
  else { assert(false); }
}

void PrintAtom(const std::any &val)
{
  if (GetFloat(val))
  {
    auto x = *std::any_cast<double>(&val);
    printf("%.6f", x);
  }
  else if (GetInt(val))
  {
    auto x = *std::any_cast<sjtu::int2048>(&val);
    x.print();
  }
  else if (GetBool(val))
  {
    auto x = *std::any_cast<bool>(&val);
    printf(x? "True":"False");
  }
  else if (GetStr(val))
  {
    auto s = *std::any_cast<std::string>(&val);
    printf("%s", s.c_str());
  }
  else if (GetNone(val)) { printf("None"); }
  else if (GetTuple(val))
  {
    auto x = *std::any_cast<tuple_type>(&val);
    for (int i = 0; i < x.size(); ++i)
    {
      PrintAtom(x[i]);
      if (i != x.size() - 1) printf(" ");
    }
  }
  else { assert(false); }
}

void Print(const arglist_type &val)
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