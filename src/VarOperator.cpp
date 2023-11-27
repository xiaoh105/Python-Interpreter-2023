#include <vector>
#include <any>
#include <cassert>
#include <int2048.h>
#include <Constant.h>
#include <BuiltinFuncs.h>
#include <VarOperator.h>
#include <Utils.h>

void ToRightVal(std::any &x)
{
  if (GetVar(x))
  {
    auto tmp = std::any_cast<lval_type>(&x)->second;
    x = **std::any_cast<std::any*>(&tmp);
  }
  else if (GetTuple(x))
  {
    auto val = *std::any_cast<tuple_type>(&x);
    for (auto &i: val) ToRightVal(i);
    x = val;
  }
}

std::any operator+(const std::any &a)
{
  auto x = a;
  ToRightVal(x);
  if (GetStr(x) || GetTuple(x))
  {
    assert(false);
  }
  else if (GetBool(x))
  {
    auto val = *std::any_cast<bool>(&x);
    return val? sjtu::int2048(1):sjtu::int2048(0);
  }
  else
  {
    return x;
  }
}

std::any operator-(const std::any &a)
{
  auto x = a;
  ToRightVal(x);
  if (GetStr(x) || GetTuple(x))
  {
    assert(false);
  }
  else if (GetFloat(x))
  {
    auto val = *std::any_cast<double>(&x);
    return -val;
  }
  else if (GetInt(x))
  {
    auto val = *std::any_cast<sjtu::int2048>(&x);
    return -val;
  }
  else if (GetBool(x))
  {
    auto val = *std::any_cast<bool>(&x);
    return val? sjtu::int2048(-1):sjtu::int2048(0);
  }
  else
  {
    assert(false);
  }
}

std::any operator+(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    if (x.type() != y.type()) assert(false);
    auto s1 = *std::any_cast<std::string>(&x);
    auto s2 = *std::any_cast<std::string>(&y);
    return s1 += s2;
  }
  else
  {
    python_consts::ktype type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) + ToFloat(y);
    }
    else
    {
      return ToInt(x).add(ToInt(y));
    }
  }
}

std::any operator-(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    assert(false);
  }
  else
  {
    python_consts::ktype type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) - ToFloat(y);
    }
    else
    {
      return ToInt(x).minus(ToInt(y));
    }
  }
}

std::any operator*(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x))
  {
    if (GetStr(y)) assert(false);
    std::string ret, s;
    s = *std::any_cast<std::string>(&x);
    sjtu::int2048 val = ToInt(y);
    for (sjtu::int2048 i = 0; i < val; i += 1) ret += s;
    return ret;
  }
  else if (GetStr(y))
  {
    if (GetStr(x)) assert(false);
    std::string ret;
    auto s = *std::any_cast<std::string>(&y);
    sjtu::int2048 val = ToInt(x);
    for (sjtu::int2048 i = 0; i < val; i += 1) ret += s;
    return ret;
  }
  else
  {
    auto type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) * ToFloat(y);
    }
    else
    {
      return ToInt(x) * ToInt(y);
    }
  }
}

std::any operator/(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    assert(false);
  }
  else
  {
    return ToFloat(x) / ToFloat(y);
  }
}

std::any IDiv(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    assert(false);
  }
  else
  {
    auto ret =  ToInt(x) / ToInt(y);
    if (std::min(GetType(x), GetType(y)) == python_consts::FLOAT)
    {
      return ToFloat(ret);
    }
    else { return ret; }
  }
}

std::any operator%(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    assert(false);
  }
  else
  {
    return a - b * IDiv(a, b);
  }
}

std::any &operator+=(std::any &x, const std::any &y) { return x = x + y; }

std::any &operator-=(std::any &x, const std::any &y) { return x = x - y; }

std::any &operator*=(std::any &x, const std::any &y) { return x = x * y; }

std::any &operator/=(std::any &x, const std::any &y) { return x = x / y; }

std::any &operator%=(std::any &x, const std::any &y) { return x = x % y; }

bool operator<(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    if (GetType(x) != GetType(y)) assert(false);
    auto s1 = *std::any_cast<std::string>(&x);
    auto s2 = *std::any_cast<std::string>(&y);
    return s1 < s2;
  }
  else
  {
    python_consts::ktype type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) < ToFloat(y);
    }
    else
    {
      return ToInt(x) < ToInt(y);
    }
  }
}

bool operator==(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    if (GetType(x) != GetType(y)) return false;
    auto s1 = *std::any_cast<std::string>(&x);
    auto s2 = *std::any_cast<std::string>(&y);
    return s1 == s2;
  }
  else
  {
    python_consts::ktype type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (GetNone(x) || GetNone(y))
    {
      return type_x == type_y;
    }
    else if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) == ToFloat(y);
    }
    else
    {
      return ToInt(x) == ToInt(y);
    }
  }
}

bool operator>(const std::any &a, const std::any &b)
{
  auto x = a, y = b;
  ToRightVal(x), ToRightVal(y);
  if (GetStr(x) || GetStr(y))
  {
    if (GetType(x) != GetType(y)) assert(false);
    auto s1 = *std::any_cast<std::string>(&x);
    auto s2 = *std::any_cast<std::string>(&y);
    return s1 > s2;
  }
  else
  {
    python_consts::ktype type_x, type_y;
    type_x = GetType(x), type_y = GetType(y);
    if (std::min(type_x, type_y) == python_consts::FLOAT)
    {
      return ToFloat(x) > ToFloat(y);
    }
    else
    {
      return ToInt(x) > ToInt(y);
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

bool operator!(const std::any &x)
{
  return !ToBool(x);
}

std::any operator&(const std::any &x, const std::any &y)
{
  return ToBool(x)? y:x;
}

std::any operator|(const std::any &x, const std::any &y)
{
  return ToBool(x)? x:y;
}