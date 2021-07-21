#ifndef COMMON_H_ 
#define COMMON_H_ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

/* Data-Types-----------------------------------------------------------------*/
typedef union
{
  uint8_t Byte;
  
  struct
  {
    uint8_t	_0	: 1;
    uint8_t	_1	: 1;
    uint8_t	_2	: 1;
    uint8_t	_3	: 1;
    uint8_t	_4	: 1;
    uint8_t	_5	: 1;
    uint8_t	_6	: 1;
    uint8_t	_7	: 1;
  } Bit;
}tREG08;

typedef union
{
  uint16_t Short;
  
  struct
  {
    uint8_t	_0	: 1;
    uint8_t	_1	: 1;
    uint8_t	_2	: 1;
    uint8_t	_3	: 1;
    uint8_t	_4	: 1;
    uint8_t	_5	: 1;
    uint8_t	_6	: 1;
    uint8_t	_7	: 1;
    uint8_t	_8	: 1;
    uint8_t	_9	: 1;
    uint8_t	_10	: 1;
    uint8_t	_11	: 1;
    uint8_t	_12	: 1;
    uint8_t	_13	: 1;
    uint8_t	_14	: 1;
    uint8_t	_15	: 1;
  } Bit;
}tREG16;

typedef union
{
  uint32_t Long;
  
  struct
  {
    uint8_t	_0	: 1;
    uint8_t	_1	: 1;
    uint8_t	_2	: 1;
    uint8_t	_3	: 1;
    uint8_t	_4	: 1;
    uint8_t	_5	: 1;
    uint8_t	_6	: 1;
    uint8_t	_7	: 1;
    uint8_t	_8	: 1;
    uint8_t	_9	: 1;
    uint8_t	_10	: 1;
    uint8_t	_11	: 1;
    uint8_t	_12	: 1;
    uint8_t	_13	: 1;
    uint8_t	_14	: 1;
    uint8_t	_15	: 1;
    uint8_t	_16	: 1;
    uint8_t	_17 : 1;
    uint8_t	_18	: 1;
    uint8_t	_19	: 1;
    uint8_t	_20	: 1;
    uint8_t	_21	: 1;
    uint8_t	_22	: 1;
    uint8_t	_23	: 1;
    uint8_t	_24	: 1;
    uint8_t	_25	: 1;
    uint8_t	_26	: 1;
    uint8_t	_27	: 1;
    uint8_t	_28	: 1;
    uint8_t	_29	: 1;
    uint8_t	_30	: 1;
    uint8_t	_31	: 1;
  } Bit;
}tREG32;

typedef union
{
	uint32_t Long;

	struct
	{ // Big endian
		uint8_t ll;
		uint8_t lh;
		uint8_t hl;
		uint8_t hh;
	} Byte;
}tLONG;

#endif /* COMMON_H_  */