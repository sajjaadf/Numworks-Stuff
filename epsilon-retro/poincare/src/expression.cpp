#include <poincare/expression.h>
#include <poincare/preferences.h>
#include <poincare/symbol.h>
#include <poincare/dynamic_hierarchy.h>
#include <poincare/static_hierarchy.h>
#include <poincare/list_data.h>
#include <poincare/matrix_data.h>
#include <poincare/undefined.h>
#include <poincare/simplification_root.h>
#include <poincare/evaluation.h>
#include <poincare/opposite.h>
#include <poincare/variable_context.h>
#include <poincare/decimal.h>
#include <ion.h>
#include <cmath>
#include <float.h>
#include "expression_parser.hpp"
#include "expression_lexer.hpp"

int poincare_expression_yyparse(Poincare::Expression ** expressionOutput);

namespace Poincare {

#include <stdio.h>

/* Constructor & Destructor */

Expression * Expression::parse(char const * string) {
  if (string[0] == 0) {
    return nullptr;
  }
  YY_BUFFER_STATE buf = poincare_expression_yy_scan_string(string);
  Expression * expression = 0;
  if (poincare_expression_yyparse(&expression) != 0) {
    // Parsing failed because of invalid input or memory exhaustion
    if (expression != nullptr) {
      delete expression;
      expression = nullptr;
    }
  }
  poincare_expression_yy_delete_buffer(buf);

  return expression;
}

void Expression::ReplaceSymbolWithExpression(Expression ** expressionAddress, char symbol, Expression * expression) {
  SimplificationRoot root(*expressionAddress);
  root.editableOperand(0)->replaceSymbolWithExpression(symbol, expression);
  *expressionAddress = root.editableOperand(0);
}

Expression * Expression::replaceSymbolWithExpression(char symbol, Expression * expression) {
  for (int i = 0; i < numberOfOperands(); i++) {
    editableOperand(i)->replaceSymbolWithExpression(symbol, expression);
  }
  return this;
}

/* Circuit breaker */

static Expression::CircuitBreaker sCircuitBreaker = nullptr;
static bool sSimplificationHasBeenInterrupted = false;

void Expression::setCircuitBreaker(CircuitBreaker cb) {
  sCircuitBreaker = cb;
}

bool Expression::shouldStopProcessing() {
  if (sCircuitBreaker == nullptr) {
    return false;
  }
  if (sCircuitBreaker()) {
    sSimplificationHasBeenInterrupted = true;
    return true;
  }
  return false;
}

/* Hierarchy */

const Expression * Expression::operand(int i) const {
  assert(i >= 0);
  assert(i < numberOfOperands());
  assert(operands()[i]->parent() == nullptr || operands()[i]->parent() == this);
  return operands()[i];
}

Expression * Expression::replaceWith(Expression * newOperand, bool deleteAfterReplace) {
  assert(m_parent != nullptr);
  m_parent->replaceOperand(this, newOperand, deleteAfterReplace);
  return newOperand;
}

void Expression::replaceOperand(const Expression * oldOperand, Expression * newOperand, bool deleteOldOperand) {
  assert(newOperand != nullptr);
  // Caution: handle the case where we replace an operand with a descendant of ours.
  if (newOperand->hasAncestor(this)) {
    newOperand->parent()->detachOperand(newOperand);
  }
  Expression ** op = const_cast<Expression **>(operands());
  for (int i=0; i<numberOfOperands(); i++) {
    if (op[i] == oldOperand) {
      if (oldOperand != nullptr && oldOperand->parent() == this) {
        const_cast<Expression *>(oldOperand)->setParent(nullptr);
      }
      if (deleteOldOperand) {
        delete oldOperand;
      }
      if (newOperand != nullptr) {
        const_cast<Expression *>(newOperand)->setParent(this);
      }
      op[i] = newOperand;
      break;
    }
  }
}

void Expression::detachOperand(const Expression * e) {
  Expression ** op = const_cast<Expression **>(operands());
  for (int i=0; i<numberOfOperands(); i++) {
    if (op[i] == e) {
      detachOperandAtIndex(i);
    }
  }
}

void Expression::detachOperands() {
  for (int i=0; i<numberOfOperands(); i++) {
    detachOperandAtIndex(i);
  }
}

void Expression::detachOperandAtIndex(int i) {
  Expression ** op = const_cast<Expression **>(operands());
  // When detachOperands is called, it's very likely that said operands have been stolen
  if (op[i] != nullptr && op[i]->parent() == this) {
    const_cast<Expression *>(op[i])->setParent(nullptr);
  }
  op[i] = nullptr;
}

void Expression::swapOperands(int i, int j) {
  assert(i >= 0 && i < numberOfOperands());
  assert(j >= 0 && j < numberOfOperands());
  Expression ** op = const_cast<Expression **>(operands());
  Expression * temp = op[i];
  op[i] = op[j];
  op[j] = temp;
}

bool Expression::hasAncestor(const Expression * e) const {
  if (m_parent == e) {
    return true;
  }
  if (m_parent == nullptr) {
    return false;
  }
  return m_parent->hasAncestor(e);
}

/* Properties */

bool Expression::isRationalZero() const {
  return type() == Type::Rational && static_cast<const Rational*>(this)->isZero();
}

bool Expression::recursivelyMatches(ExpressionTest test, Context & context) const {
  if (test(this, context)) {
    return true;
  }
  for (int i = 0; i < numberOfOperands(); i++) {
    if (operand(i)->recursivelyMatches(test, context)) {
      return true;
    }
  }
  return false;
}

bool Expression::isApproximate(Context & context) const {
  return recursivelyMatches([](const Expression * e, Context & context) {
        return e->type() == Expression::Type::Decimal || Expression::IsMatrix(e, context) || (e->type() == Expression::Type::Symbol && static_cast<const Symbol *>(e)->isApproximate(context));
    }, context);
}

float Expression::characteristicXRange(Context & context, AngleUnit angleUnit) const {
  /* A expression has a characteristic range if at least one of its operand has
   * one and the other are x-independant. We keep the biggest interesting range
   * among the operand interesting ranges. */
  float range = 0.0f;
  for (int i = 0; i < numberOfOperands(); i++) {
    float opRange = operand(i)->characteristicXRange(context, angleUnit);
    if (std::isnan(opRange)) {
      return NAN;
    } else if (range < opRange) {
      range = opRange;
    }
  }
  return range;
}

bool Expression::IsMatrix(const Expression * e, Context & context) {
  return e->type() == Type::Matrix || e->type() == Type::ConfidenceInterval || e->type() == Type::MatrixDimension || e->type() == Type::PredictionInterval || e->type() == Type::MatrixInverse || e->type() == Type::MatrixTranspose || (e->type() == Type::Symbol && static_cast<const Symbol *>(e)->isMatrixSymbol());
}

int Expression::polynomialDegree(char symbolName) const {
  for (int i = 0; i < numberOfOperands(); i++) {
    if (operand(i)->polynomialDegree(symbolName) != 0) {
      return -1;
    }
  }
  return 0;
}

int Expression::getVariables(isVariableTest isVariable, char * variables) const {
 int numberOfVariables = 0;
 for (int i = 0; i < numberOfOperands(); i++) {
   int n = operand(i)->getVariables(isVariable, variables);
   if (n < 0) {
     return -1;
   }
   numberOfVariables = n > numberOfVariables ? n : numberOfVariables;
 }
 return numberOfVariables;
}

bool dependsOnVariables(const Expression * e, Context & context) {
  return e->type() == Expression::Type::Symbol && Symbol::isVariableSymbol(static_cast<const Symbol *>(e)->name());
}

bool Expression::getLinearCoefficients(char * variables, Expression * coefficients[], Expression ** constant, Context & context, AngleUnit angleUnit) const {
  char * x = variables;
  while (*x != 0) {
    int degree = polynomialDegree(*x);
    if (degree > 1 || degree < 0) {
      return false;
    }
    x++;
  }
  Expression * equation = clone();
  x = variables;
  int index = 0;
  Expression * polynomialCoefficients[k_maxNumberOfPolynomialCoefficients];
  while (*x != 0) {
    int degree = equation->getPolynomialCoefficients(*x, polynomialCoefficients, context, angleUnit);
    if (degree == 1) {
      coefficients[index] = polynomialCoefficients[1];
    } else {
      assert(degree == 0);
      coefficients[index] = new Rational(0);
    }
    delete equation;
    equation = polynomialCoefficients[0];
    x++;
    index++;
  }
  *constant = new Opposite(equation, false);
  Reduce(constant, context, angleUnit);
  /* The expression can be linear on all coefficients taken one by one but
   * non-linear (ex: xy = 2). We delete the results and return false if one of
   * the coefficients contains a variable. */
  bool isMultivariablePolynomial = (*constant)->recursivelyMatches(dependsOnVariables, context);
  for (int i = 0; i < index; i++) {
    if (isMultivariablePolynomial) {
      break;
    }
    isMultivariablePolynomial |= coefficients[i]->recursivelyMatches(dependsOnVariables, context);
  }
  if (isMultivariablePolynomial) {
    for (int i = 0; i < index; i++) {
      delete coefficients[i];
      coefficients[i] = nullptr;
    }
    delete *constant;
    *constant = nullptr;
    return false;
  }
  return true;
}

int Expression::getPolynomialCoefficients(char symbolName, Expression * coefficients[], Context & context, AngleUnit angleUnit) const {
  int degree = privateGetPolynomialCoefficients(symbolName, coefficients);
  for (int i = 0; i <= degree; i++) {
    Reduce(&coefficients[i], context, angleUnit);
  }
  return degree;
}

int Expression::privateGetPolynomialCoefficients(char symbolName, Expression * coefficients[]) const {
  int deg = polynomialDegree(symbolName);
  if (deg == 0) {
    coefficients[0] = clone();
    return 0;
  }
  return -1;
}

bool Expression::isOfType(Type * types, int length) const {
  for (int i = 0; i < length; i++) {
    if (type() == types[i]) {
      return true;
    }
  }
  return false;
}

bool Expression::needParenthesisWithParent(const Expression * e) const {
  return false;
}

/* Comparison */

int Expression::SimplificationOrder(const Expression * e1, const Expression * e2, bool canBeInterrupted) {
  if (e1->type() > e2->type()) {
    if (canBeInterrupted && shouldStopProcessing()) {
      return 1;
    }
    return -(e2->simplificationOrderGreaterType(e1, canBeInterrupted));
  } else if (e1->type() == e2->type()) {
    return e1->simplificationOrderSameType(e2, canBeInterrupted);
  } else {
    if (canBeInterrupted && shouldStopProcessing()) {
      return -1;
    }
    return e1->simplificationOrderGreaterType(e2, canBeInterrupted);
  }
}

bool Expression::isEqualToItsApproximationLayout(Expression * approximation, int bufferSize, AngleUnit angleUnit, PrintFloat::Mode floatDisplayMode, int numberOfSignificantDigits, Context & context) {
  char buffer[bufferSize];
  approximation->writeTextInBuffer(buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits);
  /* Warning: we cannot use directly the the approximate expression but we have
   * to re-serialize it because the number of stored significative
   * numbers and the number of displayed significative numbers might not be
   * identical. (For example, 0.000025 might be displayed "0.00003" and stored
   * as Decimal(0.000025) and isEqualToItsApproximationLayout should return
   * false) */
  Expression * approximateOutput = Expression::ParseAndSimplify(buffer, context, angleUnit);
  bool equal = isIdenticalTo(approximateOutput);
  delete approximateOutput;
  return equal;
}

/* Simplification */

Expression * Expression::ParseAndSimplify(const char * text, Context & context, AngleUnit angleUnit) {
  Expression * exp = parse(text);
  if (exp == nullptr) {
    return new Undefined();
  }
  Simplify(&exp, context, angleUnit);
  if (exp == nullptr) {
    return parse(text);
  }
  return exp;
}

void Expression::Simplify(Expression ** expressionAddress, Context & context, AngleUnit angleUnit) {
  sSimplificationHasBeenInterrupted = false;
#if MATRIX_EXACT_REDUCING
#else
  if ((*expressionAddress)->recursivelyMatches(IsMatrix, context)) {
    return;
  }
#endif
  SimplificationRoot root(*expressionAddress);
  root.editableOperand(0)->deepReduce(context, angleUnit);
  root.editableOperand(0)->deepBeautify(context, angleUnit);
  *expressionAddress = root.editableOperand(0);
  if (sSimplificationHasBeenInterrupted) {
    root.detachOperands();
    delete *expressionAddress;
    *expressionAddress = nullptr;
  }
}


void Expression::Reduce(Expression ** expressionAddress, Context & context, AngleUnit angleUnit, bool recursively) {
  SimplificationRoot root(*expressionAddress);
  if (recursively) {
    root.editableOperand(0)->deepReduce(context, angleUnit);
  } else {
    root.editableOperand(0)->shallowReduce(context,angleUnit);
  }
  *expressionAddress = root.editableOperand(0);
}

Expression * Expression::deepReduce(Context & context, AngleUnit angleUnit) {
  assert(parent() != nullptr);
  for (int i = 0; i < numberOfOperands(); i++) {
    editableOperand(i)->deepReduce(context, angleUnit);
  }
  return shallowReduce(context, angleUnit);
}

Expression * Expression::shallowReduce(Context & context, AngleUnit angleUnit) {
  for (int i = 0; i < numberOfOperands(); i++) {
    if (editableOperand(i)->type() == Type::Undefined && this->type() != Type::SimplificationRoot) {
      return replaceWith(new Undefined(), true);
    }
  }
  return this;
}

Expression * Expression::deepBeautify(Context & context, AngleUnit angleUnit) {
  assert(parent() != nullptr);
  Expression * e = shallowBeautify(context, angleUnit);
  for (int i = 0; i < e->numberOfOperands(); i++) {
    e->editableOperand(i)->deepBeautify(context, angleUnit);
  }
  return e;
}

/* Evaluation */

template<typename T> Expression * Expression::approximate(Context& context, AngleUnit angleUnit, ComplexFormat complexFormat) const {
  Evaluation<T> * e = privateApproximate(T(), context, angleUnit);
  Expression * result = e->complexToExpression(complexFormat);
  delete e;
  return result;
}

template<typename T> T Expression::approximateToScalar(Context& context, AngleUnit angleUnit) const {
  Evaluation<T> * evaluation = privateApproximate(T(), context, angleUnit);
  T result = evaluation->toScalar();
  /*if (evaluation->type() == Type::Matrix) {
    if (numberOfOperands() == 1) {
      result = static_cast<const Complex<T> *>(operand(0))->toScalar();
    }
  }*/
  delete evaluation;
  return result;
}

template<typename T> T Expression::approximateToScalar(const char * text, Context& context, AngleUnit angleUnit) {
  Expression * exp = ParseAndSimplify(text, context, angleUnit);
  T result = exp->approximateToScalar<T>(context, angleUnit);
  delete exp;
  return result;
}

template<typename T> T Expression::epsilon() {
  static T epsilon = sizeof(T) == sizeof(double) ? 1E-15 : 1E-7f;
  return epsilon;
}

/* Expression roots/extrema solver*/

Expression::Coordinate2D Expression::nextMinimum(char symbol, double start, double step, double max, Context & context, AngleUnit angleUnit) const {
  return nextMinimumOfExpression(symbol, start, step, max, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1 = nullptr) {
        return expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
      }, context, angleUnit);
}

Expression::Coordinate2D Expression::nextMaximum(char symbol, double start, double step, double max, Context & context, AngleUnit angleUnit) const {
  Coordinate2D minimumOfOpposite = nextMinimumOfExpression(symbol, start, step, max, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1 = nullptr) {
        return -expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
      }, context, angleUnit);
  return {.abscissa = minimumOfOpposite.abscissa, .value = -minimumOfOpposite.value};
}

double Expression::nextRoot(char symbol, double start, double step, double max, Context & context, AngleUnit angleUnit) const {
  return nextIntersectionWithExpression(symbol, start, step, max, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1 = nullptr) {
        return expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
      }, context, angleUnit, nullptr);
}

Expression::Coordinate2D Expression::nextIntersection(char symbol, double start, double step, double max, Poincare::Context & context, AngleUnit angleUnit, const Expression * expression) const {
  double resultAbscissa = nextIntersectionWithExpression(symbol, start, step, max, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1) {
        return expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit)-expression1->approximateWithValueForSymbol(symbol, x, context, angleUnit);
      }, context, angleUnit, expression);
  Expression::Coordinate2D result = {.abscissa = resultAbscissa, .value = approximateWithValueForSymbol(symbol, resultAbscissa, context, angleUnit)};
  if (std::fabs(result.value) < step*k_solverPrecision) {
    result.value = 0.0;
  }
  return result;
}

Expression::Coordinate2D Expression::nextMinimumOfExpression(char symbol, double start, double step, double max, EvaluationAtAbscissa evaluate, Context & context, AngleUnit angleUnit, const Expression * expression, bool lookForRootMinimum) const {
  Coordinate2D result = {.abscissa = NAN, .value = NAN};
  if (start == max || step == 0.0) {
    return result;
  }
  double bracket[3];
  double x = start;
  bool endCondition = false;
  do {
    bracketMinimum(symbol, x, step, max, bracket, evaluate, context, angleUnit, expression);
    result = brentMinimum(symbol, bracket[0], bracket[2], evaluate, context, angleUnit, expression);
    x = bracket[1];
    // Because of float approximation, exact zero is never reached
    if (std::fabs(result.abscissa) < std::fabs(step)*k_solverPrecision) {
      result.abscissa = 0;
      result.value = evaluate(symbol, 0, context, angleUnit, this, expression);
    }
    /* Ignore extremum whose value is undefined or too big because they are
     * really unlikely to be local extremum. */
    if (std::isnan(result.value) || std::fabs(result.value) > k_maxFloat) {
      result.abscissa = NAN;
    }
    // Idem, exact 0 never reached
    if (std::fabs(result.value) < std::fabs(step)*k_solverPrecision) {
      result.value = 0;
    }
    endCondition = std::isnan(result.abscissa) && (step > 0.0 ? x <= max : x >= max);
    if (lookForRootMinimum) {
      endCondition |= std::fabs(result.value) > 0 && (step > 0.0 ? x <= max : x >= max);
    }
  } while (endCondition);
  if (lookForRootMinimum) {
    result.abscissa = std::fabs(result.value) > 0 ? NAN : result.abscissa;
  }
  return result;
}

void Expression::bracketMinimum(char symbol, double start, double step, double max, double result[3], EvaluationAtAbscissa evaluate, Context & context, AngleUnit angleUnit, const Expression * expression) const {
  Coordinate2D p[3];
  p[0] = {.abscissa = start, .value = evaluate(symbol, start, context, angleUnit, this, expression)};
  p[1] = {.abscissa = start+step, .value = evaluate(symbol, start+step, context, angleUnit, this, expression)};
  double x = start+2.0*step;
  while (step > 0.0 ? x <= max : x >= max) {
    p[2] = {.abscissa = x, .value = evaluate(symbol, x, context, angleUnit, this, expression)};
    if ((p[0].value > p[1].value || std::isnan(p[0].value)) && (p[2].value > p[1].value || std::isnan(p[2].value)) && (!std::isnan(p[0].value) || !std::isnan(p[2].value))) {
      result[0] = p[0].abscissa;
      result[1] = p[1].abscissa;
      result[2] = p[2].abscissa;
      return;
    }
    if (p[0].value > p[1].value && p[1].value == p[2].value) {
    } else {
      p[0] = p[1];
      p[1] = p[2];
    }
    x += step;
  }
  result[0] = NAN;
  result[1] = NAN;
  result[2] = NAN;
}

Expression::Coordinate2D Expression::brentMinimum(char symbol, double ax, double bx, EvaluationAtAbscissa evaluate, Context & context, AngleUnit angleUnit, const Expression * expression) const {
  /* Bibliography: R. P. Brent, Algorithms for finding zeros and extrema of
   * functions without calculating derivatives */
  if (ax > bx) {
    return brentMinimum(symbol, bx, ax, evaluate, context, angleUnit, expression);
  }
  double e = 0.0;
  double a = ax;
  double b = bx;
  double x = a+k_goldenRatio*(b-a);
  double v = x;
  double w = x;
  double fx = evaluate(symbol, x, context, angleUnit, this, expression);
  double fw = fx;
  double fv = fw;

  double d = NAN;
  double u, fu;

  for (int i = 0; i < 100; i++) {
    double m = 0.5*(a+b);
    double tol1 = k_sqrtEps*std::fabs(x)+1E-10;
    double tol2 = 2.0*tol1;
    if (std::fabs(x-m) <= tol2-0.5*(b-a))  {
      double middleFax = evaluate(symbol, (x+a)/2.0, context, angleUnit, this, expression);
      double middleFbx = evaluate(symbol, (x+b)/2.0, context, angleUnit, this, expression);
      double fa = evaluate(symbol, a, context, angleUnit, this, expression);
      double fb = evaluate(symbol, b, context, angleUnit, this, expression);
      if (middleFax-fa <= k_sqrtEps && fx-middleFax <= k_sqrtEps && fx-middleFbx <= k_sqrtEps && middleFbx-fb <= k_sqrtEps) {
        Coordinate2D result = {.abscissa = x, .value = fx};
        return result;
      }
    }
    double p = 0;
    double q = 0;
    double r = 0;
    if (std::fabs(e) > tol1) {
      r = (x-w)*(fx-fv);
      q = (x-v)*(fx-fw);
      p = (x-v)*q -(x-w)*r;
      q = 2.0*(q-r);
      if (q>0.0) {
        p = -p;
      } else {
        q = -q;
      }
      r = e;
      e = d;
    }
    if (std::fabs(p) < std::fabs(0.5*q*r) && p<q*(a-x) && p<q*(b-x)) {
      d = p/q;
      u= x+d;
      if (u-a < tol2 || b-u < tol2) {
        d = x < m ? tol1 : -tol1;
      }
    } else {
      e = x<m ? b-x : a-x;
      d = k_goldenRatio*e;
    }
    u = x + (std::fabs(d) >= tol1 ? d : (d>0 ? tol1 : -tol1));
    fu = evaluate(symbol, u, context, angleUnit, this, expression);
    if (fu <= fx) {
      if (u<x) {
        b = x;
      } else {
        a = x;
      }
      v = w;
      fv = fw;
      w = x;
      fw = fx;
      x = u;
      fx = fu;
    } else {
      if (u<x) {
        a = u;
      } else {
        b = u;
      }
      if (fu <= fw || w == x) {
        v = w;
        fv = fw;
        w = u;
        fw = fu;
      } else if (fu <= fv || v == x || v == w) {
        v = u;
        fv = fu;
      }
    }
  }
  Coordinate2D result = {.abscissa = x, .value = fx};
  return result;
}

double Expression::nextIntersectionWithExpression(char symbol, double start, double step, double max, EvaluationAtAbscissa evaluation, Context & context, AngleUnit angleUnit, const Expression * expression) const {
  if (start == max || step == 0.0) {
    return NAN;
  }
  double bracket[2];
  double result = NAN;
  static double precisionByGradUnit = 1E6;
  double x = start+step;
  do {
    bracketRoot(symbol, x, step, max, bracket, evaluation, context, angleUnit, expression);
    result = brentRoot(symbol, bracket[0], bracket[1], std::fabs(step/precisionByGradUnit), evaluation, context, angleUnit, expression);
    x = bracket[1];
  } while (std::isnan(result) && (step > 0.0 ? x <= max : x >= max));

  double extremumMax = std::isnan(result) ? max : result;
  Coordinate2D resultExtremum[2] = {
    nextMinimumOfExpression(symbol, start, step, extremumMax, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1) {
        if (expression1) {
          return expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit)-expression1->approximateWithValueForSymbol(symbol, x, context, angleUnit);
        } else {
          return expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
        }
      }, context, angleUnit, expression, true),
    nextMinimumOfExpression(symbol, start, step, extremumMax, [](char symbol, double x, Context & context, AngleUnit angleUnit, const Expression * expression0, const Expression * expression1) {
        if (expression1) {
          return expression1->approximateWithValueForSymbol(symbol, x, context, angleUnit)-expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
        } else {
          return -expression0->approximateWithValueForSymbol(symbol, x, context, angleUnit);
        }
      }, context, angleUnit, expression, true)};
  for (int i = 0; i < 2; i++) {
    if (!std::isnan(resultExtremum[i].abscissa) && (std::isnan(result) || std::fabs(result - start) > std::fabs(resultExtremum[i].abscissa - start))) {
      result = resultExtremum[i].abscissa;
    }
  }
  if (std::fabs(result) < std::fabs(step)*k_solverPrecision) {
    result = 0;
  }
  return result;
}

void Expression::bracketRoot(char symbol, double start, double step, double max, double result[2], EvaluationAtAbscissa evaluation, Context & context, AngleUnit angleUnit, const Expression * expression) const {
  double a = start;
  double b = start+step;
  while (step > 0.0 ? b <= max : b >= max) {
    double fa = evaluation(symbol, a, context, angleUnit, this, expression);
    double fb = evaluation(symbol, b, context, angleUnit, this, expression);
    if (fa*fb <= 0) {
      result[0] = a;
      result[1] = b;
      return;
    }
    a = b;
    b = b+step;
  }
  result[0] = NAN;
  result[1] = NAN;
}


double Expression::brentRoot(char symbol, double ax, double bx, double precision, EvaluationAtAbscissa evaluation, Context & context, AngleUnit angleUnit, const Expression * expression) const {
  if (ax > bx) {
    return brentRoot(symbol, bx, ax, precision, evaluation, context, angleUnit, expression);
  }
  double a = ax;
  double b = bx;
  double c = bx;
  double d = b-a;
  double e = b-a;
  double fa = evaluation(symbol, a, context, angleUnit, this, expression);
  double fb = evaluation(symbol, b, context, angleUnit, this, expression);
  double fc = fb;
  for (int i = 0; i < 100; i++) {
    if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
      c = a;
      fc = fa;
      e = b-a;
      d = b-a;
    }
    if (std::fabs(fc) < std::fabs(fb)) {
      a = b;
      b = c;
      c = a;
      fa = fb;
      fb = fc;
      fc = fa;
    }
    double tol1 = 2.0*DBL_EPSILON*std::fabs(b)+0.5*precision;
    double xm = 0.5*(c-b);
    if (std::fabs(xm) <= tol1 || fb == 0.0) {
      double fbcMiddle = evaluation(symbol, 0.5*(b+c), context, angleUnit, this, expression);
      double isContinuous = (fb <= fbcMiddle && fbcMiddle <= fc) || (fc <= fbcMiddle && fbcMiddle <= fb);
      if (isContinuous) {
        return b;
      }
    }
    if (std::fabs(e) >= tol1 && std::fabs(fa) > std::fabs(b)) {
      double s = fb/fa;
      double p = 2.0*xm*s;
      double q = 1.0-s;
      if (a != c) {
        q = fa/fc;
        double r = fb/fc;
        p = s*(2.0*xm*q*(q-r)-(b-a)*(r-1.0));
        q = (q-1.0)*(r-1.0)*(s-1.0);
      }
      q = p > 0.0 ? -q : q;
      p = std::fabs(p);
      double min1 = 3.0*xm*q-std::fabs(tol1*q);
      double min2 = std::fabs(e*q);
      if (2.0*p < (min1 < min2 ? min1 : min2)) {
        e = d;
        d = p/q;
      } else {
        d = xm;
        e =d;
      }
    } else {
      d = xm;
      e = d;
    }
    a = b;
    fa = fb;
    if (std::fabs(d) > tol1) {
      b += d;
    } else {
      b += xm > 0.0 ? tol1 : tol1;
    }
    fb = evaluation(symbol, b, context, angleUnit, this, expression);
  }
  return NAN;
}

template<typename T>
T Expression::approximateWithValueForSymbol(char symbol, T x, Context & context, AngleUnit angleUnit) const {
  VariableContext<T> variableContext = VariableContext<T>(symbol, &context);
  variableContext.setApproximationForVariable(x);
  T value = approximateToScalar<T>(variableContext, angleUnit);
  return value;
}

}

template Poincare::Expression * Poincare::Expression::approximate<double>(Context& context, AngleUnit angleUnit, ComplexFormat complexFormat) const;
template Poincare::Expression * Poincare::Expression::approximate<float>(Context& context, AngleUnit angleUnit, ComplexFormat complexFormat) const;
template double Poincare::Expression::approximateToScalar<double>(char const*, Poincare::Context&, Poincare::Expression::AngleUnit);
template float Poincare::Expression::approximateToScalar<float>(char const*, Poincare::Context&, Poincare::Expression::AngleUnit);
template double Poincare::Expression::approximateToScalar<double>(Poincare::Context&, Poincare::Expression::AngleUnit) const;
template float Poincare::Expression::approximateToScalar<float>(Poincare::Context&, Poincare::Expression::AngleUnit) const;
template double Poincare::Expression::epsilon<double>();
template float Poincare::Expression::epsilon<float>();
template double Poincare::Expression::approximateWithValueForSymbol(char, double, Poincare::Context&, AngleUnit) const;
template float Poincare::Expression::approximateWithValueForSymbol(char, float, Poincare::Context&, AngleUnit) const;
