#ifndef __API_RTC_H_
#define __API_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"

/* MACROS --------------------------------------------------------------------*/
/* takes in a t_wday arg */
#define YESTERDAY(dow)  (dow == Sunday)?Saturday:(t_wday)(dow - 1)
#define TOMORROW(dow)   (dow == Saturday)?Sunday:(t_wday)(dow + 1)

/* Data-Types ----------------------------------------------------------------*/
typedef enum
{
  Sunday,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday
} t_wday;

typedef struct
{
  t_wday day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t dayNumber;
  uint8_t month;
  uint16_t year;
} t_rtc_time;

/* API  ----------------------------------------------------------------------*/
void rtc_get_time(t_rtc_time * time);
bool rtc_set_time(t_rtc_time * time);
void _Error_Handler(char *file, int line);


#endif  /* __API_RTC_H_  */