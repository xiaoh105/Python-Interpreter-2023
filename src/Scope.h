#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H

#include <unordered_map>
#include <string>
#include <any>
#include <set>
#include <vector>
#include "Constant.h"

class VarScope
{
 private:
  std::unordered_map<std::string, int> var_id;
  std::vector<std::any> var;

 public:
  VarScope() = default;
  VarScope(const std::vector<std::pair<std::string, std::any>> &);
  ~VarScope() = default;
  void RegisterVar(const std::string &,const std::any &);
  std::pair<bool, std::any *> QueryVar(const std::string &);
};

class Scope
{
 private:
  VarScope global;
  VarScope *func_scope[python_consts::kMAX_FUNC + 10];
  std::set<int> status;
  std::unordered_map<int, int> scope_id;

 public:
  Scope();
  ~Scope() = default;
  /// Register a variable scope, id must be a unique number representing current scope
  void RegisterScope(int,
                     const std::vector<std::pair<std::string, std::any>> &);
  void DestroyScope(int);
  /// Register a variable in a certain scope, -1 for global variable
  void RegisterVar(int, const std::string &, const std::any &);
  /// Query a variable in a certain scope, -1 for global variable
  std::pair<bool, std::any *> QueryVar(int, const std::string &);
};

#endif //PYTHON_INTERPRETER_SCOPE_H
