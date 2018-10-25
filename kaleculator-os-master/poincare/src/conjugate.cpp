#include <poincare/conjugate.h>
#include <poincare/complex.h>
#include "layout/conjugate_layout.h"

extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

Conjugate::Conjugate() :
  Function("conj")
{
}

Expression::Type Conjugate::type() const {
  return Type::Conjugate;
}

Expression * Conjugate::cloneWithDifferentOperands(Expression** newOperands,
        int numberOfOperands, bool cloneOperands) const {
  assert(newOperands != nullptr);
  Conjugate * c = new Conjugate();
  c->setArgument(newOperands, numberOfOperands, cloneOperands);
  return c;
}

template<typename T>
Complex<T> Conjugate::templatedComputeComplex(const Complex<T> c) const {
  return c.conjugate();
}

ExpressionLayout * Conjugate::privateCreateLayout(FloatDisplayMode floatDisplayMode, ComplexFormat complexFormat) const {
  assert(floatDisplayMode != FloatDisplayMode::Default);
  assert(complexFormat != ComplexFormat::Default);
  return new ConjugateLayout(m_args[0]->createLayout(floatDisplayMode, complexFormat));
}

}

