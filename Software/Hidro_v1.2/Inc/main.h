/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"
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
#define DO4_Pin GPIO_PIN_2
#define DO4_GPIO_Port GPIOE
#define DO3_Pin GPIO_PIN_3
#define DO3_GPIO_Port GPIOE
#define DO2_Pin GPIO_PIN_4
#define DO2_GPIO_Port GPIOE
#define DO1_Pin GPIO_PIN_5
#define DO1_GPIO_Port GPIOE
#define F4_Pin GPIO_PIN_6
#define F4_GPIO_Port GPIOE
#define F5_Pin GPIO_PIN_13
#define F5_GPIO_Port GPIOC
#define ENA_24V_AUX2_Pin GPIO_PIN_0
#define ENA_24V_AUX2_GPIO_Port GPIOH
#define ENA_24V_AUX1_Pin GPIO_PIN_1
#define ENA_24V_AUX1_GPIO_Port GPIOH
#define ENA_AO_Pin GPIO_PIN_7
#define ENA_AO_GPIO_Port GPIOA
#define ENA_BT_Pin GPIO_PIN_2
#define ENA_BT_GPIO_Port GPIOB
#define ENA_485_Pin GPIO_PIN_7
#define ENA_485_GPIO_Port GPIOE
#define F2_Pin GPIO_PIN_8
#define F2_GPIO_Port GPIOE
#define PWM_LCD_Pin GPIO_PIN_9
#define PWM_LCD_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_10
#define LCD_RST_GPIO_Port GPIOE
#define ENA_AUX_3_3V_Pin GPIO_PIN_11
#define ENA_AUX_3_3V_GPIO_Port GPIOE
#define LCD_A0_Pin GPIO_PIN_14
#define LCD_A0_GPIO_Port GPIOE
#define DI10_Pin GPIO_PIN_14
#define DI10_GPIO_Port GPIOB
#define DI9_Pin GPIO_PIN_15
#define DI9_GPIO_Port GPIOB
#define DI8_Pin GPIO_PIN_10
#define DI8_GPIO_Port GPIOD
#define DI7_Pin GPIO_PIN_11
#define DI7_GPIO_Port GPIOD
#define DI6_Pin GPIO_PIN_12
#define DI6_GPIO_Port GPIOD
#define DI5_Pin GPIO_PIN_13
#define DI5_GPIO_Port GPIOD
#define DI4_Pin GPIO_PIN_14
#define DI4_GPIO_Port GPIOD
#define DI3_Pin GPIO_PIN_15
#define DI3_GPIO_Port GPIOD
#define DI2_Pin GPIO_PIN_6
#define DI2_GPIO_Port GPIOC
#define DI1_Pin GPIO_PIN_7
#define DI1_GPIO_Port GPIOC
#define DI12_Pin GPIO_PIN_0
#define DI12_GPIO_Port GPIOD
#define DI11_Pin GPIO_PIN_1
#define DI11_GPIO_Port GPIOD
#define F1_Pin GPIO_PIN_5
#define F1_GPIO_Port GPIOD
#define DO8_Pin GPIO_PIN_6
#define DO8_GPIO_Port GPIOD
#define DO7_Pin GPIO_PIN_7
#define DO7_GPIO_Port GPIOD
#define DO6_Pin GPIO_PIN_4
#define DO6_GPIO_Port GPIOB
#define DO5_Pin GPIO_PIN_5
#define DO5_GPIO_Port GPIOB
#define PB4_Pin GPIO_PIN_8
#define PB4_GPIO_Port GPIOB
#define PB3_Pin GPIO_PIN_9
#define PB3_GPIO_Port GPIOB
#define PB2_Pin GPIO_PIN_0
#define PB2_GPIO_Port GPIOE
#define PB1_Pin GPIO_PIN_1
#define PB1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
//extern volatile tREG32 Flags;
    
#define Flag_Main_Ui_Changed		        Flags.Bit._10
#define Flag_Main_1s_Event 			        Flags.Bit._16

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
