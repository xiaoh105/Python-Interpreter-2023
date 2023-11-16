#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include <any>
#include "Python3ParserBaseVisitor.h"
#include "Scope.h"

class EvalVisitor : public Python3ParserBaseVisitor
{
 public:
  virtual std::any visitAtom(Python3Parser::AtomContext *ctx) override;
  virtual std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;
  virtual std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;
  virtual std::any visitArgument(Python3Parser::ArgumentContext *ctx) override;
  virtual std::any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override;
  virtual std::any visitFactor(Python3Parser::FactorContext *ctx) override;
  virtual std::any visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) override;
  virtual std::any visitTerm(Python3Parser::TermContext *ctx) override;
  virtual std::any visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) override;
  virtual std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;
  virtual std::any visitNot_test(Python3Parser::Not_testContext *ctx) override;
  virtual std::any visitTestlist(Python3Parser::TestlistContext *ctx) override;
  virtual std::any visitComp_op(Python3Parser::Comp_opContext *ctx) override;
  virtual std::any visitAnd_test(Python3Parser::And_testContext *ctx) override;
  virtual std::any visitOr_test(Python3Parser::Or_testContext *ctx) override;
  virtual std::any visitAugassign(Python3Parser::AugassignContext *ctx) override;
  virtual std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;
  virtual std::any visitComparison(Python3Parser::ComparisonContext *ctx) override;
};


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
