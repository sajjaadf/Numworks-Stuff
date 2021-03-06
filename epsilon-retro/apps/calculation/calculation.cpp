#include "calculation.h"
#include "calculation_store.h"
#include "../shared/poincare_helpers.h"
#include <string.h>
#include <cmath>

using namespace Poincare;
using namespace Shared;

namespace Calculation {

Calculation::Calculation() :
  m_inputText(),
  m_exactOutputText(),
  m_approximateOutputText(),
  m_input(nullptr),
  m_exactOutput(nullptr),
  m_approximateOutput(nullptr),
  m_height(-1),
  m_equalSign(EqualSign::Unknown)
{
}

Calculation::~Calculation() {
  if (m_input != nullptr) {
    delete m_input;
    m_input = nullptr;
  }
  if (m_exactOutput != nullptr) {
    delete m_exactOutput;
    m_exactOutput = nullptr;
  }
  if (m_approximateOutput != nullptr) {
    delete m_approximateOutput;
    m_approximateOutput = nullptr;
  }
}

Calculation& Calculation::operator=(const Calculation& other) {
  const char * otherInputText = other.m_inputText;
  const char * otherExactOutputText = other.m_exactOutputText;
  const char * otherApproximateOutputText = other.m_approximateOutputText;
  reset();
  strlcpy(m_inputText, otherInputText, sizeof(m_inputText));
  strlcpy(m_exactOutputText, otherExactOutputText, sizeof(m_exactOutputText));
  strlcpy(m_approximateOutputText, otherApproximateOutputText, sizeof(m_approximateOutputText));
  return *this;
}

void Calculation::reset() {
  m_inputText[0] = 0;
  m_exactOutputText[0] = 0;
  m_approximateOutputText[0] = 0;
  tidy();
}

void Calculation::setContent(const char * c, Context * context, Expression * ansExpression) {
  reset();
  m_input = Expression::parse(c);
  Expression::ReplaceSymbolWithExpression(&m_input, Symbol::SpecialSymbols::Ans, ansExpression);
  /* We do not store directly the text enter by the user because we do not want
   * to keep Ans symbol in the calculation store. */
  PoincareHelpers::WriteTextInBuffer(m_input, m_inputText, sizeof(m_inputText));
  m_exactOutput = PoincareHelpers::ParseAndSimplify(m_inputText, *context);
  PoincareHelpers::WriteTextInBuffer(m_exactOutput, m_exactOutputText, sizeof(m_exactOutputText));
  m_approximateOutput = PoincareHelpers::Approximate<double>(m_exactOutput, *context);
  PoincareHelpers::WriteTextInBuffer(m_approximateOutput, m_approximateOutputText, sizeof(m_approximateOutputText));
}

KDCoordinate Calculation::height(Context * context) {
  if (m_height < 0) {
    ExpressionLayout * inputLayout = createInputLayout();
    KDCoordinate inputHeight = inputLayout->size().height();
    delete inputLayout;
    Poincare::ExpressionLayout * approximateLayout = createApproximateOutputLayout(context);
    KDCoordinate approximateOutputHeight = approximateLayout->size().height();
    if (shouldOnlyDisplayApproximateOutput(context)) {
      m_height = inputHeight+approximateOutputHeight;
    } else {
      Poincare::ExpressionLayout * exactLayout = createExactOutputLayout(context);
      KDCoordinate exactOutputHeight = exactLayout->size().height();
      KDCoordinate outputHeight = max(exactLayout->baseline(), approximateLayout->baseline()) + max(exactOutputHeight-exactLayout->baseline(), approximateOutputHeight-approximateLayout->baseline());
      delete exactLayout;
      m_height = inputHeight + outputHeight;
    }
    delete approximateLayout;
  }
  return m_height;
}

const char * Calculation::inputText() {
  return m_inputText;
}

const char * Calculation::exactOutputText() {
  return m_exactOutputText;
}

const char * Calculation::approximateOutputText() {
  return m_approximateOutputText;
}

Expression * Calculation::input() {
  if (m_input == nullptr) {
    m_input = Expression::parse(m_inputText);
  }
  return m_input;
}

ExpressionLayout * Calculation::createInputLayout() {
  if (input() != nullptr) {
    return input()->createLayout(PrintFloat::Mode::Decimal, PrintFloat::k_numberOfStoredSignificantDigits);
  }
  return nullptr;
}

bool Calculation::isEmpty() {
  /* To test if a calculation is empty, we need to test either m_inputText or
   * m_exactOutputText or m_approximateOutputText, the only three fields that
   * are not lazy-loaded. We choose m_exactOutputText to consider that a
   * calculation being added is still empty until the end of the method
   * 'setContent'. Indeed, during 'setContent' method, 'ans' evaluation calls
   * the evaluation of the last calculation only if the calculation being
   * filled is not taken into account.*/
  if (strlen(m_approximateOutputText) == 0) {
    return true;
  }
  return false;
}

void Calculation::tidy() {
  if (m_input != nullptr) {
    delete m_input;
  }
  m_input = nullptr;
  if (m_exactOutput != nullptr) {
    delete m_exactOutput;
  }
  m_exactOutput = nullptr;
  if (m_approximateOutput != nullptr) {
    delete m_approximateOutput;
  }
  m_approximateOutput = nullptr;
  m_height = -1;
  m_equalSign = EqualSign::Unknown;
}

Expression * Calculation::exactOutput(Context * context) {
  if (m_exactOutput == nullptr) {
    /* Because the angle unit might have changed, we do not simplify again. We
     * thereby avoid turning cos(Pi/4) into sqrt(2)/2 and displaying
     * 'sqrt(2)/2 = 0.999906' (which is totally wrong) instead of
     * 'cos(pi/4) = 0.999906' (which is true in degree). */
    m_exactOutput = Expression::parse(m_exactOutputText);
    if (m_exactOutput == nullptr) {
      m_exactOutput = new Undefined();
    }
  }
  return m_exactOutput;
}

ExpressionLayout * Calculation::createExactOutputLayout(Context * context) {
  if (exactOutput(context) != nullptr) {
    return PoincareHelpers::CreateLayout(exactOutput(context));
  }
  return nullptr;
}

Expression * Calculation::approximateOutput(Context * context) {
  if (m_approximateOutput == nullptr) {
    /* To ensure that the expression 'm_output' is a matrix or a complex, we
     * call 'evaluate'. */
    Expression * exp = Expression::parse(m_approximateOutputText);
    if (exp != nullptr) {
      m_approximateOutput = PoincareHelpers::Approximate<double>(exp, *context);
      delete exp;
    } else {
      m_approximateOutput = new Undefined();
    }
  }
  return m_approximateOutput;
}

ExpressionLayout * Calculation::createApproximateOutputLayout(Context * context) {
  if (approximateOutput(context) != nullptr) {
    return PoincareHelpers::CreateLayout(approximateOutput(context));
  }
  return nullptr;
}

bool Calculation::shouldOnlyDisplayApproximateOutput(Context * context) {
  if (strcmp(m_exactOutputText, m_approximateOutputText) == 0) {
    return true;
  }
  if (strcmp(m_exactOutputText, "undef") == 0) {
    return true;
  }
  return input()->isApproximate(*context);
}

Calculation::EqualSign Calculation::exactAndApproximateDisplayedOutputsAreEqual(Poincare::Context * context) {
  if (m_equalSign != EqualSign::Unknown) {
    return m_equalSign;
  }
  m_equalSign = exactOutput(context)->isEqualToItsApproximationLayout(approximateOutput(context), k_printedExpressionSize, Preferences::sharedPreferences()->angleUnit(), Preferences::sharedPreferences()->displayMode(), Preferences::sharedPreferences()->numberOfSignificantDigits(), *context) ? EqualSign::Equal : EqualSign::Approximation;
  return m_equalSign;
}

}
