/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void SystemClock_Config(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_R_Pin GPIO_PIN_13
#define LED_R_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOC
#define LED_B_Pin GPIO_PIN_15
#define LED_B_GPIO_Port GPIOC
#define Key_Motor_Pin GPIO_PIN_1
#define Key_Motor_GPIO_Port GPIOA
#define Key_Motor_EXTI_IRQn EXTI1_IRQn
#define Key_Display_Pin GPIO_PIN_2
#define Key_Display_GPIO_Port GPIOA
#define Key_Display_EXTI_IRQn EXTI2_IRQn
#define INA226_Alert_Pin GPIO_PIN_3
#define INA226_Alert_GPIO_Port GPIOA
#define INA226_Alert_EXTI_IRQn EXTI3_IRQn
#define USB_PG_Pin GPIO_PIN_5
#define USB_PG_GPIO_Port GPIOA
#define Motor_Power_Pin GPIO_PIN_12
#define Motor_Power_GPIO_Port GPIOB
#define Motor_5V_Pin GPIO_PIN_13
#define Motor_5V_GPIO_Port GPIOB
#define Motor_Brake_Pin GPIO_PIN_14
#define Motor_Brake_GPIO_Port GPIOB
#define Encoder_SW_Pin GPIO_PIN_5
#define Encoder_SW_GPIO_Port GPIOB
#define Encoder_SW_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
