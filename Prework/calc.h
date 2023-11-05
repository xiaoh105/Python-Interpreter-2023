// 你需要提交这份代码
#pragma once

#include "visitor.h"

struct calculator : visitor
{
  std::any visit_num(num_node *ctx) override;
  std::any visit_add(add_node *ctx) override;
  std::any visit_sub(sub_node *ctx) override;
  std::any visit_mul(mul_node *ctx) override;
  std::any visit_div(div_node *ctx) override;
  ~calculator() override = default;
};

std::any calculator::visit_num(num_node *ctx)
{
  return ctx->number;
}

std::any calculator::visit_add(add_node *ctx)
{
  std::any l_ans = visit(ctx->lnode);
  std::any r_ans = visit(ctx->rnode);
  if (l_ans.type() == typeid(long long))
  {
    auto x = std::any_cast<long long> (l_ans);
    auto y = std::any_cast<long long> (r_ans);
    return x + y;
  }
  else
  {
    auto x = std::any_cast<double> (l_ans);
    auto y = std::any_cast<double> (r_ans);
    return x + y;
  }
}

std::any calculator::visit_sub(sub_node *ctx)
{
  std::any l_ans = visit(ctx->lnode);
  std::any r_ans = visit(ctx->rnode);
  if (l_ans.type() == typeid(long long))
  {
    auto x = std::any_cast<long long> (l_ans);
    auto y = std::any_cast<long long> (r_ans);
    return x - y;
  }
  else
  {
    auto x = std::any_cast<double> (l_ans);
    auto y = std::any_cast<double> (r_ans);
    return x - y;
  }
}

std::any calculator::visit_mul(mul_node *ctx)
{
  std::any l_ans = visit(ctx->lnode);
  std::any r_ans = visit(ctx->rnode);
  if (l_ans.type() == typeid(long long))
  {
    auto x = std::any_cast<long long> (l_ans);
    auto y = std::any_cast<long long> (r_ans);
    return x * y;
  }
  else
  {
    auto x = std::any_cast<double> (l_ans);
    auto y = std::any_cast<double> (r_ans);
    return x * y;
  }
}

std::any calculator::visit_div(div_node *ctx)
{
  std::any l_ans = visit(ctx->lnode);
  std::any r_ans = visit(ctx->rnode);
  if (l_ans.type() == typeid(long long))
  {
    auto x = std::any_cast<long long> (l_ans);
    auto y = std::any_cast<long long> (r_ans);
    return x / y;
  }
  else
  {
    auto x = std::any_cast<double> (l_ans);
    auto y = std::any_cast<double> (r_ans);
    return x / y;
  }
}