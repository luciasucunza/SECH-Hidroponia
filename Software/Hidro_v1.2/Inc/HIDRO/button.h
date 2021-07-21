#ifndef BUTTON_H
#define BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"

/* Function ------------------------------------------------------------------*/
void clear_buttons (void);
bool inc_buttons_fast(void);
void buttons_task (void);

/* Exported Data -------------------------------------------------------------*/
extern volatile tREG08 Flags_Buttons;
extern volatile uint32_t Timer_Buttons;

/* Defines -------------------------------------------------------------------*/
#define Flag_Button_M	Flags_Buttons.Bit._3
#define Flag_Button_I	Flags_Buttons.Bit._2
#define Flag_Button_D	Flags_Buttons.Bit._1
#define Flag_Button_R	Flags_Buttons.Bit._0
#define BUTTON_MASK		  0x0F

#define T_BUTTONS_TICK  15    // 15ms
#define T_FAST_MODE		  100		// BUTTONS_TICK * 100
#define DELAY_FAST_MODE 2

#define BUTTON_M_POS    3
#define BUTTON_I_POS    2
#define BUTTON_D_POS    1
#define BUTTON_R_POS    0

#endif  /* BUTTON_H */