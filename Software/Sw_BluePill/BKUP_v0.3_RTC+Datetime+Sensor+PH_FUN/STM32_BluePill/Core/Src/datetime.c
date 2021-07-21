/* Includes ------------------------------------------------------------------*/
#include "datetime.h"

const char days_per_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char date_range_min[] = {1, 1 ,0 ,0 ,0 };
const char date_range_max[] = {31, 12, 99, 23, 59};

/* Private variables ---------------------------------------------------------*/
uint8_t Global_Date[6];

/* Functions -----------------------------------------------------------------*/
void set_datetime(uint8_t *new_datetime)
{
  t_rtc_time time;

  //rtc_disable_wakeup();

  time.year = new_datetime[YEAR];
  time.month = new_datetime[MONTH];
  time.dayNumber = new_datetime[DAY];
  time.hour = new_datetime[HOUR];
  time.min = new_datetime[MIN];
  time.sec = new_datetime[SEC];

  rtc_set_time(&time);
  
  //rtc_enable_wakeup(1);
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

void validate_clock(uint8_t *buffer, uint8_t index)
{
	char date_max;

	if(buffer[index] < date_range_min[index]) buffer[index] = date_range_min[index];		// Min

	if(index != DAY && buffer[MONTH] == 2)  // If we compare against the Day and February                                                           
  {
    if((buffer[YEAR]&0X03) == 0x00) date_max = 29;  // Leap year?
    else date_max = 28;
  }
  else 
  {
    date_max = date_range_max[index];
  }
  
	if(buffer[index] > date_max) buffer[index] = date_max;		// Max
}


/* Aux Functions -------------------------------------------------------------*/
/* Format: dd/mm/yy hh:mm*/
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

