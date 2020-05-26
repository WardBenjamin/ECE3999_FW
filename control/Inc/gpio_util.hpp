
#pragma once

namespace GPIO {

  struct pindef {
    GPIO_TypeDef *const port;
    const u16 pin;
  };

  enum class pin_state { HIGH = true, LOW = false };
  static inline void write_pin(const pindef &pin, pin_state state) {
    if (state == pin_state::HIGH) // high
      pin.port->BSRR = (uint32_t) pin.pin;
    else  // low
      pin.port->BRR = (uint32_t) pin.pin;
  }
  static inline pin_state read_pin(const pindef &pin) {
    if ((pin.port->IDR & pin.pin) == 0u)
      return pin_state::LOW;
    else
      return pin_state::HIGH;
  }
}