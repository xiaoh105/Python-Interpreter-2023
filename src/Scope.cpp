#include <cassert>
#include "BuiltinFuncs.h"
#include "Scope.h"
#include "Evalvisitor.h"
#include <VarOperator.h>

#define DEBUG

AtomVarScope::AtomVarScope(const std::vector<std::pair<std::string, std::any>> &val)
{
  var.clear();
  for (const auto &i: val) RegisterVar(i.first, i.second);
}

void AtomVarScope::RegisterVar(const std::string &name, const std::any &val)
{
  if (var_id.find(name) != var_id.end()) assert(false);
  var_id[name] = var.size();
  var.push_back(val);
}

std::pair<bool, std::any> AtomVarScope::QueryVar(const std::string &name)
{
  if (var_id.find(name) == var_id.end())
  {
    return std::make_pair(false, std::any(name));
  }
  else
  {
    return std::make_pair(true, &var[var_id[name]]);
  }
}

Scope::Scope(): global()
{
  func_scope.push(&global);
}

void Scope::RegisterScope()
{
  func_scope.push(new AtomVarScope);
}

void Scope::DestroyScope()
{
  delete func_scope.top();
  func_scope.pop();
}

void Scope::RegisterVar(const std::string &name, const std::any &val)
{
  func_scope.top()->RegisterVar(name, val);
}

std::pair<bool, std::any> Scope::QueryVar(const std::string &name)
{
  auto ret = func_scope.top()->QueryVar(name);
  if (ret.first) { return ret; }
  else { return global.QueryVar(name); }
}

std::any AtomFuncScope::CallFunc
(Python3Parser::ArglistContext *arglist)
{
  var_scope.RegisterScope();
  EvalVisitor visitor;
  for (const auto &i: var) var_scope.RegisterVar(i.first, i.second);
  GetArglist(arglist);
  for (const auto &i: var)
    if (!var_scope.QueryVar(i.first).first)
    {
      if (!i.second.has_value()) assert(false);
      var_scope.RegisterVar(i.first, i.second);
    }
  std::any ret = visitor.visit(body);
  var_scope.DestroyScope();
  return ret;
}

void AtomFuncScope::GetArglist(Python3Parser::ArglistContext *ctx)
{
  if (ctx == nullptr) return;
  int num = -1;
  for (const auto &i:ctx->argument())
  {
    ++num;
    EvalVisitor visitor;
    if (!i->ASSIGN())
    {
      auto tmp = visitor.visitTest(i->test()[0]);
      ToRightVal(tmp);
      auto tmp_var = var_scope.QueryVar(var[num].first).second;
      auto cur_var  = std::any_cast<std::any*>(tmp_var);
      *cur_var = tmp;
    }
    else
    {
      auto tmp0 = visitor.visitTest(i->test()[0]);
      auto tmp1 = visitor.visitTest(i->test()[1]);
      if (GetVar(tmp1))
      {
        tmp1 = *std::any_cast<std::any*>(tmp1);
      }
      if (!GetVar(tmp0)) assert(false);
      auto cur_var = std::any_cast<std::pair<bool, std::any>>(tmp0).second;
      *std::any_cast<std::any*>(cur_var) = tmp1;
    }
  }
}

std::any FuncScope::CallBuiltinFunc
        (const std::string &name,
         Python3Parser::ArglistContext *arglist)
{
  EvalVisitor visitor;
  std::vector<std::pair<std::string, std::any>> args;
  for (const auto &i: arglist->argument())
    args.emplace_back("", visitor.visitArgument(i));
  if (name == "print")
  {
    Print(args);
    return std::any();
  }
  else if (name == "int")
  {
    return ToInt(args[0].second);
  }
  else if (name == "float")
  {
    return ToFloat(args[0].second);
  }
  else if (name == "str")
  {
    return ToString(args[0].second);
  }
  else if (name == "bool")
  {
    return ToBool(args[0].second);
  }
  else { assert(false); }
}

void FuncScope::DefFunc
(const std::string &name,
 const std::vector<std::pair<std::string, std::any>> &def_arglist,
 Python3Parser::SuiteContext *ctx)
{
  func_id[name] = ++num_of_func;
  func_scope[num_of_func] = new AtomFuncScope(ctx, def_arglist);
}

std::any FuncScope::CallFunc
(const std::string &name,
 Python3Parser::ArglistContext *arglist)
{
  if (IsBuiltin(name)) return CallBuiltinFunc(name, arglist);
  if (!func_id[name]) assert(false);
  return func_scope[func_id[name]]->CallFunc(arglist);
}
