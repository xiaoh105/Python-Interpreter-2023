#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

namespace sjtu
{
  using std::vector;
  class int2048;
  class polynomial
  {
  private:
    // 使用NTT计算乘法, root = 6, mod = 7 * (2 ^ 50) + 1
    constexpr static long long root = 6;
    constexpr static long long inv = 1313549891316395;
    constexpr static long long mod = 7881299347898369;
    int len;
    vector<long long> a;
    /// NTT 蝶形变换, O(n)
    void ChangeIndex();
    /// 扩展多项式长度, 自动添加前缀0
    void ExtendLen(int);
    /// 对当前多项式进行快速数论变换
    void NTT(int);
    /// 对多项式进行进位处理
    void CalcCarry();
    /// 带模数的快速幂
    friend long long pow_mod(long long, long long);
    /// 扩展欧几里得算法，用于计算模数
    friend void Extend_GCD(long long, long long, long long &, long long &);
    /// 求取模意义下的乘法逆元
    friend long long inverse(long long);

  public:
    friend class int2048;
    /// 默认构造函数, 默认构造f(x) = 0
    polynomial();
    /// 复制构造函数
    polynomial(const polynomial &) = default;
    /// 移动构造函数
    polynomial(polynomial &&) = default;
    /// 利用大整数生成多项式，即将x转换为f(x) = a0 + a1 * x + ...
    explicit polynomial(const int2048 &);
    /// 析构函数
    ~polynomial() = default;
    /// 复制赋值运算
    polynomial &operator=(const polynomial &) = default;
    /// 移动赋值运算
    polynomial &operator=(polynomial &&) = default;
    /// 进行多项式乘法
    polynomial &Multiply(polynomial);
    /// 利用多项式生成大整数
    int2048 ToInteger();
  };
  class int2048
  {
  private:
    const static int base = 10000; // 压位的数字
    const static int base_log10 = 4; // 压位的位数
    int len; // 数字长度，不包含前缀0
    vector<int> a; // 储存数据，0-based, 低位存在0
    int get(int pos) const
    {
      return pos >= a.size() || pos < 0? 0:a[pos];
    }
    /// 无符号加法
    friend int2048 UnsignedAdd(const int2048 &, const int2048 &);
    /// 无符号减法
    friend int2048 UnsignedMinus(const int2048 &, const int2048 &);
    int2048 &UnsignedMul(const int2048 &);
    /// 在牛顿迭代法之后进行误差调整
    friend void Adjust(const int2048 &, const int2048&, int2048 &, long long);
    /// 运用牛顿迭代法求逆，即求[2^n / x]
    friend int2048 GetInv(const int2048 &, int);
    /// 无符号除法
    int2048 &UnsignedDivide(const int2048 &);
    int2048 &UnsignedShortDivide(const int2048 &, int, int);
    /// 将当前整数*(base^x)（左移一个block)
    int2048 &operator<<=(int);
    /// 将当前整数/(base^x)（右移一个block)
    int2048 &operator>>=(int);
    /// 左移操作，定义同上
    friend int2048 operator<<(int2048, int val);
    /// 右移操作，定义同上
    friend int2048 operator>>(int2048, int val);

  public:
    friend class polynomial;
    int sgn;
    /// 默认构造函数
    int2048();
    /// 基于long long的构造函数
    int2048(long long);
    /// 基于字符串的构造函数
    explicit int2048(const std::string &);
    /// 复制构造函数
    int2048(const int2048 &) = default;
    /// 移动构造函数
    int2048(int2048 &&) = default;
    /// 析构函数
    ~int2048() = default;

    /// 读入一个大整数
    void read(const std::string &);
    /// 输出储存的大整数
    void print() const;
    double ToDouble() const;

    /// 返回当前数的绝对值
    friend int2048 abs(const int2048 &);

    /// 加上一个大整数
    int2048 &add(const int2048 &);
    /// 返回两个大整数之和
    friend int2048 add(int2048, const int2048 &);

    /// 减去一个大整数
    int2048 &minus(const int2048 &);
    /// 返回两个大整数之差
    friend int2048 minus(int2048, const int2048 &);

    /// 一元的+运算符，即+x
    int2048 operator+() const;
    /// 一元的-运算符，即-x
    int2048 operator-() const;

    /// 复制赋值运算
    int2048 &operator=(const int2048 &) = default;
    /// 移动赋值运算
    int2048 &operator=(int2048 &&) noexcept = default;

    int2048 &operator+=(const int2048 &);
    friend int2048 operator+(int2048, const int2048 &);

    int2048 &operator-=(const int2048 &);
    friend int2048 operator-(int2048, const int2048 &);

    int2048 &operator*=(const int2048 &);
    friend int2048 operator*(int2048, const int2048 &);
    friend int2048 operator*(int2048, long long);

    int2048 &operator/=(const int2048 &);
    friend int2048 operator/(int2048, const int2048 &);

    int2048 &operator%=(const int2048 &);
    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);
    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);
    friend bool operator!=(const int2048 &, const int2048 &);
    friend bool operator<(const int2048 &, const int2048 &);
    friend bool operator>(const int2048 &, const int2048 &);
    friend bool operator<=(const int2048 &, const int2048 &);
    friend bool operator>=(const int2048 &, const int2048 &);
  };
} // namespace sjtu

#endif