#include "table_controller.h"

namespace PeriodicTable {

TableController::TableController(Responder * parentResponder) :
  ViewController(parentResponder),
  m_view(PicView())
{
}

View * TableController::view() {
  return &m_view;
}

}