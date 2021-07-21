/* Includes ------------------------------------------------------------------*/
#include "datetime.h"

/* Private Data --------------------------------------------------------------*/
const uint8_t days_per_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t date_range_min[] = {1, 1 ,0 ,0 ,0 };
const uint8_t date_range_max[] = {31, 12, 99, 23, 59};

/* Exported Data -------------------------------------------------------------*/
uint8_t Global_Date[6];

/* Function-------------------------------------------------------------------*/
void set_datetime(uint8_t *new_datetime)
{
  t_rtc_time time;

  rtc_disable_wakeup();

  time.year = new_datetime[YEAR];
  time.month = new_datetime[MONTH];
  time.dayNumber = new_datetime[DAY];
  time.hour = new_datetime[HOUR];
  time.min = new_datetime[MIN];
  time.sec = new_datetime[SEC];

  rtc_set_time(&time);
  
  rtc_enable_wakeup(1);
}

void refresh_global_datetime(void)
{
  t_rtc_time time;
  
  rtc_get_time(&time);
   
  Global_Date[YEAR] = time.year;
  Global_Date[MONTH] = time.month;
  Global_Date[DAY] = time.dayNumber;

  Global_Date[HOUR] = time.hour;
  Global_Date[MIN] = time.min;
  Global_Date[SEC] = time.sec; 
}

void clock_to_ascii(uint8_t *dataout, uint8_t *datain)
{
	char aux, i;
	const char sep[] = {'/','/',' ',':',0x00}; 

	for (i = DAY; i < SEC; i++)
	{
		aux = (char)bin_to_bcd(datain[i]);
		*dataout = 0x30 + (aux >> 4);
		dataout++;
		*dataout = 0x30 + (aux & 0x0F);
		dataout++;
		*dataout = sep[i];
		dataout++;
	}
}

uint8_t get_max_day(uint8_t *data)
{
  if(data[MONTH] == 2 && (data[YEAR]&0X03) == 0x00)  // If February and Leap year?
    return 29;
  else
    return days_per_month[data[MONTH]];
}

void validate_clock(uint8_t *buffer, uint8_t index)
{
	uint8_t date_max;

	if(buffer[index] < date_range_min[index]) buffer[index] = date_range_min[index];		// Min

  if(index == DAY) date_max = get_max_day(buffer);			// If we compare against the day, we get it from here
	else date_max = date_range_max[index];
  
	if(buffer[index] > date_max) buffer[index] = date_max;		// Max
}

/* Esta función tendría problemas en el año 99*/
void add_days(uint8_t *new_date, uint8_t *date, uint8_t period)
{
  uint8_t i;
  
  memcpy(new_date, date, 6);
  
  for(i=0 ; i<period ; i++)
  {
    if(new_date[DAY] < get_max_day(new_date))
    {
      new_date[DAY]++;
    }
    else
    {
      if(new_date[MONTH] < date_range_max[MONTH])
      {
        new_date[DAY] = 1;
        new_date[MONTH]++;
      }
      else
      {
        new_date[DAY] = 1;
        new_date[MONTH] = 1;
        new_date[YEAR]++;
      }
    }      
  }  
}