#pragma once
#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H
#include <string>
#include <int2048.h>
#include <Evalvisitor.h>
#include <Constant.h>
bool IsNumber(const std::string &val);

bool IsFloat(const std::string &val);

python_consts::ktype GetType(const std::any &x);

bool GetFloat(const std::any &x);

bool GetInt(const std::any &x);

bool GetBool(const std::any &x);

bool GetStr(const std::any &x);

bool GetTuple(const std::any &x);

bool GetNone(const std::any &x);

bool GetVar(const std::any &x);

bool GetFlow(const std::any &x);

python_consts::kflow_info GetFlowInfo(const std::any &x);

std::any* GetVarAddr(const std::any &x);

void DelQuotationMark(std::string &s);

#endif //PYTHON_INTERPRETER_UTILS_H
