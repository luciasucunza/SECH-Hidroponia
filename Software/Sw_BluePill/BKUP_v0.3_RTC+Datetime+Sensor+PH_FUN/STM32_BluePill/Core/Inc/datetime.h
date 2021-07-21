#ifndef __DATETIME_H__
#define __DATETIME_H__

/* Includes ------------------------------------------------------------------*/
#include "api_rtc.h"

/* Data-Types-----------------------------------------------------------------*/
enum {
  DAY,
  MONTH,
  YEAR,
  HOUR,
  MIN,
  SEC
};

/* Function-------------------------------------------------------------------*/
uint16_t bin_to_bcd(uint8_t bin);
void clock_to_ascii(uint8_t *dataout, uint8_t *datain);
void validate_clock(uint8_t *buffer, uint8_t index);
void refresh_global_datetime(void);
void set_datetime(uint8_t *new_datetime);

/* Extern --------------------------------------------------------------------*/
extern uint8_t Global_Date[];

#endif  /* __DATETIME_H__  */