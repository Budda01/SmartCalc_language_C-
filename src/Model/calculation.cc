#include "model.h"

namespace s21 {

double Model::CalcExp(std::deque<Model::Node> expr, int& err) {
  std::stack<Model::Node> stack;
  for (auto i = expr.begin(); i != expr.end(); ++i) {
    if (i->type == DIGIT) {
      stack.push(*i);
    } else if (i->type <= 6) {
      err = BinOpCalc(stack, i->type);
    } else if (i->type > 6 && i->type <= 17) {
      err = UnOpCalc(stack, i->type);
    }
  }
  return stack.top().value;
}

int Model::UnOpCalc(std::stack<Model::Node>& stack, Model::NodeType oper) {
  double value = stack.top().value;
  stack.pop();
  double val = 0;
  int err = 0;
  switch (oper) {
    case UNAR_M:
      val = (-1) * value;
      break;
    case UNAR_P:
      val = value;
      break;
    case SQRT:
      if (value < 0)
        err = 2;
      else
        val = sqrt(value);
      break;
    case LN:
      if (value <= 0)
        err = 2;
      else
        val = log(value);
      break;
    case LOG:
      if (value <= 0)
        err = 2;
      else
        val = log10(value);
      break;
    case SIN:
      val = sin(value);
      break;
    case COS:
      val = cos(value);
      break;
    case TAN:
      val = tan(value);
      break;
    case ACOS:
      if (value < -1 || value > 1)
        err = 2;
      else
        val = acos(value);
      break;
    case ASIN:
      if (value < -1 || value > 1)
        err = 2;
      else
        val = asin(value);
      break;
    case ATAN:
      val = atan(value);
      break;
    default:
      break;
  }
  Model::Node res(Model::DIGIT, 0, val);
  stack.push(res);
  return err;
}

int Model::BinOpCalc(std::stack<Model::Node>& stack, Model::NodeType oper) {
  double value1 = stack.top().value;
  stack.pop();
  double value2 = stack.top().value;
  stack.pop();
  double val = 0;
  int err = 0;
  switch (oper) {
    case PLUS:
      val = value1 + value2;
      break;
    case MINUS:
      val = value2 - value1;
      break;
    case DIV:
      if (value1 != 0)
        val = value2 / value1;
      else
        err = 2;
      break;
    case MUL:
      val = value1 * value2;
      break;
    case POW:
      if ((value2 == 0 && value1 == 0) ||
          (value2 < 0 && value1 - std::floor(value1) != 0))
        err = 2;
      else
        val = pow(value2, value1);
      break;
    case MOD:
      val = fmod(value2, value1);
      break;
    default:
      break;
  }
  Model::Node res(Model::DIGIT, 0, val);
  stack.push(res);
  return err;
}

int Model::SmartCalc(const std::string input, std::string& result) {
  const char* str = input.c_str();
  double res = 0;
  std::deque<Model::Node> queue;
  int err = ToPostfix(str, queue);
  if (err == 0) {
    res = CalcExp(queue, err);
  }
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(7) << res;
  result = stream.str();
  return err;
}

}  // namespace s21