#ifndef __SD_STORAGE_H
#define __SD_STORAGE_H

/* Includes ------------------------------------------------------------------*/
#include "adc_sample.h"
#include "common.h"
#include "datetime.h"
#include "fatfs.h"
#include "main.h"
#include "sdmmc.h"
#include "weather.h"

/* Function ------------------------------------------------------------------*/
void sd_init(void);
void sd_task (void);
void sd_1s_tick (void);

/* Exported Variables --------------------------------------------------------*/
extern volatile tREG08 Flags_SD;

/* Defines -------------------------------------------------------------------*/
#define Flags_SD_Mount              Flags_SD.Bit._0

/*States ---------------------------------------------------------------------*/
enum{
  SD_WAITING,
  SD_READY,
  SD_CLOSE
};

#endif  /* __ADC_SAMPLE_H  */