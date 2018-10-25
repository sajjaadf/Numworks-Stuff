#ifndef POINCARE_BINARY_OPERATION_H
#define POINCARE_BINARY_OPERATION_H

#include <poincare/expression.h>
#include <poincare/matrix.h>
#include <poincare/complex.h>
#include <poincare/complex_matrix.h>

namespace Poincare {

class BinaryOperation : public Expression {
public:
  BinaryOperation();
  BinaryOperation(Expression ** operands, bool cloneOperands = true);
  ~BinaryOperation();
  BinaryOperation(const BinaryOperation& other) = delete;
  BinaryOperation(BinaryOperation&& other) = delete;
  BinaryOperation& operator=(const BinaryOperation& other) = delete;
  BinaryOperation& operator=(BinaryOperation&& other) = delete;
  bool hasValidNumberOfArguments() const override;
  const Expression * operand(int i) const override;
  int numberOfOperands() const override;
  Expression * clone() const override;
protected:
  Expression * m_operands[2];
  virtual Evaluation<float> * privateEvaluate(SinglePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedEvaluate<float>(context, angleUnit); }
  virtual Evaluation<double> * privateEvaluate(DoublePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedEvaluate<double>(context, angleUnit); }
  template<typename T> Evaluation<T> * templatedEvaluate(Context& context, AngleUnit angleUnit) const;

  virtual Evaluation<float> * computeOnComplexAndComplexMatrix(const Complex<float> * c, Evaluation<float> * n) const {
    return templatedComputeOnComplexAndComplexMatrix(c, n);
  }
  virtual Evaluation<double> * computeOnComplexAndComplexMatrix(const Complex<double> * c, Evaluation<double> * n) const {
    return templatedComputeOnComplexAndComplexMatrix(c, n);
  }
  template<typename T> Evaluation<T> * templatedComputeOnComplexAndComplexMatrix(const Complex<T> * c, Evaluation<T> * n) const;

  virtual Evaluation<float> * computeOnComplexMatrixAndComplex(Evaluation<float> * m, const Complex<float> * d) const {
    return templatedComputeOnComplexMatrixAndComplex(m, d);
  }
  virtual Evaluation<double> * computeOnComplexMatrixAndComplex(Evaluation<double> * m, const Complex<double> * d) const {
    return templatedComputeOnComplexMatrixAndComplex(m, d);
  }
  template<typename T> Evaluation<T> * templatedComputeOnComplexMatrixAndComplex(Evaluation<T> * m, const Complex<T> * d) const;

  virtual Evaluation<float> * computeOnComplexMatrices(Evaluation<float> * m, Evaluation<float> * n) const {
    return templatedComputeOnComplexMatrices(m, n);
  }
  virtual Evaluation<double> * computeOnComplexMatrices(Evaluation<double> * m, Evaluation<double> * n) const {
    return templatedComputeOnComplexMatrices(m, n);
  }
  template<typename T> Evaluation<T> * templatedComputeOnComplexMatrices(Evaluation<T> * m, Evaluation<T> * n) const;

  virtual Complex<float> privateCompute(const Complex<float> c, const Complex<float> d) const = 0;
  virtual Complex<double> privateCompute(const Complex<double> c, const Complex<double> d) const = 0;
};

}

#endif
