#ifndef __IRRIGATION_H
#define __IRRIGATION_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "config_flash.h"
#include "datetime.h"
#include "di.h"
#include "main.h"
#include "adc_sample.h"

/* Function-------------------------------------------------------------------*/
void irrigation_task (void);
void irrigation_1s_tick (void);
void irrigation_status (char *);
void open_water_tap (void);
void close_water_tap (void);
void mixer (uint8_t);
void rail (uint8_t);
uint8_t final_day (void);

/* Exported Data -------------------------------------------------------------*/
extern volatile tREG08 Flags_Irrigation;
extern uint8_t Initial_Date[6];

/* Defines -------------------------------------------------------------------*/
#define Flag_Irriga_Ph_H            Flags_Irrigation.Bit._0
#define Flag_Irriga_Ph_L            Flags_Irrigation.Bit._1
#define Flag_Irriga_Ec_H            Flags_Irrigation.Bit._2
#define Flag_Irriga_Ec_L            Flags_Irrigation.Bit._3
#define Flag_Irriga_Growing         Flags_Irrigation.Bit._4 // Indica si estoy dentro del periodo de crecimiento de un cultivo
#define Flag_Irriga_Operate         Flags_Irrigation.Bit._5 // Indica si está operando, puede no estarlo por mantenimiento, dentro de un periodo de crecimiento

#define OFF               0
#define ON                1
#define DIP               2
#define ASCEND            3

#define CLOSE             0
#define OPEN              1

enum{
  SOLUTION,
  MICRO_NUT,
  MACRO_NUT,
  HERBICIDE,
  ACIDULANT,
  WATER
};

#define T_REVOLVE_SOLUTION  60 //s   Tiempo que la solución debe removerse para 
                               //    hacer una buena medición
#define T_MOVE_ELECTRODE    30 //s   Tiempo que se activada el motor para que
                               //    bajen/suban los electrodos de PH y TDS
#define T_STABILIZE         30 //s   Tiempo con las sondas sumergidas antes de  
                               //    tomar la medicion para estabilizar todo
#define T_OPEN_TANK         30 //s   Tiempo que se abren los tanques para regula
                               //    el ph o la conducividad                               
#define T_OPEN_TAP          20 //s   Tiempo que se abre la canilla de agua para 
                               //    regular el ph o la conducividad

/*Pins ---------------------------------------------------------------------*/
#define O_PUMP_SOLU_Pin GPIO_PIN_10
#define O_PUMP_SOLU_GPIO_Port GPIOA
#define O_PUMP_MICRO_Pin GPIO_PIN_10
#define O_PUMP_MICRO_GPIO_Port GPIOA
#define O_PUMP_MACRO_Pin GPIO_PIN_10
#define O_PUMP_MACRO_GPIO_Port GPIOA
#define O_PUMP_HERB_Pin GPIO_PIN_10
#define O_PUMP_HERB_GPIO_Port GPIOA
#define O_PUMP_ACID_Pin GPIO_PIN_10
#define O_PUMP_ACID_GPIO_Port GPIOA
#define O_MIXER_Pin GPIO_PIN_10
#define O_MIXER_GPIO_Port GPIOA
 

/*States ---------------------------------------------------------------------*/
enum{ 
  IRRIGA_INIT,                    // Verifico nivel de tanque => mido o lleno
  IRRIGA_FILLING,                 // Se carga el tanque de agua hasta un minimo
  IRRIGA_REVOLVING,               // Revuelvo para mejorar la medición
  IRRIGA_DIPPING_ELECTRODE,       // Bajo el elctrodo para hacer la medición
  IRRIGA_STABILIZING_ELECTRODE,   // Espero unos segundos para samplear
  IRRIGA_SAMPLING_PH,             // Obtengo el PH, si fuera del rango -> abro algun sub tanque, si no veo la conductividad
  IRRIGA_REGULATING_PH,           // Luego de cierto t, cierro el tanque y vuelvo al estado anterior
  IRRIGA_SAMPLING_EC,             // Obtengo condu, si fuera del rango -> abro algun sub tanque, si no subo el electrodo
  IRRIGA_REGULATING_EC,           // Luego de cierto t, cierro el tanque y vuelvo al estado anterior
  IRRIGA_ASCENDING_ELECTRODE,     // Subo el electrodo
  IRRIGA_RESTING,                 // Descanso el tiempo lo que falte hasa el siguiente regado, teniendo cuenta el tiempo de revolver
  IRRIGA_REVOLVING_2,             // Revolver antes de regar
  IRRIGA_FLOWING                  // Riego cierto tiempo
};

#endif  /* __IRRIGATION_H */