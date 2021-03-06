#include "store_controller.h"
#include "statistics_context.h"
#include "app.h"
#include "../apps_container.h"
#include "../constant.h"
#include <assert.h>
#include <float.h>
#include <cmath>

using namespace Poincare;
using namespace Shared;

namespace Statistics {

StoreController::StoreController(Responder * parentResponder, Store * store, ButtonRowController * header) :
  Shared::StoreController(parentResponder, store, header),
  m_titleCells{},
  m_store(store),
  m_statisticsContext(m_store),
  m_storeParameterController(this, store, this)
{
}

StoreContext * StoreController::storeContext() {
  m_statisticsContext.setParentContext(const_cast<AppsContainer *>(static_cast<const AppsContainer *>(app()->container()))->globalContext());
  return &m_statisticsContext;
}

void StoreController::setFormulaLabel() {
  int series = selectedColumn() / Store::k_numberOfColumnsPerSeries;
  int isValueColumn = selectedColumn() % Store::k_numberOfColumnsPerSeries == 0;
  char text[] = {isValueColumn ? 'V' : 'N', static_cast<char>('1' + series), '=', 0};
  static_cast<ContentView *>(view())->formulaInputView()->setBufferText(text);
}

bool StoreController::fillColumnWithFormula(Expression * formula) {
  return privateFillColumnWithFormula(formula, Symbol::isSeriesSymbol);
}

void StoreController::willDisplayCellAtLocation(HighlightCell * cell, int i, int j) {
  Shared::StoreController::willDisplayCellAtLocation(cell, i, j);
  if (cellAtLocationIsEditable(i, j)) {
    return;
  }
  Shared::StoreTitleCell * mytitleCell = static_cast<Shared::StoreTitleCell *>(cell);
  bool isValuesColumn = i%Store::k_numberOfColumnsPerSeries == 0;
  mytitleCell->setSeparatorLeft(isValuesColumn);
  int seriesIndex = i/Store::k_numberOfColumnsPerSeries;
  assert(seriesIndex >= 0 && seriesIndex < DoublePairStore::k_numberOfSeries);
  if (isValuesColumn) {
    I18n::Message valuesMessages[] = {I18n::Message::Values1, I18n::Message::Values2, I18n::Message::Values3};
    mytitleCell->setText(I18n::translate(valuesMessages[seriesIndex]));
  } else {
    I18n::Message sizesMessages[] = {I18n::Message::Sizes1, I18n::Message::Sizes2, I18n::Message::Sizes3};
    mytitleCell->setText(I18n::translate(sizesMessages[seriesIndex]));
  }
  mytitleCell->setColor(m_store->numberOfPairsOfSeries(seriesIndex) == 0 ? Palette::GreyDark : Store::colorOfSeriesAtIndex(seriesIndex)); // TODO Share GreyDark with graph/list_controller
}

HighlightCell * StoreController::titleCells(int index) {
  assert(index >= 0 && index < k_numberOfTitleCells);
  return m_titleCells[index];
}

bool StoreController::setDataAtLocation(double floatBody, int columnIndex, int rowIndex) {
  if (std::fabs(floatBody) > FLT_MAX) {
    return false;
  }
  if (columnIndex % Store::k_numberOfColumnsPerSeries == 1) {
    if (floatBody < 0) {
      return false;
    }
  }
  return Shared::StoreController::setDataAtLocation(floatBody, columnIndex, rowIndex);
}

View * StoreController::loadView() {
  for (int i = 0; i < k_numberOfTitleCells; i++) {
    m_titleCells[i] = new Shared::StoreTitleCell();
  }
  return Shared::StoreController::loadView();
}

void StoreController::unloadView(View * view) {
  for (int i = 0; i < k_numberOfTitleCells; i++) {
    delete m_titleCells[i];
    m_titleCells[i] = nullptr;
  }
  Shared::StoreController::unloadView(view);
}

}
