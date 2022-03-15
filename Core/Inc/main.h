/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define A_Pin GPIO_PIN_0
#define A_GPIO_Port GPIOA
#define B_Pin GPIO_PIN_1
#define B_GPIO_Port GPIOA
#define X_Pin GPIO_PIN_2
#define X_GPIO_Port GPIOA
#define Y_Pin GPIO_PIN_3
#define Y_GPIO_Port GPIOA
#define START_Pin GPIO_PIN_4
#define START_GPIO_Port GPIOA
#define Z_Pin GPIO_PIN_5
#define Z_GPIO_Port GPIOA
#define R_Pin GPIO_PIN_6
#define R_GPIO_Port GPIOA
#define L_Pin GPIO_PIN_7
#define L_GPIO_Port GPIOA
#define RIGHT_Pin GPIO_PIN_0
#define RIGHT_GPIO_Port GPIOB
#define LEFT_Pin GPIO_PIN_1
#define LEFT_GPIO_Port GPIOB
#define UP_Pin GPIO_PIN_2
#define UP_GPIO_Port GPIOB
#define DOWN_Pin GPIO_PIN_10
#define DOWN_GPIO_Port GPIOB
#define C_RIGHT_Pin GPIO_PIN_12
#define C_RIGHT_GPIO_Port GPIOB
#define C_LEFT_Pin GPIO_PIN_13
#define C_LEFT_GPIO_Port GPIOB
#define C_UP_Pin GPIO_PIN_14
#define C_UP_GPIO_Port GPIOB
#define C_DOWN_Pin GPIO_PIN_15
#define C_DOWN_GPIO_Port GPIOB
#define MS_Pin GPIO_PIN_8
#define MS_GPIO_Port GPIOA
#define LS_Pin GPIO_PIN_9
#define LS_GPIO_Port GPIOA
#define TX_INT_Pin GPIO_PIN_3
#define TX_INT_GPIO_Port GPIOB
#define TX_INT_EXTI_IRQn EXTI3_IRQn
#define RUMBLE_Pin GPIO_PIN_4
#define RUMBLE_GPIO_Port GPIOB
#define MX_Pin GPIO_PIN_8
#define MX_GPIO_Port GPIOB
#define MY_Pin GPIO_PIN_9
#define MY_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
