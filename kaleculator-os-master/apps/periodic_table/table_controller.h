#ifndef PERIODIC_TABLE_TABLE_CONTROLLER_H
#define PERIODIC_TABLE_TABLE_CONTROLLER_H

#include <escher.h>

namespace PeriodicTable {

class PicViewController : public ViewController {
public:
  PicViewController(Responder * parentResponder);
  View * view() override;
// private:
  // PicView m_view;
};

}

#endif

