/* Includes ------------------------------------------------------------------*/
#include "ui.h"

/* Private Data --------------------------------------------------------------*/
extern char time[10];
extern char date[10];

/* Functions -----------------------------------------------------------------*/
void display_welcome (void)
{
  lcd_init ();
  lcd_put_cur(0, 0);
  lcd_send_string("BluePill: Sensor");
  lcd_put_cur(1, 0);
  lcd_send_string("Version v0.2");
  lcd_put_cur(2, 0);
  lcd_send_string("07 / 01 / 2021");
  lcd_put_cur(3, 0);
  lcd_send_string("Lucia Sucunza");
  HAL_Delay(3000);
  lcd_clear();
}
 
void Display_Temp (uint8_t Temp)
{
	char str[20] = {0};
	lcd_put_cur(1, 0);

	sprintf (str, "TEMP: %d ", Temp);
	lcd_send_string(str);
	lcd_send_data(' C');
}

void Display_Rh (uint8_t Rh)
{
	char str[20] = {0};
	lcd_put_cur(2, 0);

	sprintf (str, "RH: %d ", Rh);
	lcd_send_string(str);
	lcd_send_data('%');
}

void Display_Ph(uint16_t raw)
{
  float ph;
  char str[20] = {0};
	lcd_put_cur(3, 0);

  ph = -0.01327*(float)raw + 34.28;
	sprintf (str, "PH: %f (%d)", ph, raw);
	lcd_send_string(str);
}