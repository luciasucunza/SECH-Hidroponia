#ifndef API_RTC_H_
#define API_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "rtc.h"

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

/**
 * data-type to hold the RTC time and date in readable coded format.
 * The format is MMDDYYhhmm
 */
typedef struct
{
  uint8_t Month_high;
  uint8_t Month_low;
  uint8_t Day_high;
  uint8_t Day_low;
  uint8_t Year_high;
  uint8_t Year_low;
  uint8_t Hour_high;
  uint8_t Hour_low;
  uint8_t Minute_high;
  uint8_t Minute_low;
} t_rtc_time_readable;

/* Function  -----------------------------------------------------------------*/
void rtc_get_time(t_rtc_time * time);
bool rtc_set_time(t_rtc_time * time);
bool rtc_enable_wakeup(uint32_t seconds);
bool rtc_disable_wakeup(void);

/* Macros --------------------------------------------------------------------*/
/* takes in a t_wday arg */
#define YESTERDAY(dow)  (dow == Sunday)?Saturday:(t_wday)(dow - 1)
#define TOMORROW(dow)   (dow == Saturday)?Sunday:(t_wday)(dow + 1)

#endif  /* API_RTC_H  */