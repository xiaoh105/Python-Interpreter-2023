#pragma once
#ifndef PYTHON_INTERPRETER_VAROPERATOR_H
#define PYTHON_INTERPRETER_VAROPERATOR_H
#include <vector>
#include <any>
#include <cassert>
#include <int2048.h>
#include <Constant.h>
#include <BuiltinFuncs.h>

void ToRightVal(std::any &x);

std::any operator+(const std::any &);

std::any operator-(const std::any &);

std::any operator+(const std::any &, const std::any &);

std::any operator-(const std::any &, const std::any &);

std::any operator*(const std::any &, const std::any &);

std::any operator/(const std::any &, const std::any &);

std::any IDiv(const std::any &, const std::any &);

std::any operator%(const std::any &, const std::any &);

std::any &operator+=(std::any &, const std::any &);

std::any &operator-=(std::any &, const std::any &);

std::any &operator*=(std::any &, const std::any &);

std::any &operator/=(std::any &, const std::any &);

std::any &operator%=(std::any &, const std::any &);

bool operator<(const std::any &, const std::any &);

bool operator==(const std::any &, const std::any &);

bool operator>(const std::any &, const std::any &);

bool operator<=(const std::any &, const std::any &);

bool operator>=(const std::any &, const std::any &);

bool operator!=(const std::any &, const std::any &);

bool operator!(const std::any &);

std::any operator&(const std::any &, const std::any &);

std::any operator|(const std::any &, const std::any &);

#endif //PYTHON_INTERPRETER_VAROPERATOR_H
