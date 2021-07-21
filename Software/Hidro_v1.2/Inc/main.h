/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2021 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "common.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

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

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */

/**
  * @}
*/

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
