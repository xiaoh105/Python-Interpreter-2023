#pragma once
#ifndef PYTHON_INTERPRETER_BUILTINFUNCS_H
#define PYTHON_INTERPRETER_BUILTINFUNCS_H
#include <cstring>
#include <cassert>
#include <any>
#include <int2048.h>
#include <Utils.h>
#include <Constant.h>

sjtu::int2048 ToInt(const std::any &val);

double ToFloat(const std::any &val);

std::string ToString(const std::any &val);

bool ToBool(const std::any &val);

void PrintAtom(const std::any &);

void Print(const std::vector<std::pair<std::string, std::any>> &);

bool IsBuiltin(const std::string &name);

#endif //PYTHON_INTERPRETER_BUILTINFUNCS_H
