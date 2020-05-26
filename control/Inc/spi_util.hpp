//
// Created by solomon on 4/17/20.
//

#pragma once
#include "spi.h"
#include "gpio_util.hpp"
#include "mappings.hpp"

namespace SPI {
  namespace POWER {

    static inline void select(bool inverter, bool adc, bool brake) {
      GPIO::write_pin(map::SPI::POWER::inverter::CS, inverter ? pin_state::LOW : pin_state::HIGH);
      GPIO::write_pin(map::SPI::POWER::ADC::CS, adc ? pin_state::LOW : pin_state::HIGH);
      GPIO::write_pin(map::SPI::POWER::brake::CS, brake ? pin_state::LOW : pin_state::HIGH);
    }

    static inline void select_inverter() { select(true, false, false); }
    static inline void select_ADC() { select(false, true, false); }
    static inline void select_brake() { select(false, false, true); }
    static inline void select_none() { select(false, false, false); }

    enum class DEVICES { INVERTER, ADC, BRAKE, NONE };

    // TODO make actually good
    static inline void select_device(DEVICES E) {
      switch (E) {
      case DEVICES::INVERTER: select_inverter();
        break;
      case DEVICES::ADC: select_ADC();
        break;
      case DEVICES::BRAKE: select_brake();
        break;
      case DEVICES::NONE:
      default: select_none();
      }

    }
  }
  namespace PERIPH {
    enum class DEVICES { SDCARD, NONE };
    static inline void select_device(const DEVICES device) {
      GPIO::write_pin(map::SPI::PERIPHERAL::SD::CS, (pin_state) (device == DEVICES::SDCARD));
    }
  }
}
