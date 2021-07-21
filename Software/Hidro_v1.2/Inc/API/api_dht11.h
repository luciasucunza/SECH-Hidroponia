#ifndef API_SENSOR_DHT11_H
#define API_SENSOR_DHT11_H

/* Includes ------------------------------------------------------------------*/
#include "api_rtc.h"
#include "common.h"
#include "main.h"
#include "tim.h"

/* Function-------------------------------------------------------------------*/
void DHT11_start (void);
uint8_t DHT11_check_response (void);
uint8_t DHT11_read (void);

/* Defines -------------------------------------------------------------------*/
#define T_OUT_RESPONSE 80  //us
#define T_OUT_READ     120 //us

#define DATA_ERROR     255

/*Pines ----------------------------------------------------------------------*/
#define DHT11_Pin       DI7_Pin
#define DHT11_GPIO_Port DI7_GPIO_Port

#endif  /* API_SENSOR_DHT11_H  */