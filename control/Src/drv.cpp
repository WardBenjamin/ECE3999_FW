
#include <cstdio>
#include "mappings.hpp"
#include "drv8323.hpp"
#include "spi_util.hpp"

namespace drv {

  FAULT_STAT::layout FAULT_STAT::state;
  VGS_STAT::layout VGS_STAT::state;
  DRIVER_CONTROL::layout DRIVER_CONTROL::state;
  GD_HS::layout GD_HS::state;
  GD_LS::layout GD_LS::state;
  OCP::layout OCP::state;
  CSA::layout CSA::state;

  namespace transact_sync {

    u16 write_register(u8 register_addr, u16 data) {

      drv_data_frame tx_frame{.packed=0u};
      //frame.unpacked.rw = 0b0;
      tx_frame.unpacked.addr = register_addr;
      tx_frame.unpacked.dat = data;

      drv_data_frame rx_frame{.packed=0u};

      HAL_Delay(1);
      SPI::POWER::select_device(SPI::POWER::DEVICES::INVERTER);
      //for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us
      HAL_Delay(1);

      HAL_SPI_TransmitReceive(&hspi1, (u8 *) &tx_frame, (u8 *) &rx_frame, 1, 0xFFFF);

      HAL_Delay(1);
      //for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us
      SPI::POWER::select_device(SPI::POWER::DEVICES::NONE);
      //for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us
      HAL_Delay(1);

      return rx_frame.unpacked.dat;

    }

    u16 read_register(u8 register_addr) {
      drv_data_frame tx_frame{.packed=0u};
      tx_frame.unpacked.rw = 0b1;
      tx_frame.unpacked.addr = register_addr;

      drv_data_frame rx_frame{.packed=0u};

      SPI::POWER::select_device(SPI::POWER::DEVICES::INVERTER);
      for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us

      HAL_SPI_TransmitReceive(map::SPI::POWER::SPI, (u8 *) &tx_frame.packed, (u8 *) &rx_frame.packed, 2, 0xFFFF);

      for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us
      SPI::POWER::select_device(SPI::POWER::DEVICES::NONE);
      for (u8 i = 0; i < 100; i++) __asm__("nop\n"); // Delay about 1.5-2us

      return rx_frame.unpacked.dat;
    }

  }

  void init() {
    DRIVER_CONTROL::state.DIS_CPUV = 0b0; // Enable charge pump fault
    DRIVER_CONTROL::state.DIS_GDF = 0b0; // Enable gate driver fault
    DRIVER_CONTROL::state.OTW_REP = 0b0; // Ignore overtemp
    DRIVER_CONTROL::state.PWM_MODE = (u8) REGISTER_CONSTS::DRIVER_CONTROL::PWM_MODE::PWM_MODE_3x;
    DRIVER_CONTROL::state.COAST = 0b0;
    DRIVER_CONTROL::state.BRAKE = 0b0;

    GD_HS::state.LOCK = 0b011;
    GD_HS::state.IDRIVEP_HS = 0b1111; // 190 mA, good starting point
    GD_HS::state.IDRIVEN_HS = 0b1111; // 380 mA

    GD_LS::state.TDRIVE = 0b11;
    GD_LS::state.IDRIVEP_LS = 0b1111; // 190 mA, good starting point
    GD_LS::state.IDRIVEN_LS = 0b1111; // 380 mA

    OCP::state.TRETRY = 0b0; // 4 ms
    OCP::state.DEAD_TIME = 0b00; // 100 ns dead time
    OCP::state.OCP_MODE = 0b10; // Report only
    OCP::state.OCP_DEG = 0b01; // 4 us deglitch time
    OCP::state.VDS_LVL = 0b1001;

    CSA::state.CSA_FET = 0b0; // make the op amps work
    CSA::state.VREF_DIV = 0b1; // Div vref by 2
    CSA::state.LS_REF = 0b0; // Measure vds correctly
    CSA::state.CSA_GAIN = 0b10; // 20x gain
    CSA::state.DIS_SEN = 0b1; // Disable overcurrent sense
    CSA::state.CSA_CAL_A = 0b0; // No calibrating
    CSA::state.CSA_CAL_B = 0b0;
    CSA::state.CSA_CAL_C = 0b0;
    CSA::state.SEN_LVL = 0b11; // Unused

    transact_sync::write_register<DRIVER_CONTROL>(DRIVER_CONTROL::state);
    HAL_Delay(1);
    transact_sync::write_register<GD_HS>(GD_HS::state);
    HAL_Delay(1);
    transact_sync::write_register<GD_LS>(GD_LS::state);
    HAL_Delay(1);
    transact_sync::write_register<OCP>(OCP::state);
    HAL_Delay(1);
    transact_sync::write_register<CSA>(CSA::state);
    HAL_Delay(1);

  }

  void calibrate() {
    CSA::state.CSA_CAL_A = 0b1;
    CSA::state.CSA_CAL_B = 0b1;
    CSA::state.CSA_CAL_C = 0b1;

    printf("First transaction\r\n");
    transact_sync::write_register<CSA>(CSA::state);
    printf("Done\r\n");
    HAL_Delay(5);

    CSA::state.CSA_CAL_A = 0b0;
    CSA::state.CSA_CAL_B = 0b0;
    CSA::state.CSA_CAL_C = 0b0;

    printf("Second transaction\r\n");
    transact_sync::write_register<CSA>(CSA::state);
    HAL_Delay(5);
    printf("Done\r\n");
  }
}

