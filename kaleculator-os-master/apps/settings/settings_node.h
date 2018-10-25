#ifndef SETTINGS_NODE_H
#define SETTINGS_NODE_H

#include "../node.h"

namespace Settings {

class SettingsNode : public Node {
public:
  constexpr SettingsNode(I18n::Message label = I18n::Message::Default, const SettingsNode * children = nullptr, int numberOfChildren = 0) :
    Node(label, numberOfChildren),
    m_children(children)
  {
  };
  const Node * children(int index) const override;
private:
  const SettingsNode * m_children;
};

}

#endif

