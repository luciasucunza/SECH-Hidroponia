#ifndef __UI_H__
#define __UI_H__

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "api_lcd.h"

/* Functions -----------------------------------------------------------------*/
void display_welcome (void);

void Display_Temp (uint8_t Temp);
void Display_Rh (uint8_t Rh);
void Display_Ph (uint16_t Ph);


#endif  /* __UI_H__  */