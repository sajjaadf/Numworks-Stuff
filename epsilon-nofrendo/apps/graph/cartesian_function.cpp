#include "cartesian_function.h"
#include "../shared/poincare_helpers.h"
#include <float.h>
#include <cmath>

using namespace Poincare;
using namespace Shared;

namespace Graph {

CartesianFunction::CartesianFunction(const char * text, KDColor color) :
  Shared::Function(text, color),
  m_displayDerivative(false)
{
}

bool CartesianFunction::displayDerivative() {
  return m_displayDerivative;
}

void CartesianFunction::setDisplayDerivative(bool display) {
  m_displayDerivative = display;
}

double CartesianFunction::approximateDerivative(double x, Poincare::Context * context) const {
  Poincare::Expression * abscissa = new Poincare::Approximation<double>(x);
  Poincare::Expression * args[2] = {expression(context)->clone(), abscissa};
  Poincare::Derivative derivative(args, false); // derivative takes ownership of abscissa and the clone of expression
  /* TODO: when we will simplify derivative, we might want to simplify the
   * derivative here. However, we might want to do it once for all x (to avoid
   * lagging in the derivative table. */
  return PoincareHelpers::ApproximateToScalar<double>(&derivative, *context);
}

double CartesianFunction::sumBetweenBounds(double start, double end, Poincare::Context * context) const {
  Poincare::Expression * x = new Poincare::Approximation<double>(start);
  Poincare::Expression * y = new Poincare::Approximation<double>(end);
  Poincare::Expression * args[3] = {expression(context)->clone(), x, y};
  Poincare::Integral integral(args, false); // Integral takes ownership of args
  /* TODO: when we will simplify integral, we might want to simplify the
   * integral here. However, we might want to do it once for all x (to avoid
   * lagging in the derivative table. */
  return PoincareHelpers::ApproximateToScalar<double>(&integral, *context);
}

Expression::Coordinate2D CartesianFunction::nextMinimumFrom(double start, double step, double max, Context * context) const {
  return expression(context)->nextMinimum(symbol(), start, step, max, *context, Preferences::sharedPreferences()->angleUnit());
}

Expression::Coordinate2D CartesianFunction::nextMaximumFrom(double start, double step, double max, Context * context) const {
  return expression(context)->nextMaximum(symbol(), start, step, max, *context, Preferences::sharedPreferences()->angleUnit());
}

double CartesianFunction::nextRootFrom(double start, double step, double max, Context * context) const {
  return expression(context)->nextRoot(symbol(), start, step, max, *context, Preferences::sharedPreferences()->angleUnit());
}

Expression::Coordinate2D CartesianFunction::nextIntersectionFrom(double start, double step, double max, Poincare::Context * context, const Shared::Function * function) const {
  return expression(context)->nextIntersection(symbol(), start, step, max, *context, Preferences::sharedPreferences()->angleUnit(), function->expression(context));
}

char CartesianFunction::symbol() const {
  return 'x';
}

}
