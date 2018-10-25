#ifndef REGS_REGISTER_H
#define REGS_REGISTER_H

#include <stdint.h>

template <typename T>
class Register {
public:
  Register() = delete;
  Register(T v) : m_value(v) {}
  void set(Register<T> value) volatile {
    m_value = value.m_value;
  }
  void set(T value) volatile {
    m_value = value;
  }
  T get() volatile {
    return m_value;
  }
  void setBitRange(uint8_t high, uint8_t low, T value) volatile {
    m_value = bit_range_set_value(high, low, m_value, value);
  }
  T getBitRange(uint8_t high, uint8_t low) volatile {
    return (m_value & bit_range_mask(high,low)) >> low;
  }
protected:
  static constexpr T bit_range_mask(uint8_t high, uint8_t low) {
    return ((((T)1)<<(high-low+1))-1)<<low;
  }
  static constexpr T bit_range_value(T value, uint8_t high, uint8_t low) {
    return (value<<low) & bit_range_mask(high,low);
  }
  static constexpr T bit_range_set_value(uint8_t high, uint8_t low, T originalValue, T targetValue) {
    return (originalValue & ~bit_range_mask(high,low))|bit_range_value(targetValue, high, low);
  }
private:
  T m_value;
};

typedef Register<uint8_t> Register8;
typedef Register<uint16_t> Register16;
typedef Register<uint32_t> Register32;
typedef Register<uint64_t> Register64;

#define REGS_FIELD_R(name,type,high,low) type get##name() volatile { return (type)getBitRange(high,low); };
#define REGS_FIELD_W(name,type,high,low) void set##name(type v) volatile { setBitRange(high, low, (uint8_t)v); };
#define REGS_FIELD(name,type,high,low) REGS_FIELD_R(name,type,high,low); REGS_FIELD_W(name,type,high,low);
#define REGS_TYPE_FIELD(name,high,low) REGS_FIELD(name,name,high,low)
#define REGS_BOOL_FIELD(name,bit) REGS_FIELD(name,bool,bit,bit)
#define REGS_BOOL_FIELD_R(name,bit) REGS_FIELD_R(name,bool,bit,bit)
#define REGS_BOOL_FIELD_W(name,bit) REGS_FIELD_W(name,bool,bit,bit)
#define REGS_REGISTER_AT(name, offset) volatile name * name() const { return (class name *)(Base() + offset); };

#endif
