#ifndef PYTHON_INTERPRETER_CONSTANT_H
#define PYTHON_INTERPRETER_CONSTANT_H
namespace python_consts
{
  constexpr int kMAX_FUNC = 2000;
  constexpr double kEPS = 1e-9;
  enum kTYPE{ FLOAT = 0, INT = 1, BOOL = 2, STR = 3, TUPLE = 4, NONE = 5 };
}
#endif //PYTHON_INTERPRETER_CONSTANT_H
