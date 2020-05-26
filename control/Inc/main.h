/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_IN_CH2_Pin GPIO_PIN_9
#define PWM_IN_CH2_GPIO_Port GPIOF
#define VBUS_CURRENT_Pin GPIO_PIN_2
#define VBUS_CURRENT_GPIO_Port GPIOC
#define BRAKE_CURRENT_Pin GPIO_PIN_3
#define BRAKE_CURRENT_GPIO_Port GPIOC
#define PHASE_CURRENT_A_Pin GPIO_PIN_0
#define PHASE_CURRENT_A_GPIO_Port GPIOA
#define PHASE_CURRENT_B_Pin GPIO_PIN_2
#define PHASE_CURRENT_B_GPIO_Port GPIOA
#define PHASE_CURRENT_C_Pin GPIO_PIN_3
#define PHASE_CURRENT_C_GPIO_Port GPIOA
#define POWERSPI_SCK_Pin GPIO_PIN_5
#define POWERSPI_SCK_GPIO_Port GPIOA
#define POWERSPI_MISO_Pin GPIO_PIN_6
#define POWERSPI_MISO_GPIO_Port GPIOA
#define POWERSPI_MOSI_Pin GPIO_PIN_7
#define POWERSPI_MOSI_GPIO_Port GPIOA
#define INVERTER_CS_Pin GPIO_PIN_4
#define INVERTER_CS_GPIO_Port GPIOC
#define HALFH_CS_Pin GPIO_PIN_5
#define HALFH_CS_GPIO_Port GPIOC
#define ADC_CS_Pin GPIO_PIN_7
#define ADC_CS_GPIO_Port GPIOE
#define PHA_Z_Pin GPIO_PIN_8
#define PHA_Z_GPIO_Port GPIOE
#define PHASE_A_H_Pin GPIO_PIN_9
#define PHASE_A_H_GPIO_Port GPIOE
#define PHB_Z_Pin GPIO_PIN_10
#define PHB_Z_GPIO_Port GPIOE
#define PHASE_B_H_Pin GPIO_PIN_11
#define PHASE_B_H_GPIO_Port GPIOE
#define PHC_Z_Pin GPIO_PIN_12
#define PHC_Z_GPIO_Port GPIOE
#define PHASE_C_H_Pin GPIO_PIN_13
#define PHASE_C_H_GPIO_Port GPIOE
#define VBUS_VOLTAGE_Pin GPIO_PIN_14
#define VBUS_VOLTAGE_GPIO_Port GPIOE
#define HALFH_OOS_Pin GPIO_PIN_15
#define HALFH_OOS_GPIO_Port GPIOE
#define ENCODERSPI_SCK_Pin GPIO_PIN_13
#define ENCODERSPI_SCK_GPIO_Port GPIOB
#define ENCODERSPI_MISO_Pin GPIO_PIN_14
#define ENCODERSPI_MISO_GPIO_Port GPIOB
#define ENCODERSPI_MOSI_Pin GPIO_PIN_15
#define ENCODERSPI_MOSI_GPIO_Port GPIOB
#define HALFH_DIAG_Pin GPIO_PIN_8
#define HALFH_DIAG_GPIO_Port GPIOD
#define HALFH_RESET_Pin GPIO_PIN_9
#define HALFH_RESET_GPIO_Port GPIOD
#define HALFH_ENABLE_Pin GPIO_PIN_10
#define HALFH_ENABLE_GPIO_Port GPIOD
#define PWM_IN_CH1_Pin GPIO_PIN_12
#define PWM_IN_CH1_GPIO_Port GPIOD
#define PWR_REF_3V3_Pin GPIO_PIN_13
#define PWR_REF_3V3_GPIO_Port GPIOD
#define MOTOR_THERMISTOR_Pin GPIO_PIN_14
#define MOTOR_THERMISTOR_GPIO_Port GPIOD
#define MOTOR_ENCODER_A_Pin GPIO_PIN_6
#define MOTOR_ENCODER_A_GPIO_Port GPIOC
#define MOTOR_ENCODER_B_Pin GPIO_PIN_7
#define MOTOR_ENCODER_B_GPIO_Port GPIOC
#define LEDCONTROLLER_SDA_Pin GPIO_PIN_9
#define LEDCONTROLLER_SDA_GPIO_Port GPIOC
#define LEDCONTROLLER_SCL_Pin GPIO_PIN_8
#define LEDCONTROLLER_SCL_GPIO_Port GPIOA
#define PERIPH_SPI_SCK_Pin GPIO_PIN_10
#define PERIPH_SPI_SCK_GPIO_Port GPIOC
#define PERIPH_SPI_MISO_Pin GPIO_PIN_11
#define PERIPH_SPI_MISO_GPIO_Port GPIOC
#define PERIPH_SPI_MOSI_Pin GPIO_PIN_12
#define PERIPH_SPI_MOSI_GPIO_Port GPIOC
#define SD_SENSE_Pin GPIO_PIN_2
#define SD_SENSE_GPIO_Port GPIOD
#define HALL_A_Pin GPIO_PIN_3
#define HALL_A_GPIO_Port GPIOD
#define HALL_B_Pin GPIO_PIN_4
#define HALL_B_GPIO_Port GPIOD
#define INVERTER_FAULT_Pin GPIO_PIN_5
#define INVERTER_FAULT_GPIO_Port GPIOD
#define INVERTER_ENABLE_Pin GPIO_PIN_6
#define INVERTER_ENABLE_GPIO_Port GPIOD
#define HALL_C_Pin GPIO_PIN_7
#define HALL_C_GPIO_Port GPIOD
#define BRAKE_L_Pin GPIO_PIN_5
#define BRAKE_L_GPIO_Port GPIOB
#define CNTLI2C_SCL_Pin GPIO_PIN_6
#define CNTLI2C_SCL_GPIO_Port GPIOB
#define CNTLI2C_SDA_Pin GPIO_PIN_7
#define CNTLI2C_SDA_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_8
#define SD_CS_GPIO_Port GPIOB
#define BRAKE_H_Pin GPIO_PIN_9
#define BRAKE_H_GPIO_Port GPIOB
#define STATUS_LED_Pin GPIO_PIN_0
#define STATUS_LED_GPIO_Port GPIOE
#define SERVO_PWM_IN_Pin GPIO_PIN_1
#define SERVO_PWM_IN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

//#define __packed __attribute__((packed))

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
