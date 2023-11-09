#include <cassert>
#include "Scope.h"

VarScope::VarScope(const std::vector<std::pair<std::string, std::any>> &val)
{
  var.clear();
  for (const auto &i: val) RegisterVar(i.first, i.second);
}

void VarScope::RegisterVar(const std::string &name,const std::any &val)

{
  if (var_id.find(name) != var_id.end()) assert(false);
  var_id[name] = var.size();
  var.push_back(val);
}

std::pair<bool, std::any *> VarScope::QueryVar(const std::string &name)
{
  if (var_id.find(name) == var_id.end())
  {
    return std::make_pair(false, nullptr);
  }
  else
  {
    return std::make_pair(true, &var[var_id[name]]);
  }
}

Scope::Scope(): global(), scope_id(), func_scope()
{
  for (int i = 1; i <= python_consts::kMAX_FUNC; ++i) status.insert(i);
}

void Scope::RegisterScope
(int id, const std::vector<std::pair<std::string, std::any>> &val)
{
  if (status.empty()) assert(false);
  int pos = *status.begin();
  status.erase(status.begin());
  func_scope[pos] = new VarScope(val);
  scope_id[id] = pos;
}

void Scope::DestroyScope(int id)
{
  int pos = scope_id[id];
  status.insert(pos);
  delete func_scope[pos];
  func_scope[pos] = nullptr;
  scope_id.erase(id);
}

void Scope::RegisterVar(int id, const std::string &name, const std::any &val)
{
  if (id == -1)
  {
    global.RegisterVar(name, val);
  }
  else
  {
    int pos = scope_id[id];
    func_scope[pos]->RegisterVar(name, val);
  }
}

std::pair<bool, std::any *> Scope::QueryVar(int id, const std::string &name)
{
  if (id == -1)
  {
    return global.QueryVar(name);
  }
  else
  {
    int pos = scope_id[id];
    auto ret = func_scope[pos]->QueryVar(name);
    if (ret.first) { return ret; }
    else { return global.QueryVar(name); }
  }
}