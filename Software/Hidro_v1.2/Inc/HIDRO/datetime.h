#ifndef DATETIME_H__
#define DATETIME_H__

/* Includes ------------------------------------------------------------------*/
#include "api_rtc.h"
#include "utils.h"

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
void set_datetime (uint8_t *);
void refresh_global_datetime (void);
void clock_to_ascii (uint8_t *, uint8_t *);
uint8_t get_max_day (uint8_t *);
void validate_clock (uint8_t *, uint8_t );
void add_days (uint8_t *, uint8_t *, uint8_t );

/* Exported Data -------------------------------------------------------------*/
extern uint8_t Global_Date[];

#endif  /* DATETIME_H */