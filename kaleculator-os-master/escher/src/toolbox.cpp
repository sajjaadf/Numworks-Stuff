#include <escher/toolbox.h>

Toolbox::Toolbox(Responder * parentResponder, ViewController * rootViewController) :
  StackViewController(parentResponder, rootViewController, true, KDColorWhite, Palette::PurpleBright, Palette::PurpleDark),
  m_sender(nullptr)
{
}

void Toolbox::setSender(Responder * sender) {
  m_sender = sender;
}

void Toolbox::viewWillAppear() {
  StackViewController::viewWillAppear();
}

void Toolbox::viewDidDisappear() {
  StackViewController::viewDidDisappear();
}

Responder * Toolbox::sender() {
  return m_sender;
}
