#include "element_type.h"

namespace PeriodicTable {

class ScientificElement {
public:
  ScientificElement(int atomicNumber, const char * name, const char * symbol, float atomicMass, ElementType type) {
    m_atomicNumber = atomicNumber;
    m_name = name;
    m_symbol = symbol;
    m_atomicMass = atomicMass;
    m_type = type;
  };
  const int atomicNumber() { return m_atomicNumber; }
  const char * name() { return m_name; }
  const char * symbol() { return m_symbol; }
  const float atomicMass() { return m_atomicMass; }
  const ElementType type() { return m_type; }
private:
  int m_atomicNumber;
  const char * m_name;
  const char * m_symbol;
  float m_atomicMass;
  ElementType m_type;
};

}