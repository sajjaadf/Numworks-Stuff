#ifndef PERIODIC_TABLE_APP_H
#define PERIODIC_TABLE_APP_H

#include <escher.h>
// #include "main_controller.h"

namespace PeriodicTable {

class App : public ::App {
public:
  class Descriptor : public ::App::Descriptor {
  public:
    I18n::Message name() override;
    I18n::Message upperName() override;
    const Image * icon() override;
  };
  class Snapshot : public ::App::Snapshot {
  public:
    App * unpack(Container * container) override;
    Descriptor * descriptor() override;
  };
private:
  App(Container * container, Snapshot * snapshot);
};

}

#endif
