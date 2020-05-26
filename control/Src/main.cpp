#include <drv8323.hpp>
#include <cstdio>
#include "main.h"
#include "adc.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"
#include "a4928.hpp"
#include "math_transforms.hpp"
#include "math_util.hpp"

#include "mappings.hpp"

extern "C" void initialise_monitor_handles(void);

u32 get_phase_delay_for_rpm(u16 rpm) {
  // 60 rpm -> 1 rev/second
  // = 21 phase transitions/second
  float trans_per_s = 21.f * (f32) rpm / 60.f;
  float ms_per_trans = 1000.f / trans_per_s;
  float us_per_trans = 1000.f * ms_per_trans;
  return (u32) us_per_trans;
}

uint32_t __attribute__((hot)) micros() {
  uint32_t ms, cycle_cnt;
  do {
    ms = HAL_GetTick();
    cycle_cnt = __HAL_TIM_GET_COUNTER(&htim6);
  } while (ms != HAL_GetTick());

  return (ms * 1000) + cycle_cnt;
}
void __attribute__((hot)) delay_us(uint32_t us) {
  uint32_t start = micros();
  while (micros() - start < (uint32_t) us) {
    __ASM("nop");
  }
}

extern "C" int main() {

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC4_Init();
  MX_CAN_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();

  HAL_TIM_Base_Start(&htim6);
  initialise_monitor_handles();

  a4928::set_enable(false);
  a4928::enable_reset(true);
  drv::set_enable(false);

  //HAL_Delay(100);
  //drv::set_enable(true);
  //HAL_Delay(100);

  /*

  {
    u16 tx_buf[1] = {0b1010100000000000u};
    u16 rx_buf[1] = {0b0u};
    SPI::POWER::select_device(SPI::POWER::DEVICES::INVERTER);
    HAL_Delay(1);
    HAL_SPI_TransmitReceive(&hspi1, (u8 *) tx_buf, (u8 *) rx_buf, 1, 0xFFFF);
    HAL_Delay(1);
    SPI::POWER::select_device(SPI::POWER::DEVICES::NONE);

    u16 rx_buf_combined = rx_buf[0];
    printf("Received OCP Register: %u\r\n", rx_buf_combined);
    //printf("Buf low: %u, high: %u\r\n", rx_buf[0], rx_buf[1]);
    printf("Should be: %u\r\n", 0b0000000101011001u);
  }

  HAL_Delay(1);

  {
    drv::OCP::state.VDS_LVL = 0b1111;
    drv::OCP::state.OCP_DEG = 0b11;
    drv::OCP::state.OCP_MODE = 0b11;
    drv::OCP::state.DEAD_TIME = 0b00;
    drv::OCP::state.TRETRY = 0b1;
    drv::transact_sync::write_register<drv::OCP>(drv::OCP::state);
    //drv::transact_sync::write_register(0, 0);
  }

  HAL_Delay(1);

  {
    u16 tx_buf[1] = {0b1010100000000000u};
    u16 rx_buf[1] = {0b0u};
    SPI::POWER::select_device(SPI::POWER::DEVICES::INVERTER);
    HAL_Delay(1);
    HAL_SPI_TransmitReceive(&hspi1, (u8 *) tx_buf, (u8 *) rx_buf, 1, 0xFFFF);
    HAL_Delay(1);
    SPI::POWER::select_device(SPI::POWER::DEVICES::NONE);

    u16 rx_buf_combined = rx_buf[0u];
    printf("Received OCP Register: %u\r\n", rx_buf_combined);
    //printf("Buf low: %u, high: %u\r\n", rx_buf[0], rx_buf[1]);
    printf("Should be: %u\r\n", 0b11111111111);
  }
   */

  //printf("Initializing...\r\n");
  //drv::init();

  //printf("Calibrating...\r\n");
  //drv::calibrate();

  HAL_Delay(10);
  drv::set_enable(false);
  HAL_Delay(10);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  //HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0u);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0u);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0u);

  HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_RESET);

  drv::set_enable(true);
  HAL_Delay(10);
  printf("Initializing...\r\n");
  drv::init();

  printf("Calibrating...\r\n");
  drv::calibrate();
  printf("Done calibrating...\r\n");

  f32 compare = 0.08;
  printf("Raising to %f %% in 3... ", compare);
  HAL_Delay(500);
  printf("2... ");
  HAL_Delay(500);
  printf("1... ");
  HAL_Delay(500);
  //drv::set_enable(false);
  // Small TIM compare  -> small on time
  //__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (4096.f * compare));


  // Align

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (4096.f * compare));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (0.f * compare));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (0.f * compare));

  HAL_Delay(100);

  u16 rpm = 60;
  u16 end_rpm = 600;
  u16 ramprate = 1; // 50 RPM per second

  u32 lastTime = HAL_GetTick();
  u16 delay = get_phase_delay_for_rpm(rpm) / 2;

  HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_RESET);

  while (true) {

      if (rpm <= end_rpm)
        rpm++;

    {
      // Phase 1: A+ BZ C-
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (4096.f * compare)); // A modulate

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_RESET); // B hi-z
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (0.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (0.f * compare)); // C gnd
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    {
      // Phase 2: A+ B- CZ
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (4096.f * compare)); // A modulate

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_SET); // B hi-z
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (0.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (0.f * compare)); // C gnd
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    {
      // Phase 3: AZ B- C+
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (0.f * compare)); // A hi-z

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_SET); // B gnd
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (0.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (4096.f * compare)); // C modulate
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    {
      // Phase 4: A- BZ C+
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (0.f * compare)); // A gnd

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_RESET); // B hi-z
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (0.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (4096.f * compare)); // C modulate
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    {
      // Phase 5: A- B+ CZ
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (0.f * compare)); // A-

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_SET); // B modulate
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (4096.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (0.f * compare)); // C hi-z
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    {
      // Phase 6: AZ B+ C-
      HAL_GPIO_WritePin(PHA_Z_GPIO_Port, PHA_Z_Pin, GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint16_t) (0.f * compare)); // A hi-z

      HAL_GPIO_WritePin(PHB_Z_GPIO_Port, PHB_Z_Pin, GPIO_PIN_SET); // B modulate
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint16_t) (4096.f * compare));

      HAL_GPIO_WritePin(PHC_Z_GPIO_Port, PHC_Z_Pin, GPIO_PIN_SET);
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint16_t) (0.f * compare)); // C gnd
    }

    delay_us(get_phase_delay_for_rpm(rpm) / 2);

    // Phase 3: AZ B- C+
    // Phase 4: A- BZ C+
    // Phase 5: A- B+ CZ
    // Phase 6: AZ B+ C-
  }

}
