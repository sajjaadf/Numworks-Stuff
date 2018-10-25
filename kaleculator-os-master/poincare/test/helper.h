#include <poincare.h>

constexpr Poincare::Expression::AngleUnit Degree = Poincare::Expression::AngleUnit::Degree;
constexpr Poincare::Expression::AngleUnit Radian = Poincare::Expression::AngleUnit::Radian;

void assert_parsed_expression_type(const char * expression, Poincare::Expression::Type type);
void assert_parsed_simplified_expression_type(const char * expression, Poincare::Expression::Type type);
template<typename T>
void assert_parsed_expression_evaluates_to(const char * expression, Poincare::Complex<T> * results, int numberOfRows, int numberOfColumns = 1, Poincare::Expression::AngleUnit angleUnit = Degree);
template<typename T>
void assert_parsed_expression_evaluates_to(const char * expression, Poincare::Complex<T> * results, Poincare::Expression::AngleUnit angleUnit = Degree) {
  assert_parsed_expression_evaluates_to(expression, results, 1, 1, angleUnit);
}
