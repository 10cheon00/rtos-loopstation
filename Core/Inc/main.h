/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCP23017_001_INTB_Pin GPIO_PIN_11
#define MCP23017_001_INTB_GPIO_Port GPIOI
#define MCP23017_001_INTB_EXTI_IRQn EXTI15_10_IRQn
#define MCP23017_111_INTB_Pin GPIO_PIN_8
#define MCP23017_111_INTB_GPIO_Port GPIOF
#define MCP23017_111_INTB_EXTI_IRQn EXTI9_5_IRQn
#define MCP23017_101_INTA_Pin GPIO_PIN_4
#define MCP23017_101_INTA_GPIO_Port GPIOH
#define MCP23017_101_INTA_EXTI_IRQn EXTI4_IRQn
#define ENCODER4_CH1_Pin GPIO_PIN_10
#define ENCODER4_CH1_GPIO_Port GPIOH
#define ENCODER4_CH2_Pin GPIO_PIN_11
#define ENCODER4_CH2_GPIO_Port GPIOH
#define ENCODER3_CH1_Pin GPIO_PIN_6
#define ENCODER3_CH1_GPIO_Port GPIOC
#define ENCODER3_CH2_Pin GPIO_PIN_7
#define ENCODER3_CH2_GPIO_Port GPIOC
#define MCP23017_011_INTA_Pin GPIO_PIN_0
#define MCP23017_011_INTA_GPIO_Port GPIOI
#define MCP23017_011_INTA_EXTI_IRQn EXTI0_IRQn
#define ENCODER2_CH1_Pin GPIO_PIN_15
#define ENCODER2_CH1_GPIO_Port GPIOA
#define GMG12864_CS_Pin GPIO_PIN_3
#define GMG12864_CS_GPIO_Port GPIOD
#define SDMMC_Detect_Pin GPIO_PIN_4
#define SDMMC_Detect_GPIO_Port GPIOD
#define GMG12864_RST_Pin GPIO_PIN_5
#define GMG12864_RST_GPIO_Port GPIOD
#define GMG12864_DC_Pin GPIO_PIN_7
#define GMG12864_DC_GPIO_Port GPIOD
#define ENCODER2_CH2_Pin GPIO_PIN_3
#define ENCODER2_CH2_GPIO_Port GPIOB
#define ENCODER1_CH1_Pin GPIO_PIN_4
#define ENCODER1_CH1_GPIO_Port GPIOB
#define ENCODER1_CH2_Pin GPIO_PIN_5
#define ENCODER1_CH2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
