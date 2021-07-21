/* Includes ------------------------------------------------------------------*/
#include "weather.h"

/* Private Data --------------------------------------------------------------*/
uint8_t Weather_State = 0;
uint32_t Weather_Timer;

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_Weather;
uint8_t temperature = 0;
uint8_t humidity = 0;

/* Functions -----------------------------------------------------------------*/
void weather_1s_tick(void)
{
	if(Weather_Timer)
    Weather_Timer--;
}

void weather_task(void)
{
  uint8_t DHT11_data[5];
  uint8_t checksum, aux_temp, aux_rh, i;
  static uint8_t sample = 0;
  static uint8_t temp[3];
  static uint8_t rh[3];
  
  switch(Weather_State)
  {
  case WEAT_GET_VALUE_0:        //Toma 3 muestras, evitar lecturas erroneas
  case WEAT_GET_VALUE_1:
  case WEAT_GET_VALUE_2:
    
    DHT11_start();
    if(DHT11_check_response())
    {
      for(i=0, checksum=0 ; i<5 ; i++)
      {
        DHT11_data[i] = DHT11_read();
        checksum += DHT11_data[i]; 
      }
      checksum -= DHT11_data[4];
      if(DHT11_data[CHECK]==checksum)
      {
        temp[sample] = DHT11_data[TEMP];
        rh[sample] = DHT11_data[RH]; 
        sample++;            
      }
    } 
    Weather_Timer = T_DELAY;
    Weather_State++;
    break;
    
  case WEAT_PROCESS:
    if(!sample)
    {      
      //ERROR NINGUNA MUESTRA VALIDA
      Flag_Weather_Value_Error = 1;
      Weather_State = WEAT_GET_VALUE_0;
    }      
    else  
    {
      Flag_Weather_Value_Error = 0;
      for(i=0, aux_temp=0, aux_rh=0 ; i<sample ; i++)
      {
        aux_temp += temp[i];
        aux_rh += rh[i];
      }
      temperature = aux_temp/sample;
      humidity = aux_rh/sample;
      sample = 0;
      
      if(temperature >= read_flash(ID_Temp_Max))
      {
        if(!Flag_Weather_Temp_H)
        {
          Flag_Weather_Temp_H = 1;
          //Encender algo que enfrie
        }
      }
      else
      {
        if(Flag_Weather_Temp_H)
        {
          Flag_Weather_Temp_H = 0;
          //Apagar algo que enfrie
        }
      }
      
      if(temperature <= read_flash(ID_Temp_Min))
      {
        if(!Flag_Weather_Temp_L)
        {
          Flag_Weather_Temp_L = 1;
          //Encender algo que caliente
        }
      }
      else
      {
        if(Flag_Weather_Temp_L)
        {
          Flag_Weather_Temp_L = 0;
          //Apagar algo que caliente
        }
      }
      
      if(humidity >= read_flash(ID_Hum_Max))
      {
        if(!Flag_Weather_Hum_H)
        {
          Flag_Weather_Hum_H = 1;
          //Encender algo que seque
        }
      }
      else
      {
        if(Flag_Weather_Hum_H)
        {
          Flag_Weather_Hum_H = 0;
          //Apagar algo que seque
        }
      }
      
      if(humidity <= read_flash(ID_Hum_Min))
      {
        if(!Flag_Weather_Hum_L)
        {
          Flag_Weather_Hum_L = 1;
          //Encender algo que humedezca
        }
      }
      else 
      {
        if(Flag_Weather_Hum_L)
        {
          Flag_Weather_Hum_L = 0;
          //Apagar algo que humedezca
        }
      }
      
			if((Flags_Weather.Byte & WEATHER_FLAG_MASK) == 0) //Flags en 0
        Weather_Timer = T_REST;      
      else
        Weather_Timer = T_WAIT;       
      
      Weather_State = WEAT_WAITING;
    }
    break;
    
  case WEAT_WAITING_0:
  case WEAT_WAITING_1:
  case WEAT_WAITING:
    if(!Weather_Timer)
    {
      Weather_State++;
    }
    break;
    
  case WEAT_EXIT:
    Weather_State = WEAT_GET_VALUE_0;
    break;        
  }
}
