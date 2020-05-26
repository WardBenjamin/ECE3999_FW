#pragma once
#include "gpio_util.hpp"
#include "mappings.hpp"

namespace a4928 {
  static inline void set_enable(bool en) {
    GPIO::write_pin(map::GPIO::brake::enable,(pin_state) en);
  }

  static inline void enable_reset(bool en) {
    GPIO::write_pin(map::GPIO::brake::reset,(pin_state) !en);
  }
}

