#ifndef APPS_MATH_TOOLBOX_H
#define APPS_MATH_TOOLBOX_H

#define MATRICES_ARE_DEFINED 1
#include <escher.h>
#include "toolbox_node.h"

/* m_nodeModel points at the node of the tree (describing the whole model)
 * where we are located. It enables to know which rows are leaves and which are
 * subtrees. */

class MathToolbox : public Toolbox, public ListViewDataSource, public SelectableTableViewDataSource {
public:
  MathToolbox();
  void didBecomeFirstResponder() override;
  bool handleEvent(Ion::Events::Event event) override;

  int numberOfRows() override;
  HighlightCell * reusableCell(int index, int type) override;
  int reusableCellCount(int type) override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
  KDCoordinate rowHeight(int j) override;
  KDCoordinate cumulatedHeightFromIndex(int j) override;
  int indexFromCumulatedHeight(KDCoordinate offsetY) override;
  int typeAtLocation(int i, int j) override;

  void viewWillAppear() override;
  void viewDidDisappear() override;
protected:
  int stackDepth();
  TextField * sender() override;
  bool handleEventForRow(Ion::Events::Event event, int selectedRow);
  SelectableTableView m_selectableTableView;
  constexpr static int k_maxNumberOfDisplayedRows = 6; //240/40
private:
  class Stack {
  public:
    class State {
    public:
      State(int selectedRow = -1, KDCoordinate verticalScroll = 0);
      bool isNull();
      int selectedRow();
      KDCoordinate verticalScroll();
    private:
      int m_selectedRow;
      KDCoordinate m_verticalScroll;
    };
    void push(int selectedRow, KDCoordinate verticalScroll);
    void pop();
    State * stateAtIndex(int index);
    int depth();
    void resetStack();
  private:
    constexpr static int k_maxModelTreeDepth = 2;
    State m_statesStack[k_maxModelTreeDepth];
  };
  class ListController : public ViewController {
  public:
    ListController(Responder * parentResponder, SelectableTableView * tableView);
    const char * title() override;
    View * view() override;
    void didBecomeFirstResponder() override;
    void setFirstSelectedRow(int firstSelectedRow);
  private:
    SelectableTableView * m_selectableTableView;
    int m_firstSelectedRow;
  };
  constexpr static KDCoordinate k_nodeRowHeight = 40;
  constexpr static KDCoordinate k_leafRowHeight = 40;
  const ToolboxNode * rootModel();
  bool selectLeaf(ToolboxNode * selectedNode);
  bool selectSubMenu(ToolboxNode * selectedNode);
  bool returnToPreviousMenu();
  Stack m_stack;
  MessageTableCellWithMessage m_leafCells[k_maxNumberOfDisplayedRows];
  MessageTableCellWithChevron m_nodeCells[k_maxNumberOfDisplayedRows];
  ListController m_listController;
  ToolboxNode * m_nodeModel;
};

#endif
