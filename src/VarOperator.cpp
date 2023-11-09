#include <vector>
#include <any>
#include <cassert>
#include <int2048.h>
#include <Constant.h>
#include <BuiltinFuncs.h>
#include <VarOperator.h>
#include <Utils.h>

std::any operator+(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    if (x.type() != y.type()) assert(false);
    std::string s1, s2;
    s1 = std::any_cast<std::string>(x);
    s2 = std::any_cast<std::string>(y);
    return s1 + s2;
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) + ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) + ToInt(y);
    }
    else
    {
      return ToBool(x) + ToBool(y);
    }
  }
}

std::any operator-(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    assert(false);
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) - ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) - ToInt(y);
    }
    else
    {
      return ToBool(x) - ToBool(y);
    }
  }
}

std::any operator*(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR)
  {
    if (GetType(y) == python_consts::STR) assert(false);
    std::string ret, s;
    s = std::any_cast<std::string>(x);
    sjtu::int2048 val = ToInt(y);
    for (sjtu::int2048 i = 0; i < val; i += 1) ret += s;
    return ret;
  }
  else if (GetType(y) == python_consts::STR)
  {
    if (GetType(x) == python_consts::STR) assert(false);
    std::string ret, s;
    s = std::any_cast<std::string>(y);
    sjtu::int2048 val = ToInt(x);
    for (sjtu::int2048 i = 0; i < val; i += 1) ret += s;
    return ret;
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) * ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) * ToInt(y);
    }
    else
    {
      return ToBool(x) * ToBool(y);
    }
  }
}

std::any operator/(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    assert(false);
  }
  else
  {
    return ToFloat(x) / ToFloat(y);
  }
}

std::any IDiv(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    assert(false);
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      assert(false);
    }
    else
    {
      return ToInt(x) / ToInt(y);
    }
  }
}

std::any operator%(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    assert(false);
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      assert(false);
    }
    else
    {
      return ToInt(x) % ToInt(y);
    }
  }
}

std::any &operator+=(std::any &x, const std::any &y)
{
  return x = x + y;
}

std::any &operator-=(std::any &x, const std::any &y)
{
  return x = x - y;
}

std::any &operator*=(std::any &x, const std::any &y)
{
  return x = x * y;
}

std::any &operator/=(std::any &x, const std::any &y)
{
  return x = x / y;
}

std::any &operator%=(std::any &x, const std::any &y)
{
  return x = x % y;
}

bool operator<(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    if (GetType(x) != GetType(y)) assert(false);
    std::string s1, s2;
    s1 = std::any_cast<std::string>(x);
    s2 = std::any_cast<std::string>(y);
    return s1 < s2;
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) < ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) < ToInt(y);
    }
    else
    {
      return ToBool(x) < ToBool(y);
    }
  }
}

bool operator==(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    if (GetType(x) != GetType(y)) assert(false);
    std::string s1, s2;
    s1 = std::any_cast<std::string>(x);
    s2 = std::any_cast<std::string>(y);
    return s1 == s2;
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) == ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) == ToInt(y);
    }
    else
    {
      return ToBool(x) == ToBool(y);
    }
  }
}

bool operator>(const std::any &x, const std::any &y)
{
  if (GetType(x) == python_consts::STR || GetType(y) == python_consts::STR)
  {
    if (GetType(x) != GetType(y)) assert(false);
    std::string s1, s2;
    s1 = std::any_cast<std::string>(x);
    s2 = std::any_cast<std::string>(y);
    return s1 > s2;
  }
  else
  {
    python_consts::kTYPE type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) > ToFloat(y);
    }
    else if (std::min(type_x, type_y) == python_consts::INT)
    {
      return ToInt(x) > ToInt(y);
    }
    else
    {
      return ToBool(x) > ToBool(y);
    }
  }
}

bool operator<=(const std::any &x, const std::any &y)
{
  return !(x > y);
}

bool operator>=(const std::any &x, const std::any &y)
{
  return !(x < y);
}

bool operator!=(const std::any &x, const std::any &y)
{
  return !(x == y);
}

std::any GetIndex(const std::any &x, int id)
{
  if (GetType(x) != python_consts::TUPLE) assert(false);
  auto val = std::any_cast<std::vector<std::any>>(x);
  if (id >= val.size()) assert(false);
  return val[id];
}