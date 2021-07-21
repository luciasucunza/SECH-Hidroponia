/**
  ******************************************************************************
  * File Name          : api_rtc.c
  * Description        : 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "api_rtc.h"

/* Private Functions ---------------------------------------------------------*/
t_wday _rtc_dow(uint16_t year, uint8_t month, uint8_t date);


/* Private Data --------------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
char time[10];
char date[10];


/* API Function --------------------------------------------------------------*/
void rtc_get_time(t_rtc_time * time)
{
  RTC_DateTypeDef RTC_DateStruct1;
  RTC_TimeTypeDef RTC_TimeStruct1;

  //Recover internal RTC values
  HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct1, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &RTC_DateStruct1, RTC_FORMAT_BIN);

  /* Display time Format: hh:mm:ss */
  sprintf((char*)time,"%02d:%02d:%02d",RTC_TimeStruct1.Hours, RTC_TimeStruct1.Minutes, RTC_TimeStruct1.Seconds);

  /* Display date Format: dd-mm-yy */
  sprintf((char*)date,"%02d-%02d-%2d",RTC_DateStruct1.Date, RTC_DateStruct1.Month, 2000 + RTC_DateStruct1.Year);
  
  /* Translate data & trigger events */
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
  if (HAL_RTC_SetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BCD) != HAL_OK)
  {
    return false; //_Error_Handler(__FILE__, __LINE__); 
  }

  RTC_DateStruct.Date =     time->dayNumber;
  RTC_DateStruct.Month =    time->month;
  RTC_DateStruct.Year =     time->year;
  RTC_DateStruct.WeekDay =  _rtc_dow((time->year)+2000,time->month,time->dayNumber);
  if (HAL_RTC_SetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BCD) != HAL_OK)
  {
    return false; //_Error_Handler(__FILE__, __LINE__); 
  } 

  //HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);  // backup register
  return true;
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/* Private Function ----------------------------------------------------------*/
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


