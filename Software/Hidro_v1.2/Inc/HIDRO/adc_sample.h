#ifndef __ADC_SAMPLE_H
#define __ADC_SAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "common.h"
#include "config_flash.h"
#include "main.h"      //Solo por Error_Handler

/* Function ------------------------------------------------------------------*/
void adc_task (void);
//void adc_1s_tick (void);
void adc_new_sample(uint8_t);
bool adc_done(uint8_t);

/* Exported Variables --------------------------------------------------------*/
extern volatile tREG08 Flags_ADC;
extern float ph;
extern float ec;
extern float sol_temp;

/* Defines -------------------------------------------------------------------*/
#define Flag_ADC_Cmd_Ph	             Flags_ADC.Bit._0
#define Flag_ADC_Cmd_Ec              Flags_ADC.Bit._1
#define ADC_CMD_PH                  (1<<0)     // Mascaras para Flags
#define ADC_CMD_EC                  (1<<1)
//#define ADC_CMD_MASK                (1<<0)|(1<<1)
#define Flag_ADC_Error_Vrefint       Flags_ADC.Bit._2
#define Flag_ADC_Error_Temp          Flags_ADC.Bit._3
#define Flag_ADC_Error_Ph	           Flags_ADC.Bit._4
#define Flag_ADC_Error_Ec            Flags_ADC.Bit._5
#define Flag_ADC_Electrode_Ready     Flags_ADC.Bit._6
#define Flag_ADC_Event_Storage       Flags_ADC.Bit._7

#define ENA_P0_Pin    DO1_Pin
#define ENA_P0_Port   DO1_GPIO_Port
#define ENA_P1_Pin    DO2_Pin
#define ENA_P1_Port   DO2_GPIO_Port

//ADC_CHANNEL_VREFINT
#define CH_TEMP           ADC_CHANNEL_14  
#define CH_PH             ADC_CHANNEL_13
#define CH_EC             ADC_CHANNEL_4

#define LOG2_NUM_SAMP			3                         // Pasar a 5 para EC
#define NUM_SAMP				  (1<<LOG2_NUM_SAMP)//+2    // Se puedenSUMAN 2 para eliminar los extremos y promediar

#define ADC_RES           4095
#define REF_VOLTAGE       3.3  //V   Valor preciso:  3.3v*Vref_cal/Vref_data
                                //        Vref_cal  = Valor en memoria
                                //        Vref_data = ADC_CHANNEL_VREFINT

/*States ---------------------------------------------------------------------*/
enum{
  ADC_WAITING,
  ADC_SAMPLING_VREF,
  ADC_SAMPLING_TEMP,
  ADC_SAMPLING_PH,
  ADC_SAMPLING_EC
};

#endif  /* __ADC_SAMPLE_H  */