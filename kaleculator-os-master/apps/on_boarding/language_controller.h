#ifndef ON_BOARDING_LANGUAGE_CONTROLLER_H
#define ON_BOARDING_LANGUAGE_CONTROLLER_H

#include <escher.h>
#include "../i18n.h"
#include "logo_controller.h"
#include "update_controller.h"

namespace OnBoarding {

class LanguageController : public ViewController, public SimpleListViewDataSource, public SelectableTableViewDataSource {
public:
  LanguageController(Responder * parentResponder, LogoController * logoController, UpdateController * updateController);
  View * view() override;
  void reinitOnBoarding();
  void didBecomeFirstResponder() override;
  bool handleEvent(Ion::Events::Event event) override;

  int numberOfRows() override;
  KDCoordinate cellHeight() override;
  HighlightCell * reusableCell(int index) override;
  int reusableCellCount() override;

  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
private:
  LogoController * m_logoController;
#if OS_WITH_SOFTWARE_UPDATE_PROMPT
  UpdateController * m_updateController;
#endif
  SelectableTableView m_selectableTableView;
  MessageTableCell m_cells[I18n::NumberOfLanguages];
};

}

#endif
