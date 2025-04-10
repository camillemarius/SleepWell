/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IO_LED1_Pin GPIO_PIN_0
#define IO_LED1_GPIO_Port GPIOC
#define IO_LED2_Pin GPIO_PIN_1
#define IO_LED2_GPIO_Port GPIOC
#define Taster_EXTI0_Pin GPIO_PIN_0
#define Taster_EXTI0_GPIO_Port GPIOA
#define Taster_EXTI0_EXTI_IRQn EXTI0_IRQn
#define SPI1_SD_CS_Pin GPIO_PIN_4
#define SPI1_SD_CS_GPIO_Port GPIOA
#define IO_VIBRATOR_Pin GPIO_PIN_4
#define IO_VIBRATOR_GPIO_Port GPIOC
#define BATTERY_ALERT_EXTI9_Pin GPIO_PIN_9
#define BATTERY_ALERT_EXTI9_GPIO_Port GPIOC
#define BATTERY_ALERT_EXTI9_EXTI_IRQn EXTI9_5_IRQn
#define UVLO_CTR_EXTI8_Pin GPIO_PIN_8
#define UVLO_CTR_EXTI8_GPIO_Port GPIOA
#define UVLO_CTR_EXTI8_EXTI_IRQn EXTI9_5_IRQn
#define BATTERY_CHARGING_EXTI10_Pin GPIO_PIN_10
#define BATTERY_CHARGING_EXTI10_GPIO_Port GPIOC
#define ADXL_INT1_EXTI5_Pin GPIO_PIN_5
#define ADXL_INT1_EXTI5_GPIO_Port GPIOB
#define ADXL_INT1_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define ADXL_INT2_EXTI6_Pin GPIO_PIN_6
#define ADXL_INT2_EXTI6_GPIO_Port GPIOB
#define ADXL_INT2_EXTI6_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
