/* Includes ------------------------------------------------------------------*/
#include "api_rtc.h"

/* Private Functions ---------------------------------------------------------*/
t_wday _rtc_dow(uint16_t year, uint8_t month, uint8_t date);

/* Function ------------------------------------------------------------------*/
void rtc_get_time(t_rtc_time * time)
{
  RTC_TimeTypeDef RTC_TimeStruct1;
  RTC_DateTypeDef RTC_DateStruct1;

  //Recover internal RTC values
  HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct1, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &RTC_DateStruct1, RTC_FORMAT_BIN); // Always after HAL_RTC_GetTime

/*
  if(memcmp(&RTC_TimeStruct1, &RTC_TimeStruct2, sizeof(RTC_TimeStruct1)) != 0)
  {
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct1, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct1, RTC_FORMAT_BIN);
  }
  if(memcmp(&RTC_DateStruct1, &RTC_DateStruct2, sizeof(RTC_DateStruct1)) != 0)
  {
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct1, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct1, RTC_FORMAT_BIN);
  }
*/
  //Translate data & trigger events
  time->year = RTC_DateStruct1.Year;
  time->month = RTC_DateStruct1.Month;
  time->dayNumber = RTC_DateStruct1.Date;
  time->day = (t_wday)RTC_DateStruct1.WeekDay;
  time->hour = RTC_TimeStruct1.Hours;
  time->min = RTC_TimeStruct1.Minutes;
  time->sec = RTC_TimeStruct1.Seconds;
}

bool rtc_set_time(t_rtc_time * time)
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  RTC_TimeStruct.Hours =    time->hour;
  RTC_TimeStruct.Minutes =  time->min;
  RTC_TimeStruct.Seconds =  time->sec;
  RTC_TimeStruct.SubSeconds =  0;

  RTC_TimeStruct.TimeFormat = RTC_HOURFORMAT_24;
  RTC_TimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  RTC_TimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
  if(HAL_RTC_SetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN) != HAL_OK)
  {
    return false;
  }

  RTC_DateStruct.Date =     time->dayNumber;
  RTC_DateStruct.Month =    time->month;
  RTC_DateStruct.Year =     time->year;
  RTC_DateStruct.WeekDay =  _rtc_dow((time->year)+2000,time->month,time->dayNumber);
  if(HAL_RTC_SetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN) != HAL_OK)
  {
    return false;
  }
  /* Allow access to RTC */
  /* Indicator for the RTC configuration */
  //HAL_RTCEx_BKUPWrite(&hrtc, RTC_INIT, 0x32F2);
  //HAL_PWR_DisableBkUpAccess();

  return true;
}

bool rtc_enable_wakeup(uint32_t seconds)
{
  if(HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds-1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    return false;
  }

  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  return true;
}

bool rtc_disable_wakeup(void)
{
  if(HAL_RTCEx_DeactivateWakeUpTimer(&hrtc) != HAL_OK)
  {
    return false;
  }

  HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
  return true;
}

/* Private Functions ---------------------------------------------------------*/
/**
 * @brief Sakamoto's algorithm to calculate DOW
 * @param   year    Year. For example 2014.
 * @param   month   Month. Example January = 1.
 * @param   date    Date.
 * @return  t_wday
 */
t_wday _rtc_dow(uint16_t year, uint8_t month, uint8_t date)
{
  const uint8_t _dow_table[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  uint16_t day;

  year-=month<3;

  day=year+ year/4 -year/100 +year/400 +_dow_table[month-1] +date;
  day%=7;

  return (t_wday)day;
}