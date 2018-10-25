#ifndef POINCARE_CONTEXT_H
#define POINCARE_CONTEXT_H

#include <poincare/expression.h>
#include <poincare/symbol.h>

namespace Poincare {

class Context {
public:
  virtual const Expression * expressionForSymbol(const Symbol * symbol) = 0;
  virtual void setExpressionForSymbolName(Expression * expression, const Symbol * symbol) = 0;
};

}

#endif
