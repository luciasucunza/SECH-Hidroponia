#ifndef __DI_H
#define __DI_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"

/* Function-------------------------------------------------------------------*/
void di_new_sample (void);
bool di_done (void);
void di_task (void);
void di_1s_tick (void);
void di_init (void);

/* Exported Data -------------------------------------------------------------*/
extern volatile tREG08 Flags_DI;

/* Defines -------------------------------------------------------------------*/
#define Flag_DI_Cmd                	     Flags_DI.Bit._0
#define Flag_Di_Level_Solucion           Flags_DI.Bit._1
#define Flag_Di_Level_Micro_Nut          Flags_DI.Bit._2
#define Flag_Di_Level_Macro_Nut          Flags_DI.Bit._3
#define Flag_Di_Level_Herbicide          Flags_DI.Bit._4
#define Flag_Di_Level_Acidulant          Flags_DI.Bit._5

#define DI_MASK   0x0f// 00001111
#define DI_HIGH   0x0f// 00001111
#define DI_LOW    0x00// 00000000

#define DI_NUMBER               5 // Cantidad de entradas digitales
#define T_DI_TICK               5 //ms Tiempo entre dos muestras del pin para el
                                  // debounce, que toma 4 muestras

enum{
  DI_LEVEL_SOL,
  DI_LEVEL_MIC,
  DI_LEVEL_MAC,
  DI_LEVEL_HER,
  DI_LEVEL_ACI,
};

/*Pins ---------------------------------------------------------------------*/
#define DI_LEVEL_SOL_Pin    DI7_Pin
#define DI_LEVEL_SOL_Port   DI7_GPIO_Port
#define DI_LEVEL_MIC_Pin    DI8_Pin
#define DI_LEVEL_MIC_Port   DI8_GPIO_Port
#define DI_LEVEL_MAC_Pin    DI9_Pin
#define DI_LEVEL_MAC_Port   DI9_GPIO_Port
#define DI_LEVEL_HER_Pin    DI10_Pin 
#define DI_LEVEL_HER_Port   DI10_GPIO_Port 
#define DI_LEVEL_ACI_Pin    DI11_Pin 
#define DI_LEVEL_ACI_Port   DI11_GPIO_Port 
 
/*States ---------------------------------------------------------------------*/
enum{
  DI_WAITING,
  DI_SAMPLE_1,
  DI_SAMPLE_2,
  DI_SAMPLE_3,
  DI_SAMPLE_4,
  DI_UPDATE
};

#endif  /* __DI_H */