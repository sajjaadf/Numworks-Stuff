#ifndef GRAPH_PARAMETRIC_FUNCTION_STORE_H
#define GRAPH_PARAMETRIC_FUNCTION_STORE_H

#include "parametric_function.h"
#include "../shared/function_store.h"
#include <stdint.h>
#include <escher.h>

namespace Parametric {

class ParametricFunctionStore : public Shared::FunctionStore {
  //Q: given that parametric plots need two functions, can we get away with using shared::functionstore?
public:
  ParametricFunctionStore();
  uint32_t storeChecksum() override;
  ParametricFunction * modelAtIndex(int i) override { return &m_functions[i]; }
  ParametricFunction * activeFunctionAtIndex(int i) override { return (ParametricFunction *)Shared::FunctionStore::activeFunctionAtIndex(i); }
  ParametricFunction * definedFunctionAtIndex(int i) override { return (ParametricFunction *)Shared::FunctionStore::definedFunctionAtIndex(i); }
  int maxNumberOfModels() const override {
    return k_maxNumberOfFunctions;
  }
  char symbol() const override; // "x" as the dependent variable?  Change to "t"?
  void removeAll() override;
  static constexpr int k_maxNumberOfFunctions = 4;
private:
  static constexpr const char * k_functionNames[k_maxNumberOfFunctions = {
      "c", "q", "r", "s",
  };
  ParametricFunction * emptyModel() override;
  ParametricFunction * nullModel() override;
  void setModelAtIndex(Shared::ExpressionModel * f, int i) override; // ExpressionModel OK?  use two ExpressionModel args?
  const char * firstAvailableName() override {
    return firstAvailableAttribute(k_functionNames, FunctionStore::name);
  }
  ParametricFunction m_functions[k_maxNumberOfFunctions];
};
  
}

#endif
