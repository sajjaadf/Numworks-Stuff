#include "hello_controller.h"
#include <assert.h>

namespace Helloworld {

HelloController::HelloController(Responder * parentResponder) :
  ViewController(parentResponder)
{
}

View * HelloController::view() {
  return &m_helloView;
}

void HelloController::didBecomeFirstResponder() {
}

bool HelloController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::OK || event == Ion::Events::EXE) {
    m_helloView.changeColor();
    return true;
  }
  return false;
}

}
