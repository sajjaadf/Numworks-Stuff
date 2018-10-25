#include "app.h"
#include "periodic_table_icon.h"
#include "../i18n.h"

namespace PeriodicTable {

I18n::Message App::Descriptor::name() {
  return I18n::Message::PeriodicTableApp;
}

I18n::Message App::Descriptor::upperName() {
  return I18n::Message::PeriodicTableAppCapital;
}

const Image * App::Descriptor::icon() {
  return ImageStore::PeriodicTableIcon;
}

App * App::Snapshot::unpack(Container * container) {
  return new App(container, this);
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

App::App(Container * container) :
  ::App(container),
{
}

}
