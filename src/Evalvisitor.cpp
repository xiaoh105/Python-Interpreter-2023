#include "Evalvisitor.h"
#include "BuiltinFuncs.h"
#include "VarOperator.h"

Scope var_scope;
FuncScope func_scope;

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx)
{
  if (ctx->NAME())
  {
    return (std::string)(ctx->NAME()->getText());
  }
  else if (ctx->NUMBER())
  {
    std::string s = ctx->NUMBER()->getText();
    if (IsFloat(s)) { return ToFloat(s); }
    else { return ToInt(s); }
  }
  else if (ctx->NONE()) { return std::any(); }
  else if (ctx->TRUE()) { return true; }
  else if (ctx->FALSE()) { return false; }
  else if (!ctx->STRING().empty())
  {
    std::string ret;
    for (const auto &i: ctx->STRING()) ret += i->getText();
    return ret;
  }
  else if (ctx->test()) { return visit(ctx->test()); }
  else return nullptr;
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx)
{
  if (!ctx->trailer())
  {
    if (ctx->atom()->NAME())
    {
      return var_scope.QueryVar(ctx->atom()->getText());
    }
    else return visitAtom(ctx->atom());
  }
  else
  {
    return func_scope.CallFunc(ctx->atom()->NAME()->getText(),
                               ctx->trailer()->arglist());
  }
}

std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx)
{
  EvalVisitor visitor;
  auto ret = visitor.visitTypedargslist(ctx->parameters()->typedargslist());
  auto init_args =
          std::any_cast<std::vector<std::pair<std::string, std::any>>>(ret);
  func_scope.DefFunc(ctx->NAME()->getText(), init_args, ctx->suite());
  return std::any();
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx)
{
  if (ctx->ASSIGN())
  {
    auto tmp = visitTest(ctx->test()[1]);
    if (GetVar(tmp))
    {
      tmp = *std::any_cast<std::any*>
              (std::any_cast<std::pair<bool, std::any>>(tmp).second);
    }
    return tmp;
  }
  else
  {
    auto tmp = visitTest(ctx->test()[0]);
    if (GetVar(tmp))
    {
      tmp = *std::any_cast<std::any*>
              (std::any_cast<std::pair<bool, std::any>>(tmp).second);
    }
    return tmp;
  }
}

std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx)
{
  std::vector<std::pair<std::string, std::any>> ret;
  int undefined_paras = ctx->tfpdef().size() - ctx->test().size();
  for (int i = 0; i < ctx->tfpdef().size(); ++i)
  {
    std::string name = ctx->tfpdef()[i]->NAME()->getText();
    std::any default_val;
    EvalVisitor visitor;
    if (i >= undefined_paras)
      default_val = visitor.visitTest(ctx->test()[i - undefined_paras]);
    ret.emplace_back(name, default_val);
  }
  return ret;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx)
{
  std::any ret;
  if (ctx->factor()) { ret = visitFactor(ctx->factor()); }
  else { ret = visitAtom_expr(ctx->atom_expr()); }
  if (ctx->MINUS()) { return -ret; }
  else { return ret; }
}

std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx)
{
  if (ctx->STAR()) { return python_consts::MUL; }
  else if (ctx->DIV()) { return python_consts::DIV; }
  else if (ctx->IDIV()) { return python_consts::IDIV; }
  else if (ctx->MOD()) { return python_consts::MOD; }
  else { assert(false); }
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx)
{
  std::any ret = visitFactor(ctx->factor()[0]);
  for (int i = 1; i < ctx->factor().size(); ++i)
  {
    auto tmp = visitFactor(ctx->factor()[i]);
    int op = std::any_cast<python_consts::kterm_op>
            (visitMuldivmod_op(ctx->muldivmod_op()[i - 1]));
    switch (op)
    {
      case python_consts::MUL: ret = ret * tmp; break;
      case python_consts::DIV: ret = ret / tmp; break;
      case python_consts::IDIV: ret = IDiv(ret, tmp); break;
      case python_consts::MOD: ret = ret % tmp; break;
      default: assert(false);
    }
  }
  return ret;
}

std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx)
{
  if (ctx->ADD()) { return python_consts::ADD; }
  else if (ctx->MINUS()) { return python_consts::MINUS; }
  else { assert(false); }
}

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx)
{
  std::any ret = visitTerm(ctx->term()[0]);
  for (int i = 1; i < ctx->term().size(); ++i)
  {
    auto tmp = visitTerm(ctx->term()[i]);
    int op = std::any_cast<python_consts::karith_op>
            (visitAddorsub_op(ctx->addorsub_op()[i - 1]));
    switch (op)
    {
      case python_consts::ADD: ret = ret + tmp; break;
      case python_consts::MINUS: ret = ret - tmp; break;
      default: assert(false);
    }
  }
  return ret;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx)
{
  if (ctx->NOT()) { return !visitNot_test(ctx->not_test()); }
  else { return visitComparison(ctx->comparison()); }
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx)
{
  if (!ctx->COMMA().empty())
  {
    std::vector<std::any> ret;
    for (const auto &i: ctx->test()) ret.push_back(visitTest(i));
    return ret;
  }
  else
  {
    return visitTest(ctx->test()[0]);
  }
}

std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx)
{
  if (ctx->LESS_THAN()) { return python_consts::LE; }
  else if (ctx->GREATER_THAN()) { return python_consts::GE; }
  else if (ctx->LT_EQ()) { return python_consts::LEQ; }
  else if (ctx->GT_EQ()) { return python_consts::GEQ; }
  else if (ctx->EQUALS()) { return python_consts::EQ; }
  else if (ctx->NOT_EQ_2()) { return python_consts::NEQ; }
  else { assert(false); }
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx)
{
  auto cur = visitNot_test(ctx->not_test()[0]);
  if (ctx->not_test().size() == 1) return cur;
  if (!ToBool(cur)) return cur;
  for (int i = 1; i < ctx->not_test().size(); ++i)
  {
    cur = cur & visitNot_test(ctx->not_test()[i]);
    if (!ToBool(cur)) return cur;
  }
  return cur;
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx)
{
  auto cur = visitAnd_test(ctx->and_test()[0]);
  if (ctx->and_test().size() == 1) return cur;
  if (ToBool(cur)) return cur;
  for (int i = 1; i < ctx->and_test().size(); ++i)
  {
    cur = cur | visitAnd_test(ctx->and_test()[i]);
    if (ToBool(cur)) return cur;
  }
  return cur;
}

std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx)
{
  if (ctx->ADD_ASSIGN()) { return python_consts::ADDASSIGN; }
  else if (ctx->SUB_ASSIGN()) { return python_consts::SUBASSIGN; }
  else if (ctx->MULT_ASSIGN()) { return python_consts::MULASSIGN; }
  else if (ctx->DIV_ASSIGN()) { return python_consts::DIVASSIGN; }
  else if (ctx->IDIV_ASSIGN()) { return python_consts::IDIVASSIGN; }
  else if (ctx->MOD_ASSIGN()) { return python_consts::MODASSIGN; }
  else { assert(false); }
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx)
{
  if (ctx->augassign())
  {
    auto val = visitAugassign(ctx->augassign());
    auto op = std::any_cast<python_consts::kaug_assign>(val);
    auto x = std::any_cast<std::pair<bool, std::any>>
                           (visitTestlist(ctx->testlist()[0])).second;
    auto var = std::any_cast<std::any*>(x);
    auto y = visitTestlist(ctx->testlist()[1]);
    ToRightVal(y);
    switch (op)
    {
      case python_consts::ADDASSIGN: return *var += y;
      case python_consts::SUBASSIGN: return *var -= y;
      case python_consts::MULASSIGN: return *var *= y;
      case python_consts::DIVASSIGN: return *var /= y;
      case python_consts::IDIVASSIGN: return *var = IDiv(*var, y);
      case python_consts::MODASSIGN: return *var %= y;
      default: assert(false);
    }
  }
  else
  {
    int len = ctx->testlist().size();
    auto val = visitTestlist(ctx->testlist()[len - 1]);
    ToRightVal(val);
    for (int i = len - 2; i >= 0; --i)
    {
      auto tmp = visitTestlist(ctx->testlist()[i]);
      if (!GetVar(tmp) && !GetTuple(tmp)) assert(false);
      if (GetTuple(tmp) && GetTuple(val))
      {
        auto v1 = std::any_cast<std::vector<std::any>>(tmp);
        auto v2 = std::any_cast<std::vector<std::any>>(val);
        if (v1.size() != v2.size()) assert(false);
        for (int j = 0; j < v1.size(); ++j)
        {
          auto var = std::any_cast<std::pair<bool, std::any>>(v1[j]);
          if (!var.first)
          {
            auto name = std::any_cast<std::string>(var.second);
            var_scope.RegisterVar(name, v2[j]);
          }
          else { *GetVarAddr(v1[j]) = v2[j]; }
        }
      }
      else
      {
        auto var = std::any_cast<std::pair<bool, std::any>>(tmp);
        if (!var.first) {
          auto name = std::any_cast<std::string>(var.second);
          var_scope.RegisterVar(name, val);
        } else { *GetVarAddr(tmp) = val; }
      }
    }
    return val;
  }
}

