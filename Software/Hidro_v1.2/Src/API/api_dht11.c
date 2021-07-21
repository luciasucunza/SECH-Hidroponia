/* Includes ------------------------------------------------------------------*/
#include "api_dht11.h"

/* Private Functions ---------------------------------------------------------*/
void delay (uint16_t);
void set_pin_input (GPIO_TypeDef *, uint16_t);
void set_pin_output (GPIO_TypeDef *, uint16_t);

/* Functions -----------------------------------------------------------------*/
void DHT11_start(void)
{
	set_pin_output(DHT11_GPIO_Port, DHT11_Pin);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
	delay(4000);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
	delay(18000);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
	delay(30);
	set_pin_input(DHT11_GPIO_Port, DHT11_Pin);
}

uint8_t DHT11_check_response(void)
{
	uint8_t Flag_Response = 0;
	delay (40);
	if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
	{
		delay (80);
		if ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin))) 
      Flag_Response = 1;
	}
  __HAL_TIM_SET_COUNTER(&htim2, 0);
	while(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin))   // wait for the pin to go low or time out
  {
    if(__HAL_TIM_GET_COUNTER(&htim2) > T_OUT_RESPONSE)
    {
      Flag_Response = 0;
      break;
    }
  }

	return Flag_Response;
}

uint8_t DHT11_read(void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
		while (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)));   // Esperar que el pin vaya a 1 o time out
    {
      if(__HAL_TIM_GET_COUNTER(&htim2) > T_OUT_READ)
        return DATA_ERROR;
    }
		delay (40);           // Esperar 40us
		if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
		{
			i&= ~(1<<(7-j));      // Si el pin está bajo, escribir un 0
		}
		else i|= (1<<(7-j));    // Si el pin está alto, escribir un 1
    
    __HAL_TIM_SET_COUNTER(&htim2, 0);
		while (HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin));  // Esperar que el pin vaya a 0 o time out
    {
      if(__HAL_TIM_GET_COUNTER(&htim2) > T_OUT_READ)
        return DATA_ERROR;
    }
	}
	return i;
}

/* Private Functions ---------------------------------------------------------*/
void delay(uint16_t us)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

