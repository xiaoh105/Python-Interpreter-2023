#pragma once
#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H

#include <unordered_map>
#include <string>
#include <any>
#include <set>
#include <vector>
#include "Constant.h"
#include "antlr4-runtime.h"
#include "Python3Parser.h"

class AtomVarScope
{
 private:
  std::unordered_map<std::string, int> var_id;
  std::vector<std::any> var;

 public:
  AtomVarScope() = default;
  explicit AtomVarScope(const std::vector<std::pair<std::string, std::any>> &);
  ~AtomVarScope() = default;
  void RegisterVar(const std::string &, const std::any &);
  std::pair<bool, std::any> QueryVar(const std::string &);
};

class Scope
{
 private:
  friend class AtomFuncScope;
  AtomVarScope global;
  std::stack<AtomVarScope *> func_scope;

 public:
  Scope();
  ~Scope() = default;
  /// Register a variable scope
  static AtomVarScope *RegisterScope();
  void DestroyScope();
  /// Register a variable in a certain scope, -1 for global variable
  void RegisterVar(const std::string &, const std::any &);
  /// Query a variable in a certain scope, -1 for global variable
  std::pair<bool, std::any> QueryVar(const std::string &);
};

class AtomFuncScope
{
 private:
  Python3Parser::SuiteContext *body;
  std::vector<std::pair<std::string, std::any>> var;
  void GetArglist(Python3Parser::ArglistContext *ctx,
                  AtomVarScope &);

 public:
  AtomFuncScope(Python3Parser::SuiteContext *ctx,
                const std::vector<std::pair<std::string, std::any>> &val):
            var(val), body(ctx){};
  ~AtomFuncScope() = default;
  /// Initialize arglist and call a function
  std::any CallFunc(Python3Parser::ArglistContext *);
};

class FuncScope
{
 private:
  int num_of_func = 0;
  AtomFuncScope *func_scope[python_consts::kMAX_FUNC];
  std::unordered_map<std::string, int> func_id;
  static std::any CallBuiltinFunc
          (const std::string &, Python3Parser::ArglistContext *);

 public:
  FuncScope() = default;
  ~FuncScope() = default;
  void DefFunc(const std::string &,
               const std::vector<std::pair<std::string, std::any>> &,
               Python3Parser::SuiteContext *);

  std::any CallFunc(const std::string &,
                    Python3Parser::ArglistContext *);
};

extern Scope var_scope;
extern FuncScope func_scope;

#endif //PYTHON_INTERPRETER_SCOPE_H
