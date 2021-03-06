#ifndef POINCARE_ADDITION_H
#define POINCARE_ADDITION_H

#include <poincare/dynamic_hierarchy.h>
#include <poincare/rational.h>
#include <poincare/layout_engine.h>
#include <poincare/approximation_engine.h>

namespace Poincare {

class Addition : public DynamicHierarchy {
  using DynamicHierarchy::DynamicHierarchy;
  friend class Logarithm;
  friend class Multiplication;
  friend class Subtraction;
  friend class Power;
  friend class Complex<float>;
  friend class Complex<double>;
public:
  Type type() const override;
  Expression * clone() const override;
  int polynomialDegree(char symbolName) const override;
  int privateGetPolynomialCoefficients(char symbolName, Expression * coefficients[]) const override;
  /* Evaluation */
  template<typename T> static std::complex<T> compute(const std::complex<T> c, const std::complex<T> d);
  template<typename T> static MatrixComplex<T> computeOnMatrices(const MatrixComplex<T> m, const MatrixComplex<T> n) {
    return ApproximationEngine::elementWiseOnComplexMatrices(m, n, compute<T>);
  }
  template<typename T> static MatrixComplex<T> computeOnComplexAndMatrix(const std::complex<T> c, const MatrixComplex<T> m) {
    return ApproximationEngine::elementWiseOnMatrixComplexAndComplex(m, c, compute<T>);
  }
private:
  /* Layout */
  bool needParenthesisWithParent(const Expression * e) const override;
  ExpressionLayout * createLayout(PrintFloat::Mode floatDisplayMode, int numberOfSignificantDigits) const override {
    return LayoutEngine::createInfixLayout(this, floatDisplayMode, numberOfSignificantDigits, name());
  }
  int writeTextInBuffer(char * buffer, int bufferSize, PrintFloat::Mode floatDisplayMode, int numberOfSignificantDigits) const override {
    return LayoutEngine::writeInfixExpressionTextInBuffer(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, name());
  }
  static const char * name() { return "+"; }

  /* Simplification */
  Expression * shallowReduce(Context& context, AngleUnit angleUnit) override;
  Expression * shallowBeautify(Context & context, AngleUnit angleUnit) override;
  Expression * factorizeOnCommonDenominator(Context & context, AngleUnit angleUnit);
  void factorizeOperands(Expression * e1, Expression * e2, Context & context, AngleUnit angleUnit);
  static const Rational RationalFactor(Expression * e);
  static bool TermsHaveIdenticalNonRationalFactors(const Expression * e1, const Expression * e2);
  /* Evaluation */
  template<typename T> static MatrixComplex<T> computeOnMatrixAndComplex(const MatrixComplex<T> m, const std::complex<T> c) {
    return ApproximationEngine::elementWiseOnMatrixComplexAndComplex(m, c, compute<T>);
  }
  Evaluation<float> * privateApproximate(SinglePrecision p, Context& context, AngleUnit angleUnit) const override {
    return ApproximationEngine::mapReduce<float>(this, context, angleUnit, compute<float>, computeOnComplexAndMatrix<float>, computeOnMatrixAndComplex<float>, computeOnMatrices<float>);
   }
  Evaluation<double> * privateApproximate(DoublePrecision p, Context& context, AngleUnit angleUnit) const override {
    return ApproximationEngine::mapReduce<double>(this, context, angleUnit, compute<double>, computeOnComplexAndMatrix<double>, computeOnMatrixAndComplex<double>, computeOnMatrices<double>);
   }
};

}

#endif
