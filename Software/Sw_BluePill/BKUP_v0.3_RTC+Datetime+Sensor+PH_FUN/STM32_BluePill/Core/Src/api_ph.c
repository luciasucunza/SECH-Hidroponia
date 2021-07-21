/* Includes ------------------------------------------------------------------*/
#include "api_ph.h"
#include "main.h"

/* Private Data --------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* Functions -----------------------------------------------------------------*/
uint16_t ADC_GetValue (void)
{
  uint16_t raw;

  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  raw = HAL_ADC_GetValue(&hadc1);
  
  //ph = (float)raw* 3,85 / 4096 /; return ph;
  
  return raw;
}

