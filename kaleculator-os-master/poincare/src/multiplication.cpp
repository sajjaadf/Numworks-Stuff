extern "C" {
#include <assert.h>
#include <stdlib.h>
}
#include <cmath>

#include <poincare/multiplication.h>
#include "layout/string_layout.h"
#include "layout/horizontal_layout.h"
#include "layout/parenthesis_layout.h"

namespace Poincare {

Expression::Type Multiplication::type() const {
  return Expression::Type::Multiplication;
}

ExpressionLayout * Multiplication::privateCreateLayout(FloatDisplayMode floatDisplayMode, ComplexFormat complexFormat) const {
  assert(floatDisplayMode != FloatDisplayMode::Default);
  assert(complexFormat != ComplexFormat::Default);
  ExpressionLayout * children_layouts[3];
  children_layouts[0] = m_operands[0]->createLayout(floatDisplayMode, complexFormat);
  children_layouts[1] = new StringLayout("*", 1);
  children_layouts[2] = m_operands[1]->type() == Type::Opposite ? new ParenthesisLayout(m_operands[1]->createLayout(floatDisplayMode, complexFormat)) : m_operands[1]->createLayout(floatDisplayMode, complexFormat);
  return new HorizontalLayout(children_layouts, 3);
}

Expression * Multiplication::cloneWithDifferentOperands(Expression** newOperands,
    int numberOfOperands, bool cloneOperands) const {
  assert(numberOfOperands == 2);
  assert(newOperands != nullptr);
  return new Multiplication(newOperands, cloneOperands);
}

template<typename T>
Complex<T> Multiplication::compute(const Complex<T> c, const Complex<T> d) {
  return Complex<T>::Cartesian(c.a()*d.a()-c.b()*d.b(), c.b()*d.a() + c.a()*d.b());
}

template<typename T>
Evaluation<T> * Multiplication::computeOnComplexAndMatrix(const Complex<T> * c, Evaluation<T> * m) {
  Multiplication mul;
  return mul.computeOnComplexAndComplexMatrix(c, m);
}

template<typename T>
Evaluation<T> * Multiplication::computeOnMatrices(Evaluation<T> * m, Evaluation<T> * n) {
  if (m->numberOfColumns() != n->numberOfRows()) {
    return new Complex<T>(Complex<T>::Float(NAN));
  }
  Complex<T> * operands = new Complex<T>[m->numberOfRows()*n->numberOfColumns()];
  for (int i = 0; i < m->numberOfRows(); i++) {
    for (int j = 0; j < n->numberOfColumns(); j++) {
      T a = 0.0f;
      T b = 0.0f;
      for (int k = 0; k < m->numberOfColumns(); k++) {
        Complex<T> mEntry = *(m->complexOperand(i*m->numberOfColumns()+k));
        Complex<T> nEntry = *(n->complexOperand(k*n->numberOfColumns()+j));
        a += mEntry.a()*nEntry.a() - mEntry.b()*nEntry.b();
        b += mEntry.b()*nEntry.a() + mEntry.a()*nEntry.b();
      }
      operands[i*n->numberOfColumns()+j] = Complex<T>::Cartesian(a, b);
    }
  }
  Evaluation<T> * result = new ComplexMatrix<T>(operands, m->numberOfRows(), n->numberOfColumns());
  delete[] operands;
  return result;
}

template Poincare::Evaluation<float>* Poincare::Multiplication::computeOnComplexAndMatrix<float>(Poincare::Complex<float> const*, Poincare::Evaluation<float>*);
template Poincare::Evaluation<double>* Poincare::Multiplication::computeOnComplexAndMatrix<double>(Poincare::Complex<double> const*, Poincare::Evaluation<double>*);
}
template Poincare::Complex<float> Poincare::Multiplication::compute<float>(Poincare::Complex<float>, Poincare::Complex<float>);
template Poincare::Complex<double> Poincare::Multiplication::compute<double>(Poincare::Complex<double>, Poincare::Complex<double>);
