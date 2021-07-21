/* Includes ------------------------------------------------------------------*/
#include "lcd.h"

/* Private Functions ---------------------------------------------------------*/
void st7565_command(uint8_t);
void st7565_data(uint8_t *, uint16_t);
void GotoXY(uint8_t, uint8_t);
void LCDScroll (void);
void LCDBlink (void);

/* Private Data --------------------------------------------------------------*/
uint16_t LcdMemIdx;                   // LCD memory index
uint8_t LcdMemory[LCD_CACHE_SIZE];    // represent LCD matrix

volatile uint32_t Timer_LCD_Blink;
volatile uint32_t Timer_LCD_Scroll;
uint8_t blink_x;
uint8_t blink_y;
uint8_t blink_size;
uint8_t * blink_ptr;
uint8_t scroll_x;
uint8_t scroll_y;
uint8_t * scroll_index;
uint8_t * scroll_start;
uint8_t * scroll_ptr;

uint8_t coord_x;
uint8_t coord_y;

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_LCD;

/* Functions -----------------------------------------------------------------*/
void LCDContrast(void)
{
  // electronic volume mode set  
  st7565_command(CMD_SET_VOLUME_FIRST);
  st7565_command(read_flash(ID_Contrast));
//  st7565_command(16);
}

void LCDInit(void)
{
  // Reset low
  HAL_GPIO_WritePin (LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);  
  // A0 low
  HAL_GPIO_WritePin (LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);
  // SPI init
  MX_SPI1_Init();
  // 3.3V aux on
  HAL_GPIO_WritePin (ENA_AUX_3_3V_GPIO_Port, ENA_AUX_3_3V_Pin, GPIO_PIN_SET);
  HAL_Delay(4);
  // Reset high
  HAL_GPIO_WritePin (LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(2);

  // LCD bias select
  st7565_command(CMD_SET_BIAS_9);
  // ADC select
  st7565_command(CMD_SET_ADC_NORMAL);
  // display off
  st7565_command(CMD_DISPLAY_OFF);
  // SHL select
  st7565_command(CMD_SET_COM_REVERSE);
  // turn on voltage follower (VC=1, VR=1, VF=1)
  st7565_command(CMD_SET_POWER_CONTROL | 0x7);
  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  st7565_command(CMD_SET_RESISTOR_RATIO | 0x5);
  LCDContrast();
  // display on
  st7565_command(CMD_DISPLAY_ON);
  
  GotoXY(0,0);
	LCDClearLines(L0,L7);
  LCDUpdate();
  
  // backlight on
  MX_TIM1_Init();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void LCDSleep(void)
{
  // display off
  st7565_command(CMD_DISPLAY_OFF);
  // all point on
  st7565_command(CMD_SET_ALLPTS_ON);
  // Reset low
  HAL_GPIO_WritePin (LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);  
  // A0 low
  HAL_GPIO_WritePin (LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);
  // spi deinit
  HAL_SPI_DeInit(&hspi1);
  HAL_Delay(10);
  // 3.3V aux off
  HAL_GPIO_WritePin (ENA_AUX_3_3V_GPIO_Port, ENA_AUX_3_3V_Pin, GPIO_PIN_RESET);
  // backlight off
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_DeInit(&htim1);
}

void LCDClearLines(unsigned short a, unsigned short b)
{
	unsigned short i;
  
	if((a <= blink_y) && (b >= blink_y)) Flag_LCD_Blink_On = 0;
	if((a <= scroll_y) && (b >= scroll_y)) Flag_LCD_Scroll_On = 0;

	b++;
	for(i = (LCD_X_RES * a); i < (LCD_X_RES * b); i++)
    LcdMemory[i] = 0;

	Flag_LCD_Update = 1;
    
}

// Function : Change LCD Pixel mode
void LCDPixel (uint8_t x, uint8_t y, uint8_t mode )
{
	unsigned int    index   = 0;
	uint8_t   offset  = 0;
	uint8_t   data    = 0;
	
	index = ((y / 8) * 84) + x;
	offset  = y - ((y / 8) * 8);
	
	data = LcdMemory[index];
	
	if( mode == PIXEL_OFF )
	{
		data &= (~(0x01 << offset));
	}
	else if( mode == PIXEL_ON )
	{
		data |= (0x01 << offset);
	}
	else if( mode  == PIXEL_XOR )
	{
		data ^= (0x01 << offset);
	}
	
	LcdMemory[index] = data;
}

// Function : Place one uint8_t at X,Y 
void LCDChrXY (uint8_t x, uint8_t y, uint8_t ch )
{
	unsigned int index;
	uint8_t i;

	index = y*LCD_X_RES + x*COL_WIDTH;

	for ( i = 0; i < (COL_WIDTH-1); i++ ) // stop on pixel 5, 6 is the space
	{
	  LcdMemory[index] = FontLookup[ch - 32][i];
	  index++;
	}
}

void LCDNotChrXY (uint8_t x, uint8_t y, uint8_t ch )
{
	unsigned int index;
	uint8_t i;

	index = y*LCD_X_RES + x*COL_WIDTH;

	for (i = 0; i < (COL_WIDTH-1); i++) // stop on pixel 5, 6 is the space
	{
	  LcdMemory[index] = ~FontLookup[ch - 32][i];
	  index++;
	}
}

void LCDChrXY_2 (uint8_t x, uint8_t y, uint8_t ch )
{
	unsigned int index;
	uint8_t i;
	
	index = y*LCD_X_RES + x*COL_WIDTH_2;
	
	for (i = 0; i < COL_WIDTH_2; i++)
	{
		LcdMemory[index] = (FontLookup_2[ch - 32][i] & 0x00FF) << 3;
		LcdMemory[index+LCD_X_RES] = FontLookup_2[ch - 32][i] >> 5;
		index++;
	}
}

void LCDStr_2(uint8_t x, uint8_t y, uint8_t *dataPtr )
{
	// If the desired line equals the one being blinked or scrolled, disable these functions
	if(y == blink_y) Flag_LCD_Blink_On = 0;
	if(y == scroll_y) Flag_LCD_Scroll_On = 0;

	// Loop to the until the end of the string
	while( *dataPtr && (x < LINE_SIZE_2))
	{
		LCDChrXY_2( x, y, (*dataPtr));
		x++;
		dataPtr++;
	}

	Flag_LCD_Update = 1;
}

// Function : Send string to LCD 
void LCDStr(uint8_t x, uint8_t y, uint8_t *dataPtr )
{
	// If the desired line equals the one being blinked or scrolled, disable these functions
	if(y == blink_y) Flag_LCD_Blink_On = 0;
	if(y == scroll_y) Flag_LCD_Scroll_On = 0;

	// Loop to the until the end of the string
	while( *dataPtr && (x < LINE_SIZE))
	{
		LCDChrXY( x, y, (*dataPtr));
		x++;
		dataPtr++;
	}
	Flag_LCD_Update = 1;
}

// Function : Update LCD memory (send all the array) 
void LCDUpdate ( void )
{
 	int row;

  if((HAL_GetTick() - Timer_LCD_Blink) >= 500)
	{
		Timer_LCD_Blink = HAL_GetTick();
		if(Flag_LCD_Blink_On)
      LCDBlink();
	}

  if((HAL_GetTick() - Timer_LCD_Scroll) >= 333)
	{
		Timer_LCD_Scroll = HAL_GetTick();
		if(Flag_LCD_Scroll_On)
      LCDScroll();
	}

	if(!Flag_LCD_Update) return;	// Update LCD if needed only
	Flag_LCD_Update = 0;

	for(row = 0; row < (LCD_Y_RES/8); row++)
	{
    GotoXY(0, row*8);
    st7565_data(&LcdMemory[(LCD_X_RES * row)], LCD_X_RES);
    /*
		for(col = 0; col < LCD_X_RES; col++)
		{
      st7565_data(LcdMemory[(LCD_X_RES * row) + col]);
		}
    */
	}
}

/* Scroll routines -----------------------------------------------------------*/
void LCDScrollOn(uint8_t y, uint8_t *dataPtr )
{
	// If the desired line equals the one being blinked, disable this function
	if(y == blink_y) Flag_LCD_Blink_On = 0;

	scroll_y = y;
	scroll_start = dataPtr;
	scroll_ptr = dataPtr;
	Flag_LCD_Scroll_On = 1;
}

void LCDScrollOff(void)
{
	Flag_LCD_Scroll_On = 0;
}


/* Blink routines ------------------------------------------------------------*/
// This function will print a string but will reverse the bytes# reverse and reverse+1
void LCDBlinkOn(uint8_t x, uint8_t y, uint8_t *dataPtr, uint8_t size )
{
	// If the desired line equals the one being scrolled, disable this function
	if(y == scroll_y) Flag_LCD_Scroll_On = 0;

	blink_x = x;
	blink_y = y;
	blink_size = size;
	blink_ptr = dataPtr;
	Flag_LCD_Blink_On = 1;
}

void LCDBlinkOff(void)
{
	Flag_LCD_Blink_On = 0;
}

/* Private Functions ---------------------------------------------------------*/
void st7565_command(uint8_t c)
{
  // A0 = 0;
  HAL_GPIO_WritePin (LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);
  // Transmit data  
  HAL_SPI_Transmit (&hspi1, &c, 1, 2);
}
                    
void st7565_data(uint8_t * buf, uint16_t size)
{
  // A0 = 1;
  HAL_GPIO_WritePin (LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET);
  // Transmit data  
  HAL_SPI_Transmit (&hspi1, buf, size, 5);
}

void GotoXY(uint8_t x, uint8_t y)
{
    coord_x = x;
    coord_y = y;
	// set x address
    st7565_command(CMD_SET_COLUMN_UPPER | (x >> 4));
    st7565_command(CMD_SET_COLUMN_LOWER | (x & 0x0F));

	// set y address (y/8=page)
    st7565_command(CMD_SET_PAGE | (y >> 3));
}

void LCDScroll(void)
{
	uint8_t i;

	scroll_index = scroll_start;
	for(i=0; i<LINE_SIZE; i++)
	{
		if(*scroll_index == 0)
		{
			LCDChrXY(i, scroll_y, ' ');
			scroll_index = scroll_ptr;
		}
		else
		{
			LCDChrXY(i, scroll_y, *scroll_index);
			scroll_index++;
		}
	}

	if(*scroll_start == 0) scroll_start = scroll_ptr;
	else scroll_start++;

	Flag_LCD_Update = 1;
}

void LCDBlink(void)
{
	uint8_t size;

	if(Flag_LCD_Blink)
	{
      Flag_LCD_Blink = 0;
      for(size = 0; size < blink_size; size++)
      {
          LCDChrXY(blink_x + size, blink_y, *(blink_ptr + size));
      }
	}
	else
	{
      Flag_LCD_Blink = 1;
      for(size = 0; size < blink_size; size++)
      {
          LCDNotChrXY(blink_x + size, blink_y, *(blink_ptr + size));
      }
	}
	Flag_LCD_Update = 1;
}
