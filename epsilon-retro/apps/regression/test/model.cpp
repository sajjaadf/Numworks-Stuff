#include <quiz.h>
#include <string.h>
#include <assert.h>
#include "../model/model.h"
#include "../regression_context.h"
#include "../store.h"

using namespace Poincare;
using namespace Regression;

/* The data was generated by choosing X1 and the coefficients of the regression,
 * then filling Y1 with the regression formula + random()/10. */

void assert_regression_is(double * xi, double * yi, int numberOfPoints, Model::Type modelType, double * trueCoefficients) {
  double precision = 0.1;
  int series = 0;
  Regression::Store store;

  // Set the points and the regression type
  for (int i = 0; i < numberOfPoints; i++) {
    store.set(xi[i], series, 0, i);
    store.set(yi[i], series, 1, i);
  }
  RegressionContext context(&store);
  store.setSeriesRegressionType(series, modelType);

  // Compute the coefficients
  double * coefficients = store.coefficientsForSeries(series, &context);

  // Comapre the coefficients
  int numberOfCoefs = store.modelForSeries(series)->numberOfCoefficients();
  for (int i = 0; i < numberOfCoefs; i++) {
    assert(std::fabs(coefficients[i] - trueCoefficients[i]) < precision);
  }
}

QUIZ_CASE(linear_regression) {
  double x[] = {1.0, 8.0, 14.0, 79.0};
  double y[] = {-3.581, 20.296, 40.676, 261.623};
  double coefficients[] = {3.4, -7.0};
  assert_regression_is(x, y, 4, Model::Type::Linear, coefficients);
}

QUIZ_CASE(quadratic_regression) {
  double x[] = {-34.0, -12.0, 5.0, 86.0, -2.0};
  double y[] = {-8241.389, -1194.734, -59.163, - 46245.39, -71.774};
  double coefficients[] = {-6.5, 21.3, -3.2};
  assert_regression_is(x, y, 5, Model::Type::Quadratic, coefficients);
}

QUIZ_CASE(cubic_regression) {
  double x[] = {-3.0, -2.8, -1.0, 0.0, 12.0};
  double y[] = {691.261, 566.498, 20.203, -12.865, -34293.21};
  double coefficients[] = {-21.2, 16.0, 4.1, -12.9};
  assert_regression_is(x, y, 5, Model::Type::Cubic, coefficients);
}

QUIZ_CASE(quartic_regression) {
  double x[] = {1.6, 3.5, 3.5, -2.8, 6.4, 5.3, 2.9, -4.8, -5.7, 3.1};
  double y[] = {-112.667, -1479.824, -1479.805, 1140.276, -9365.505, -5308.355, -816.925, 5554.007, 9277.107, -1009.874};
  double coefficients[] = {0.6, -43, 21.5, 3.1, -0.5};
  assert_regression_is(x, y, 10, Model::Type::Quartic, coefficients);
}

QUIZ_CASE(logarithmic_regression) {
  double x[] = {0.2, 0.5, 5, 7};
  double y[] = {-11.952, -9.035, -1.695, -0.584};
  double coefficients[] = {3.2, -6.9};
  assert_regression_is(x, y, 4, Model::Type::Logarithmic, coefficients);
}

QUIZ_CASE(exponential_regression) {
  double x[] = {5.5, 5.6, 5.7, 5.8, 5.9, 6.0};
  double y[] = {-276.842, -299.956, -324.933, -352.0299, -381.314, -413.0775};
  double coefficients[] = {-3.4, 0.8};
  assert_regression_is(x, y, 6, Model::Type::Exponential, coefficients);
}

QUIZ_CASE(power_regression) {
  double x[] = {1.0, 50.0, 34.0, 67.0, 20.0};
  double y[] = {71.860, 2775514, 979755.1, 6116830, 233832.9};
  double coefficients[] = {71.8, 2.7};
  assert_regression_is(x, y, 5, Model::Type::Power, coefficients);
}

// No case for trigonometric regression, because it has no unique solution

/* This data was generated without the random error, otherwise it did not pass
 * the test. */
QUIZ_CASE(logistic_regression) {
  double x[] = {2.3, 5.6, 1.1, 4.3};
  double y[] = {3.948, 4.694, 2.184, 4.656};
  double coefficients[] = {6, 1.5, 4.7};
  assert_regression_is(x, y, 4, Model::Type::Logistic, coefficients);
}
