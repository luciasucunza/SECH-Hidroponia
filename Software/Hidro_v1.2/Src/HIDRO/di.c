/* Includes ------------------------------------------------------------------*/
#include "di.h"

/* Private Data --------------------------------------------------------------*/
uint8_t Di_State = 0;
uint8_t di[DI_NUMBER];

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_DI;

/* Functions -----------------------------------------------------------------*/
void di_new_sample(void)
{
	Flag_DI_Cmd = 1;
}

bool di_done(void)
{
	return !Flag_DI_Cmd;
}

void di_task(void)
{
  static uint8_t di_tickstart;
  
  switch(Di_State)
  {
  case DI_WAITING:
    if(Flag_DI_Cmd)
    {
      di_tickstart = HAL_GetTick();
      Di_State = DI_SAMPLE_1;
    }      
    break;
    
  case DI_SAMPLE_1:
  case DI_SAMPLE_2:
  case DI_SAMPLE_3:
  case DI_SAMPLE_4:
    if(HAL_GetTick()-di_tickstart <= T_DI_TICK)
      break;
    
    di[DI_LEVEL_SOL] = (di[DI_LEVEL_SOL] << 1) | HAL_GPIO_ReadPin(DI_LEVEL_SOL_Port, DI_LEVEL_SOL_Pin);
    di[DI_LEVEL_MIC] = (di[DI_LEVEL_MIC] << 1) | HAL_GPIO_ReadPin(DI_LEVEL_MIC_Port, DI_LEVEL_MIC_Pin);
    di[DI_LEVEL_MAC] = (di[DI_LEVEL_MAC] << 1) | HAL_GPIO_ReadPin(DI_LEVEL_MAC_Port, DI_LEVEL_MAC_Pin);
    di[DI_LEVEL_HER] = (di[DI_LEVEL_HER] << 1) | HAL_GPIO_ReadPin(DI_LEVEL_HER_Port, DI_LEVEL_HER_Pin);
    di[DI_LEVEL_ACI] = (di[DI_LEVEL_ACI] << 1) | HAL_GPIO_ReadPin(DI_LEVEL_ACI_Port, DI_LEVEL_ACI_Pin);
    
    if((di[DI_LEVEL_SOL] & DI_MASK) == DI_HIGH)
      Flag_Di_Level_Solucion = 1;
    else if((di[DI_LEVEL_SOL] & DI_MASK) == DI_LOW)
      Flag_Di_Level_Solucion = 0;
    
    if((di[DI_LEVEL_MIC] & DI_MASK) == DI_HIGH)
      Flag_Di_Level_Micro_Nut = 1;
    else if((di[DI_LEVEL_MIC] & DI_MASK) == DI_LOW)
      Flag_Di_Level_Micro_Nut = 0;  
    
    if((di[DI_LEVEL_MAC] & DI_MASK) == DI_HIGH)
      Flag_Di_Level_Macro_Nut = 1;
    else if((di[DI_LEVEL_MAC] & DI_MASK) == DI_LOW)
      Flag_Di_Level_Macro_Nut = 0;  
    
    if((di[DI_LEVEL_HER] & DI_MASK) == DI_HIGH)
      Flag_Di_Level_Herbicide = 1;
    else if((di[DI_LEVEL_HER] & DI_MASK) == DI_LOW)
      Flag_Di_Level_Herbicide = 0;  
    
    if((di[DI_LEVEL_ACI] & DI_MASK) == DI_HIGH)
      Flag_Di_Level_Acidulant = 1;
    else if((di[DI_LEVEL_ACI] & DI_MASK) == DI_LOW)
      Flag_Di_Level_Acidulant = 0;

    di_tickstart = HAL_GetTick();
    Di_State++;
    break;
    
  case DI_UPDATE:    
    Di_State = DI_WAITING;
    Flag_DI_Cmd = 0;
    break;
  }  
}
