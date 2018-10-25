#ifndef POINCARE_OPPOSITE_H
#define POINCARE_OPPOSITE_H

#include <poincare/expression.h>
#include <poincare/matrix.h>

namespace Poincare {

class Opposite : public Expression {
public:
  Opposite(Expression * operand, bool cloneOperands = true);
  ~Opposite();
  Opposite(const Opposite& other) = delete;
  Opposite(Opposite&& other) = delete;
  Opposite& operator=(const Opposite& other) = delete;
  Opposite& operator=(Opposite&& other) = delete;
  bool hasValidNumberOfArguments() const override;
  const Expression * operand(int i) const override;
  int numberOfOperands() const override;
  Expression * clone() const override;
  Type type() const override;
  Expression * cloneWithDifferentOperands(Expression** newOperands,
    int numnerOfOperands, bool cloneOperands = true) const override;
  template<typename T> static Complex<T> compute(const Complex<T> c);
  template<typename T> static Evaluation<T> * computeOnMatrix(Evaluation<T> * m);
private:
  Evaluation<float> * privateEvaluate(SinglePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedEvaluate<float>(context, angleUnit); }
  Evaluation<double> * privateEvaluate(DoublePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedEvaluate<double>(context, angleUnit); }
 template<typename T> Evaluation<T> * templatedEvaluate(Context& context, AngleUnit angleUnit) const;
  ExpressionLayout * privateCreateLayout(FloatDisplayMode floatDisplayMode, ComplexFormat complexFormat) const override;
  Expression * m_operand;
};

}

#endif
