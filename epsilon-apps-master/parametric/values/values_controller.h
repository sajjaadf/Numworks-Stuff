#ifndef PARAMETRIC_VALUES_CONTROLLER_H
#define PARAMETRIC_VALUES_CONTROLLER_H

#include "../parametric_function_store.h"
#include "../../shared/buffer_function_title_cell.h"
#include "../../shared/values_controller.h"
#include "../../shared/interval_parameter_controller.h"
#include "derivative_parameter_controller.h"
#include "function_parameter_controller.h"

namespace Parametric {

class ValuesController : public Shared::ValuesController {
public:
  ValuesController(Responder * parentResponder, ParametricFunctionStore * functionStore, Shared::Interval * interval, ButtonRowController * header);
  bool handleEvent(Ion::Events::Event event) override;
  void willDisplayCellAtLocation(HighlightCell * cell, int i, int j) override;
  I18n::Message emptyMessage() override;
  Shared::IntervalParameterController * intervalParameterController() override;
  void updateNumberOfColumns() override;
private:
  ParametricFunction * functionAtColumn(int i) override;
  bool isDerivativeColumn(int i);
  void configureDerivativeFunction();
  int maxNumberOfCells() override;
  int maxNumberOfFunctions() override;
  double evaluationOfAbscissaAtColumn(double abscissa, int columnIndex) override;
  constexpr static int k_maxNumberOfCells = 50;
  constexpr static int k_maxNumberOfFunctions = 5;
  Shared::BufferFunctionTitleCell * m_functionTitleCells[k_maxNumberOfFunctions];
  Shared::BufferFunctionTitleCell * functionTitleCells(int j) override;
  ParametricFunctionStore * m_functionStore;
  ParametricFunctionStore * functionStore() const override;
  FunctionParameterController m_functionParameterController;
  FunctionParameterController * functionParameterController() override;
  View * loadView() override;
  void unloadView(View * view) override;
  Shared::IntervalParameterController m_intervalParameterController;
  DerivativeParameterController m_derivativeParameterController;
};

}

#endif
