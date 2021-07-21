/* Includes ------------------------------------------------------------------*/
#include "button.h"

/* Private Functions ---------------------------------------------------------*/
void process_buttons(void);

/* Private Data --------------------------------------------------------------*/
uint8_t buttons_current, buttons_past;

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_Buttons;
volatile uint32_t Timer_Buttons;

/* Functions -----------------------------------------------------------------*/
void clear_buttons(void)
{
  buttons_current = HAL_GPIO_ReadPin(PB1_GPIO_Port, PB1_Pin) << BUTTON_I_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB2_GPIO_Port, PB2_Pin) << BUTTON_M_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB3_GPIO_Port, PB3_Pin) << BUTTON_D_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB4_GPIO_Port, PB4_Pin) << BUTTON_R_POS;

	buttons_past = buttons_current;
	Flags_Buttons.Byte = buttons_past;
}

bool inc_buttons_fast(void)
{
  if(!Timer_Buttons)
  {
    Timer_Buttons = DELAY_FAST_MODE;
    return true;
  }
  return false;
}

void buttons_task (void)
{
  static uint32_t button_tickstart;
  
  if((HAL_GetTick() - button_tickstart) >= T_BUTTONS_TICK) 
  {
    process_buttons();
    button_tickstart = HAL_GetTick();
  }
}


/* Private Functions ---------------------------------------------------------*/
void process_buttons(void)
{
  buttons_current = HAL_GPIO_ReadPin(PB1_GPIO_Port, PB1_Pin) << BUTTON_I_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB2_GPIO_Port, PB2_Pin) << BUTTON_M_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB4_GPIO_Port, PB4_Pin) << BUTTON_D_POS;
  buttons_current |= HAL_GPIO_ReadPin(PB3_GPIO_Port, PB3_Pin) << BUTTON_R_POS;

	if(buttons_past != buttons_current)		  
	{
		buttons_past = buttons_current;		    
	}
	else
	{
		if(Flags_Buttons.Byte != buttons_past)
		{
			Flags_Buttons.Byte = buttons_past;
			Timer_Buttons = T_FAST_MODE;										// Reload fast timer (slow down)
    }
		else																	            // No change
		{	
			if(Timer_Buttons) Timer_Buttons--;							// Dec fast timer			
		}
	}
}

