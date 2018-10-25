#ifndef REGRESSION_QUARTIC_MODEL_H
#define REGRESSION_QUARTIC_MODEL_H

#include "model.h"

namespace Regression {

class QuarticModel : public Model {
public:
  using Model::Model;
  Poincare::Layout layout() override;
  Poincare::Expression simplifiedExpression(double * modelCoefficients, Poincare::Context * context) override;
  I18n::Message formulaMessage() const override { return I18n::Message::QuarticRegressionFormula; }
  double evaluate(double * modelCoefficients, double x) const override;
  double partialDerivate(double * modelCoefficients, int derivateCoefficientIndex, double x) const override;
  int numberOfCoefficients() const override { return 5; }
  int bannerLinesCount() const override { return 4; }
};

}


#endif
