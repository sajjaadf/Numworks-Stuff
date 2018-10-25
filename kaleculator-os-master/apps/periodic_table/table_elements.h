#include "scientific_element.h"

namespace PeriodicTable {

class TableElements {
public:
  const ScientificElement elementAtPosition(int pos);
private:
  static constexpr int k_numberOfElements = 118;
};

}