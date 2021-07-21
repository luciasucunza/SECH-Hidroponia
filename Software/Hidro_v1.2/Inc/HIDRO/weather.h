#ifndef __WEATHER_H
#define __WEATHER_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "api_dht11.h"
#include "config_flash.h"

/* Function ------------------------------------------------------------------*/
void weather_task (void);
void weather_1s_tick (void);

/* Exported Data -------------------------------------------------------------*/
extern volatile tREG08 Flags_Weather;
extern uint8_t temperature;
extern uint8_t humidity;

/* Defines -------------------------------------------------------------------*/
#define Flag_Weather_Temp_H         Flags_Weather.Bit._0
#define Flag_Weather_Temp_L         Flags_Weather.Bit._1
#define Flag_Weather_Hum_H          Flags_Weather.Bit._2
#define Flag_Weather_Hum_L          Flags_Weather.Bit._3
#define WEATHER_FLAG_MASK           0x0f
#define Flag_Weather_Value_Error	  Flags_Weather.Bit._4

enum{
  RH,
  RH_DECI,        //DHT22
  TEMP,
  TEMP_DECI,     //DHT22
  CHECK
};

#define T_DELAY   2   //s    Periodo de muestreo del sensor

#define T_REST    2//600 //s    Tiempo de espera entre mediciones cuando T/H 
                      //     estuvieron dentro del rango permitido
#define T_WAIT    2//180 //s    Tiempo de espera entre mediciones cuando T/H no 
                      //     para que actuen los dispositivos que cambian T/H


/*States ---------------------------------------------------------------------*/
enum{ 
  WEAT_GET_VALUE_0,   // Toma una primera muestra
  WEAT_WAITING_0,     // Espera T_DELAY entre muestra y muestra, condicion del DHT11
  WEAT_GET_VALUE_1,   // Segunda muestra
  WEAT_WAITING_1,     // Espera T_DELAY
  WEAT_GET_VALUE_2,   // Tercera muestra
  WEAT_PROCESS,       // Procesa los valores tomados y decide
  WEAT_WAITING,       // Espera, T_WAIT si está actuando algun periférico, o T_REST si T/H estan en rango
  WEAT_EXIT           // Nada, solo para que los WAITING sean igaules 
};

#endif  /* __WEATHER_H */