#ifndef REGS_FLASH_H
#define REGS_FLASH_H

#include "register.h"

class FLASH {
public:
  class ACR : public Register32 {
  public:
    REGS_FIELD(LATENCY, uint8_t, 3, 0);
  };

  constexpr FLASH() {};
  REGS_REGISTER_AT(ACR, 0x00);
private:
  constexpr uint32_t Base() const {
    return 0x40023C00;
  }
};

constexpr FLASH FLASH;

#endif
