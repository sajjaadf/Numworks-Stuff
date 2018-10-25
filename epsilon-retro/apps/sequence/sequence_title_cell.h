#ifndef SEQUENCE_SEQUENCE_TITLE_CELL_H
#define SEQUENCE_SEQUENCE_TITLE_CELL_H

#include "../shared/function_title_cell.h"

namespace Sequence {

class SequenceTitleCell : public Shared::FunctionTitleCell {
public:
  SequenceTitleCell(Orientation orientation);
  void setExpressionLayout(Poincare::ExpressionLayout * expressionLayout);
  void setEven(bool even) override;
  void setHighlighted(bool highlight) override;
  void setColor(KDColor color) override;
  Poincare::ExpressionLayout * expressionLayout() const override {
    return m_titleTextView.expressionLayout();
  }
private:
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  void layoutSubviews() override;
  EvenOddExpressionCell m_titleTextView;
};

}

#endif
