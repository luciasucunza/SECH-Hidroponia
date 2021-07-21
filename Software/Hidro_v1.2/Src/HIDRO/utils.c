/* Includes ------------------------------------------------------------------*/
#include "utils.h"

/* Private Data --------------------------------------------------------------*/
const uint8_t bcd_to_bin_table[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
const uint16_t num_to_bit[] = {
  0x0001,0x0002,0x0004,0x0008,
  0x0010,0x0020,0x0040,0x0080,
  0x0100,0x0200,0x0400,0x0800,
  0x1000,0x2000,0x4000,0x8000
};

/* Functions -----------------------------------------------------------------*/
void bin_16_a_bcd_5(uint16_t bin, uint8_t * bcd)
{
	uint8_t m;

	*bcd = 0;
	*(bcd+1) = 0;
	*(bcd+2) = 0;

	for(m = 16; m > 0; m--)
	{
//		WDTCTL = WDTPW + WDTCNTCL;							// Clr WDT
		if((*bcd & 0X0F) >= 0X05) *bcd += 0X03; 
//		if((*bcd & 0XF0) >= 0X50) *bcd += 0X30;

		if((*(bcd+1) & 0X0F) >= 0X05) *(bcd+1) += 0X03; 
		if((*(bcd+1) & 0XF0) >= 0X50) *(bcd+1) += 0X30;

		if((*(bcd+2) & 0X0F) >= 0X05) *(bcd+2) += 0X03; 
		if((*(bcd+2) & 0XF0) >= 0X50) *(bcd+2) += 0X30; 

		*bcd <<= 1;
		if(*(bcd+1) & 0X80) *bcd |= 0X01;

		*(bcd+1) <<= 1;
		if(*(bcd+2) & 0X80) *(bcd+1) |= 0X01;

		*(bcd+2) <<= 1;
		if(bin & 0X8000) *(bcd+2) |= 0X01;

		bin <<= 1;
	}
}

uint8_t bin_dec_pos_to_disp_int(uint8_t bin_dec_pos)
{
	if(bin_dec_pos <= 2) return 4;	// 12,11,10 integer bits
	if(bin_dec_pos <= 5) return 3;	// 9,8,7 integer bits
	if(bin_dec_pos <= 8) return 2;	// 6,5,4 integer bits
	return 1;						// 3,2,1 integer bits
}

/**
*  Input: 12bits
*  Returns: xxxxi.ddd   if bin_decimal_position [0,2]
*           xxxxi.dddd  if bin_decimal_position [3,5]
*          xxxxi.dddd0 if bin_decimal_position [6,12]
**/
void binary_to_ascii(int16_t bin, uint8_t * ascii, uint8_t bin_decimal_position, uint8_t disp_integer)
{
	uint8_t bcd[3];
	uint8_t aux = 0;
	uint8_t * paux;
	uint16_t fraction;
	uint16_t addition;

  if(bin < 0)
  {
    bin = -bin;
    *ascii++ = '-';
  }
  paux = ascii;

// Compute the integer part
	bin_16_a_bcd_5((bin >> bin_decimal_position), bcd);			// binary to bcd

	if(disp_integer >= 5)
		*ascii++ = (bcd[0] & 0X0F) + 0X30;

	if(disp_integer >= 4)
		*ascii++ = (bcd[1] >> 4) + 0X30;

	if(disp_integer >= 3)
		*ascii++ = (bcd[1] & 0X0F) + 0X30;

	if(disp_integer >= 2)
		*ascii++ = (bcd[2] >> 4) + 0X30;

	*ascii++ = (bcd[2] & 0X0F) + 0X30;

// Decimal point
	*ascii = '.';
	ascii++;

// Compute the frational part
	bin <<= (16-bin_decimal_position);
	fraction = 20000;											//5000 * 4
	addition = 0;
	for(aux = 12; aux > 0; aux--)
	{
		if(bin & 0X8000) addition += fraction;					// Acumulate
		fraction >>= 1;											// fraction/2
		bin <<= 1;												// Rotate
	}
	addition >>= 2;												// addition/4

	// addition max val = 9999
	bin_16_a_bcd_5(addition, bcd);								// binary to bcd

	*ascii++ = (bcd[1] >> 4) + 0X30;
	*ascii++ = (bcd[1] & 0X0F) + 0X30;
	*ascii++ = (bcd[2] >> 4) + 0X30;
	*ascii++ = (bcd[2] & 0X0F) + 0X30;
	if(bin_decimal_position > 5)
	{
		*ascii++ = '0';
	}

// Clear left 0's
	while(1)
	{
		if(*(paux+1) == '.') return;
		if(*paux != '0') return;
		*paux++ = ' ';
	}
}

uint8_t nibble_to_ascii(uint8_t data)
{
	if(data <= 9) return (data + 0x30);
	else return (data + 0x37);
}

uint8_t ascii_to_nibble(uint8_t data)
{
	if(data <= 0x39) return (data - 0x30);
	else return (data - 0x37);
}

uint8_t two_ascii_to_bin(uint8_t * data)
{
	uint8_t aux;

	aux = bcd_to_bin_table[*data & 0X0F];
	data++;
	aux += (*data & 0X0F);

	return aux;
}

uint16_t bin_to_bcd(uint8_t bin)
{
    uint8_t m;
	uint16_t bcd = 0;

   	for(m = 0; m < 8; m++)
	{
   		if((bcd & 0X000F) >= 0X0005) bcd += 0X0003;
   		if((bcd & 0X00F0) >= 0X0050) bcd += 0X0030;

   		bcd <<= 1;
   		if(bin & 0X80) bcd |= 0X0001;

   		bin <<= 1;
   	}

    return bcd;
}

uint8_t bcd_to_bin(uint8_t data)
{
	uint8_t aux;

	aux = bcd_to_bin_table[(data & 0XF0)>>4];
	aux += (data & 0X0F);

	return aux;
}

void digit_to_ascii(uint8_t c, uint8_t * buffer)
{
	uint16_t aux;

	aux = bin_to_bcd(c);
	*buffer = 0X30 + (aux >> 8);
	buffer++;
	*buffer = 0X30 + ((aux >> 4) & 0X000F);
	buffer++;
	*buffer = 0X30 + (aux & 0X000F);
}

void exit_sleep(void)
{
  // Re enable PLL after stop
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  // Re enable PLLSAI1 after stop (used by SDMMC and USB)
  SET_BIT(RCC->CR, RCC_CR_PLLSAI1ON);
  // Set PLL as main clock
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
}