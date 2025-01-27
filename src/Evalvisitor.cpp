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
    for (const auto &i: ctx->STRING())
    {
      auto tmp = i->getText();
      DelQuotationMark(tmp);
      ret += tmp;
    }
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
  if (ctx->parameters()->typedargslist())
  {
    auto ret = visitor.visitTypedargslist(ctx->parameters()->typedargslist());
    auto init_args = *std::any_cast<arglist_type>(&ret);
    func_scope.DefFunc(ctx->NAME()->getText(), init_args, ctx->suite());
  }
  else
  {
    auto init_args = arglist_type();
    func_scope.DefFunc(ctx->NAME()->getText(), init_args, ctx->suite());
  }
  return std::any();
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx)
{
  auto test = ctx->test();
  if (ctx->ASSIGN())
  {
    auto tmp = visitTest(test[1]);
    ToRightVal(tmp);
    return tmp;
  }
  else
  {
    auto tmp = visitTest(test[0]);
    ToRightVal(tmp);
    return tmp;
  }
}

std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx)
{
  arglist_type ret;
  auto tfpdef = ctx->tfpdef();
  auto test = ctx->test();
  int undefined_paras = tfpdef.size() - test.size();
  for (int i = 0; i < tfpdef.size(); ++i)
  {
    std::string name = tfpdef[i]->NAME()->getText();
    std::any default_val;
    EvalVisitor visitor;
    if (i >= undefined_paras)
      default_val = visitor.visitTest(test[i - undefined_paras]);
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
  auto factor = ctx->factor();
  auto muldivmod_op = ctx->muldivmod_op();
  std::any ret = visitFactor(factor[0]);
  for (int i = 1; i < factor.size(); ++i)
  {
    auto tmp = visitFactor(factor[i]);
    int op = std::any_cast<python_consts::kterm_op>
            (visitMuldivmod_op(muldivmod_op[i - 1]));
    switch (op)
    {
      case python_consts::MUL: ret *= tmp; break;
      case python_consts::DIV: ret /= tmp; break;
      case python_consts::IDIV: ret = IDiv(ret, tmp); break;
      case python_consts::MOD: ret %= tmp; break;
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
  auto term = ctx->term();
  auto addorsub_op = ctx->addorsub_op();
  std::any ret = visitTerm(term[0]);
  for (int i = 1; i < term.size(); ++i)
  {
    auto tmp = visitTerm(term[i]);
    int op = std::any_cast<python_consts::karith_op>
            (visitAddorsub_op(addorsub_op[i - 1]));
    switch (op)
    {
      case python_consts::ADD: ret += tmp; break;
      case python_consts::MINUS: ret -= tmp; break;
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
  if (ctx->test().size() > 1)
  {
    std::vector<std::any> ret;
    for (const auto &i: ctx->test())
    {
      auto tmp = visitTest(i);
      ret.push_back(tmp);
    }
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
  auto not_test = ctx->not_test();
  auto cur = visitNot_test(not_test[0]);
  if (not_test.size() == 1) return cur;
  if (!ToBool(cur)) return cur;
  for (int i = 1; i < not_test.size(); ++i)
  {
    cur = cur & visitNot_test(not_test[i]);
    if (!ToBool(cur)) return cur;
  }
  return cur;
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx)
{
  auto and_test = ctx->and_test();
  auto cur = visitAnd_test(and_test[0]);
  if (ctx->and_test().size() == 1) return cur;
  if (ToBool(cur)) return cur;
  for (int i = 1; i < and_test.size(); ++i)
  {
    cur = cur | visitAnd_test(and_test[i]);
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
  auto testlist = ctx->testlist();
  if (ctx->augassign())
  {
    auto val = visitAugassign(ctx->augassign());
    auto op = *std::any_cast<python_consts::kaug_assign>(&val);
    auto x = std::any_cast<lval_type>(visitTestlist(testlist[0])).second;
    auto var = *std::any_cast<std::any*>(&x);
    auto y = visitTestlist(testlist[1]);
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
    int len = testlist.size();
    auto val = visitTestlist(testlist[len - 1]);
    ToRightVal(val);
    if (len == 1) return val;
    for (int i = len - 2; i >= 0; --i)
    {
      auto tmp = visitTestlist(testlist[i]);
      if (!GetVar(tmp) && !GetTuple(tmp)) assert(false);
      if (GetTuple(tmp) && GetTuple(val))
      {
        auto v1 = *std::any_cast<tuple_type>(&tmp);
        auto v2 = *std::any_cast<tuple_type>(&val);
        if (v1.size() != v2.size()) assert(false);
        for (int j = 0; j < v1.size(); ++j)
        {
          auto var = *std::any_cast<lval_type>(&v1[j]);
          if (!var.first)
          {
            auto name = *std::any_cast<std::string>(&var.second);
            var = std::any_cast<lval_type>(var_scope.QueryVar(name));
            if (var.first)
            {
              *GetVarAddr(var) = v2[j];
            }
            else
            {
              var_scope.RegisterVar(name, v2[j]);
            }
          }
          else { *GetVarAddr(v1[j]) = v2[j]; }
        }
      }
      else
      {
        auto var = *std::any_cast<lval_type>(&tmp);
        if (!var.first)
        {
          auto name = *std::any_cast<std::string>(&var.second);
          var_scope.RegisterVar(name, val);
        }
        else { *GetVarAddr(tmp) = val; }
      }
    }
    return val;
  }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx)
{
  auto arith_expr = ctx->arith_expr();
  auto comp_op = ctx->comp_op();
  std::any lhs = visitArith_expr(arith_expr[0]);
  if (arith_expr.size() == 1) return lhs;
  for (int i = 1; i < arith_expr.size(); ++i)
  {
    std::any rhs = visitArith_expr(arith_expr[i]);
    int op = std::any_cast<python_consts::kcomp_op>
            (visitComp_op(comp_op[i - 1]));
    bool ret;
    switch (op)
    {
      case python_consts::LE: ret = (lhs < rhs); break;
      case python_consts::LEQ: ret = (lhs <= rhs); break;
      case python_consts::GE: ret = (lhs > rhs); break;
      case python_consts::GEQ: ret = (lhs >= rhs); break;
      case python_consts::EQ: ret = (lhs == rhs); break;
      case python_consts::NEQ: ret = (lhs != rhs); break;
      default: assert(false);
    }
    if (!ret) return false;
    lhs = rhs;
  }
  return true;
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx)
{
  return std::make_pair(python_consts::BREAK, std::any());
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx)
{
  return std::make_pair(python_consts::CONTINUE, std::any());
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx)
{
  auto testlist = ctx->testlist();
  auto ret = testlist? visitTestlist(testlist):std::any();
  ToRightVal(ret);
  return std::make_pair(python_consts::RETURN, std::any(ret));
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx)
{
  if (ctx->simple_stmt())
  {
    return visitSimple_stmt(ctx->simple_stmt());
  }
  else
  {
    for (const auto &i: ctx->stmt())
    {
      auto ret = visitStmt(i);
      if (GetFlow(ret)) { return ret; }
    }
  }
  return std::make_pair(python_consts::END, std::any());
}

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx)
{
  auto test = ctx->test();
  auto suite = ctx->suite();
  bool enter_if = false;
  for (int i = 0; i < test.size(); ++i)
  {
    if (ToBool(visitTest(test[i])))
    {
      auto ret = visitSuite(suite[i]);
      enter_if = true;
      if (GetFlow(ret) && GetFlowInfo(ret) != python_consts::END) return ret;
      break;
    }
  }
  if (ctx->ELSE() && !enter_if)
  {
    auto ret = visitSuite(suite[suite.size() - 1]);
    if (GetFlow(ret) && GetFlowInfo(ret) != python_consts::END) return ret;
  }
  return std::any();
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx)
{
  while (ToBool(visitTest(ctx->test())))
  {
    auto ret = visitSuite(ctx->suite());
    if (GetFlow(ret))
    {
      auto status = GetFlowInfo(ret);
      if (status == python_consts::CONTINUE) continue;
      if (status == python_consts::BREAK) break;
      if (status == python_consts::RETURN) return ret;
    }
  }
  return std::any();
}

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx)
{
  if (ctx->simple_stmt()) { return visitSimple_stmt(ctx->simple_stmt()); }
  else { return visitCompound_stmt(ctx->compound_stmt()); }
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx)
{
  return visitSmall_stmt(ctx->small_stmt());
}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx)
{
  if (ctx->expr_stmt()) { return visitExpr_stmt(ctx->expr_stmt()); }
  else { return visitFlow_stmt(ctx->flow_stmt()); }
}

std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx)
{
  if (ctx->break_stmt()) { return visitBreak_stmt(ctx->break_stmt()); }
  else if (ctx->continue_stmt())
  {
    return visitContinue_stmt(ctx->continue_stmt());
  }
  else { return visitReturn_stmt(ctx->return_stmt()); }
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx)
{
  if (ctx->if_stmt()) { return visitIf_stmt(ctx->if_stmt()); }
  else if (ctx->while_stmt()) { return visitWhile_stmt(ctx->while_stmt()); }
  else { return visitFuncdef(ctx->funcdef()); }
}

std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx)
{
  return visitOr_test(ctx->or_test());
}
