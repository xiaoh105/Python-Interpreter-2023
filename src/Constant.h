#include <any>
#pragma once
#ifndef PYTHON_INTERPRETER_CONSTANT_H
#define PYTHON_INTERPRETER_CONSTANT_H
namespace python_consts
{
  constexpr int kMAX_FUNC = 2000;
  constexpr double kEPS = 1e-9;
  enum ktype{ FLOAT = 0, INT = 1, BOOL = 2, STR = 3, TUPLE = 4, VAR = 5, NONE = 6 };
  enum kterm_op{ MUL, DIV, IDIV, MOD };
  enum karith_op{ ADD, MINUS };
  enum kcomp_op{ LE, GE, LEQ, GEQ, EQ, NEQ };
  enum kaug_assign{ ADDASSIGN, SUBASSIGN, MULASSIGN,
                    DIVASSIGN, IDIVASSIGN, MODASSIGN};
  enum kflow_info{ BREAK, CONTINUE, RETURN, END };
}
using tuple_type = std::vector<std::any>;
using lval_type = std::pair<bool, std::any>;
using arglist_type = std::vector<std::pair<std::string, std::any>>;
#endif //PYTHON_INTERPRETER_CONSTANT_H
