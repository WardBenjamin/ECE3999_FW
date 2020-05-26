#pragma once

#include "main.h"
#include "spi_util.hpp"

/*
 * Notes:
 * If a struct is packed
 * E1
 * E2
 * E3
 * E4
 * E5
 *
 * then E1 will be LSB, E5 will be MSB
 *
 * Will be lsb-aligned when packed (5 bit struct into u8 gives 000XXXXX)
 * Will be lsb-aligned when unpacked (u8 000XXXXX into 5-bit struct OK)
 *
 *
 * If a struct is
 * E1: 1
 * E2: 3
 * E3: 1
 *
 * It'll be packed E3 (E2 MSB -> LSB) E1
 *
 */

namespace drv {

  struct register_def {};

  struct drv_register {
    const u8 addr;
    register_def state;
  };

  union drv_data_frame {
    u16 packed;
    struct __packed {
      u16 dat: 11;
      u8 addr: 4;
      u8 rw: 1;
    } unpacked;
  };

  namespace transact_sync {
    u16 write_register(u8 register_addr, u16 data);
    u16 read_register(u8 register_addr);

    template<class T> void write_register(const typename T::layout reg) {
      union {
        u16 packed_data;
        struct T::layout register_map;
      } data_pack{.packed_data=0};

      data_pack.register_map = reg;

      write_register(T::addr, data_pack.packed_data);
    }

    template<class T> void read_register(register_def &reg) {
      union {
        u16 packed_data;
        struct T::layout register_map;
      } data_pack{.packed_data=0};

      data_pack.packed_data = read_register(T::addr);
      reg = data_pack.register_map;
    }
  }

  struct FAULT_STAT : drv_register {
    struct __packed layout : register_def {
      u8 VDS_LC: 1 = 0;
      u8 VDS_HC: 1=0;
      u8 VDS_LB: 1=0;
      u8 VDS_HB: 1=0;
      u8 VDS_LA: 1=0;
      u8 VDS_HA: 1=0;
      u8 OTSD: 1=0;
      u8 UVLO: 1=0;
      u8 GDF: 1=0;
      u8 VDS_OCP: 1=0;
      u8 FAULT: 1=0;
    };
    static constexpr u8 addr = 0x0;
    static layout state;
  };

  class VGS_STAT : drv_register {
  public:
    struct __packed layout : register_def {
      u8 VGS_LC: 1;
      u8 VGS_HC: 1;
      u8 VGS_LB: 1;
      u8 VGS_HB: 1;
      u8 VGS_LA: 1;
      u8 VGS_HA: 1;
      u8 CPUV: 1;
      u8 OTW: 1;
      u8 SC_OC: 1;
      u8 SB_OC: 1;
      u8 SA_OC: 1;
    };
    static constexpr u8 addr = 0x1;
    static layout state;
  };

  class DRIVER_CONTROL : drv_register {
  public:
    struct __packed layout : register_def {
      u8 CLR_FLT: 1=0b0;
      u8 BRAKE: 1=0b0;
      u8 COAST: 1=0b0;
      u8 PWM1_DIR: 1=0b0;
      u8 PWM1_COM: 1=0b0;
      u8 PWM_MODE: 2=0b00;
      u8 OTW_REP: 1=0b0;
      u8 DIS_GDF: 1=0b0;
      u8 DIS_CPUV: 1=0b0;
      u8 RES: 1=0b0;
    };
    static constexpr u8 addr = 0x2;
    static layout state;
  };

  class GD_HS : drv_register {
  public:
    struct __packed layout : register_def {
      u8 IDRIVEN_HS: 4=0b1111;
      u8 IDRIVEP_HS: 4=0b1111;
      u8 LOCK: 3=0b011;
    };
    static constexpr u8 addr = 0x3;
    static layout state;
  };

  class GD_LS : drv_register {
  public:
    struct __packed layout : register_def {
      u8 IDRIVEN_LS: 4=0b1111;
      u8 IDRIVEP_LS: 4=0b1111;
      u8 TDRIVE: 2=0b11;
      u8 CBC: 1=0b0;
    };
    static constexpr u8 addr = 0x4;
    static layout state;
  };

  class OCP : drv_register {
  public:
    struct __packed layout : register_def {
      u8 VDS_LVL: 4=0b1001;
      u8 OCP_DEG: 2=0b01;
      u8 OCP_MODE: 2=0b01;
      u8 DEAD_TIME: 2=01;
      u8 TRETRY: 1=0b0;
    };
    static constexpr u8 addr = 0x5;
    static layout state;
  };

  class CSA : drv_register {
  public:
    struct __packed layout : register_def {
      u8 SEN_LVL: 2=0b11;
      u8 CSA_CAL_C: 1=0b0;
      u8 CSA_CAL_B: 1=0b0;
      u8 CSA_CAL_A: 1=0b0;
      u8 DIS_SEN: 1=0b0;
      u8 CSA_GAIN: 2=0b10;
      u8 LS_REF: 1=0b0;
      u8 VREF_DIV: 1=0b1;
      u8 CSA_FET: 1=0b0;
    };
    static constexpr u8 addr = 0x6;
    static layout state;
  };
  enum class SEN_OCP_FAULT_EN { _ENABLE = 0b0, _DISABLE = 0b1 };

  namespace REGISTER_CONSTS {

    namespace DRIVER_CONTROL {
      enum class DIS_CPUV { CP_UVLO_FAULT_EN = 0b00, CP_UVLO_FAULT_DISABLE = 0b01 };
      enum class DIS_GDF { GD_FAULT_EN = 0b00, GD_FAULT_DISABLE = 0b01 };
      enum class OTW_REP { OTW_IGNORE = 0b00, OTW_REPORT = 0b01 };
      enum class PWM_MODE { PWM_MODE_6x = 0b00, PWM_MODE_3x = 0b01, PWM_MODE_1x = 0b10, PWM_MODE_INDEPENDENT = 0b11 };
      enum class PWM1_COM { RECT_SYNC = 0b00, RECT_ASYNC = 0b01 };
    }

    namespace GD_HS {
      enum class LOCK_MODE { LOCK = 0b110, UNLOCK = 0b011 };
      enum class HS_DRIVE_SOURCE_CURRENT_mA {
        _10 = 0b000, _30 = 0b001, _60 = 0b0010, _80 = 0b0011, _120 = 0b0100, _140 = 0b0101, _170 = 0b0110,
        _190 = 0b0111, _260 = 0b1000, _330 = 0b1001, _370 = 0b1010, _440 = 0b1011, _570 = 0b1100, _680 = 0b1101,
        _820 = 0b1110, _1000 = 0b1111
      };
      enum class HS_DRIVE_SINK_CURRENT_mA {
        _20 = 0b000, _60 = 0b001, _120 = 0b0010, _160 = 0b0011, _240 = 0b0100, _280 = 0b0101, _340 = 0b0110,
        _380 = 0b0111, _520 = 0b1000, _660 = 0b1001, _740 = 0b1010, _880 = 0b1011, _1140 = 0b1100, _1360 = 0b1101,
        _1640 = 0b1110, _2000 = 0b1111
      };
    }

    namespace GD_LS {
      enum class DRIVE_TIME_ns { _500 = 0b00, _1000 = 0b01, _2000 = 0b10, _4000 = 0b11 };

      typedef GD_HS::HS_DRIVE_SOURCE_CURRENT_mA LS_DRIVE_SOURCE_CURRENT_mA;
      typedef GD_HS::HS_DRIVE_SINK_CURRENT_mA LS_DRIVE_SINK_CURRENT_mA;
    }

    namespace OCP {
      enum class VDS_SEN_OCP_RETRY_TIME_us { _4000 = 0b0, _50 = 0b1 };
      enum class DEAD_TIME_ns { _50 = 0b00, _100 = 0b01, _200 = 0b10, _400 = 0b11 };
      enum class OCP_FAULT_MODE { LATCHED_FAULT = 0b00, AUTORETRY_FAULT = 0b01, REPORT_ONLY = 0b10, IGNORE = 0b11 };
      enum class OCP_DEGLITCH_TIME_us { _2 = 0b00, _4 = 0b01, _6 = 0b10, _8 = 0b11 };
      enum class OCP_VDS_THRESHOLD_mV {
        _60 = 0b0000, _130 = 0b0001, _200 = 0b0010, _260 = 0b0011, _310 = 0b0100, _450 = 0b0101, _530 = 0b0110,
        _600 = 0b0111, _680 = 0b1000, _750 = 0b1001, _940 = 0b1010, _1130 = 0b1011, _1300 = 0b1100, _1500 = 0b1101,
        _1700 = 0b1110, _1880 = 0b1111
      };
    }

    namespace CSA {
      enum class CSA_FET_INPUT { SPx = 0b0, SHx = 0b1 };
      enum class VREF_DIV { VREF_NO_DIV = 0b0, VREF_DIV_2 = 0b1 };
      enum class LS_REF { SHx_TO_SPx = 0b0, SHx_TO_SNx = 0b1 };
      enum class CSA_GAIN { _5x = 0b00, _10x = 0b01, _20x = 0b10, _40x = 0b11 };
      enum class SENSE_OCP_EN { _ENABLE = 0b0, _DISABLE = 0b01 };
      enum class CSA_CAL_A { RUN = 0b0, CALIBRATE = 0b1 };
      enum class CSA_CAL_B { RUN = 0b0, CALIBRATE = 0b1 };
      enum class CSA_CAL_C { RUN = 0b0, CALIBRATE = 0b1 };
      enum class SEN_OCP_LVL_mV { _250 = 0b00, _500 = 0b01, _750 = 0b10, _1000 = 0b11 };
    }
  }

  static inline bool get_fault() {
    return (bool) GPIO::read_pin(map::GPIO::inverter::fault);
  }

  static inline void set_enable(bool en) {
    GPIO::write_pin(map::GPIO::inverter::enable, (pin_state) en);
  }

  void init();

  void calibrate();
}

