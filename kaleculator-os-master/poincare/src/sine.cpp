#include <poincare/sine.h>
#include <poincare/hyperbolic_sine.h>
#include <poincare/complex.h>
#include <poincare/multiplication.h>
extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

Sine::Sine() :
  Function("sin")
{
}

Expression::Type Sine::type() const {
  return Expression::Type::Sine;
}

Expression * Sine::cloneWithDifferentOperands(Expression** newOperands,
    int numberOfOperands, bool cloneOperands) const {
  assert(newOperands != nullptr);
  Sine * s = new Sine();
  s->setArgument(newOperands, numberOfOperands, cloneOperands);
  return s;
}

template<typename T>
Complex<T> Sine::compute(const Complex<T> c, AngleUnit angleUnit) {
  if (c.b() == 0) {
    T input = c.a();
    if (angleUnit == AngleUnit::Degree) {
      input *= M_PI/180;
    }
    T result = std::sin(input);
    /* Cheat: see comment in cosine.cpp
     * We cheat to avoid returning sin(Pi) = epsilon */
    if (input !=  0 && std::fabs(result/input) <= epsilon<T>()) {
      return Complex<T>::Float(0);
    }
    return Complex<T>::Float(result);
  }
  Complex<T> arg = Complex<T>::Cartesian(-c.b(), c.a());
  Complex<T> sinh = HyperbolicSine::compute(arg);
  return Multiplication::compute(Complex<T>::Cartesian(0, -1), sinh);
}

}
