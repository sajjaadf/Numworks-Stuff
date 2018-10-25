#include <poincare/floor.h>

extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

Floor::Floor() :
  Function("floor")
{
}

Expression::Type Floor::type() const {
  return Type::Floor;
}

Expression * Floor::cloneWithDifferentOperands(Expression** newOperands,
        int numberOfOperands, bool cloneOperands) const {
  assert(newOperands != nullptr);
  Floor * f = new Floor();
  f->setArgument(newOperands, numberOfOperands, cloneOperands);
  return f;
}

template<typename T>
Complex<T> Floor::templatedComputeComplex(const Complex<T> c) const {
  if (c.b() != 0) {
    return Complex<T>::Float(NAN);
  }
  return Complex<T>::Float(std::floor(c.a()));
}

}


