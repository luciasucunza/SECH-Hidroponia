#ifndef __API_SENSOR_DHT11_H_
#define __API_SENSOR_DHT11_H_

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"
#include "stm32f1xx_hal.h"

/* Function-------------------------------------------------------------------*/
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void DHT11_Start (void);
uint8_t DHT11_Check_Response (void);
uint8_t DHT11_Read (void);

#endif  /* __API_SENSOR_DHT11_H_  */