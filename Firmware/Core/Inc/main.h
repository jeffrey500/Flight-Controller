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
#define SPI4_CS_Pin GPIO_PIN_4
#define SPI4_CS_GPIO_Port GPIOE
#define SPI2_CS_Pin GPIO_PIN_3
#define SPI2_CS_GPIO_Port GPIOC
#define DAC_Pin GPIO_PIN_4
#define DAC_GPIO_Port GPIOA
#define BATT_VOLTAGE_Pin GPIO_PIN_6
#define BATT_VOLTAGE_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_7
#define Buzzer_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define SDMMC1_CD_Pin GPIO_PIN_9
#define SDMMC1_CD_GPIO_Port GPIOE
#define ERROR_Pin GPIO_PIN_10
#define ERROR_GPIO_Port GPIOE
#define DFU_Pin GPIO_PIN_11
#define DFU_GPIO_Port GPIOE
#define ARM_Pin GPIO_PIN_12
#define ARM_GPIO_Port GPIOE
#define HEARTBEAT_Pin GPIO_PIN_14
#define HEARTBEAT_GPIO_Port GPIOE
#define SPI1_INT_Pin GPIO_PIN_4
#define SPI1_INT_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
