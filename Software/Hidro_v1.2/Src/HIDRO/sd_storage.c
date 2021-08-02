/* Includes ------------------------------------------------------------------*/
#include "sd_storage.h"

/* Private Functions ---------------------------------------------------------*/
void get_data_csv(char *);

/* Private Data --------------------------------------------------------------*/
uint8_t SD_State = 0;
uint32_t SD_Timer;
uint32_t SD_Timer_test;   // La uso ahora para probar que grabe cada 30'

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_SD;

/* Functions -----------------------------------------------------------------*/
void sd_init(void)
{
  FATFS SDFatFS;                   /* File system object for SD logical drive */
  uint8_t rtext[_MAX_SS];                           /* File read buffer */
  uint8_t wtext[] = "Se va a generar un archivo .csv por mes";   /* File write buffer */
  uint32_t byteswritten;                            /* File write/read counts */
      
//  Format the default drive 
  if(f_mkfs( (TCHAR const*) SDPath, FM_ANY, 0, rtext, sizeof(rtext)) != FR_OK)
    Error_Handler();
  
//  Moutn SD  
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
    Error_Handler();
    
//  Open file for writing (Create)
  if(f_open(&SDFile, "Readme.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    Error_Handler();

//  Write to the text file
  if(f_write(&SDFile, wtext, strlen((char *)wtext), (void *)&byteswritten) != FR_OK)
    Error_Handler();
  if(byteswritten == 0)
    Error_Handler();

//  Close file for writing (Create)    
  f_close(&SDFile);

//  Unmoutn SD
  f_mount(&SDFatFS, (TCHAR const*)NULL, 0);

  return;
}

void sd_1s_tick(void)
{
  if(!SD_Timer)
    SD_Timer--;
}

void sd_task(void)
{
  FATFS SDFatFS;                   /* File system object for SD logical drive */
  uint8_t rtext[_MAX_SS];                           /* File read buffer */
  uint8_t wtext[] = "DD,HH,MM,TT,HH,EEEE,PH,TT";    /* File write buffer */
  uint32_t byteswritten;                            /* File write/read counts */
  static uint8_t month = 0;
  static char f_name[] = "XX_XX.txt";
  char aux;
  
  switch(SD_State)
  {
  case SD_WAITING:
    if(!Flag_ADC_Electrode_Ready)
      break;

    if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
      Error_Handler();
    Flags_SD_Mount = 1; 
    if(month != Global_Date[MONTH])
    {
      aux = (char)bin_to_bcd(Global_Date[YEAR]);
      f_name[0] = 0x30 + (aux >> 4);
      f_name[1] = 0x30 + (aux & 0x0F);
      aux = (char)bin_to_bcd(Global_Date[MONTH]);
      f_name[3] = 0x30 + (aux >> 4);
      f_name[4] = 0x30 + (aux & 0x0F);
      
      if(f_open(&SDFile, f_name, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        Error_Handler();
    }
    else
    {
      if(f_open(&SDFile, f_name, FA_OPEN_APPEND | FA_WRITE) != FR_OK)
        Error_Handler();
    }
    SD_State = SD_READY;
    break;
    
  case SD_READY:
    if(!Flag_ADC_Electrode_Ready)
    {
      SD_State = SD_CLOSE;
      break;      
    }
    
    if(Flag_ADC_Event_Storage)
    { 
      Flag_ADC_Event_Storage = 0;
      get_data_csv(wtext);
      if(f_write(&SDFile, wtext, strlen((char *)wtext), (void *)&byteswritten) != FR_OK)
        Error_Handler();
      if(byteswritten == 0)
        Error_Handler();
      SD_State = SD_CLOSE;
    }
    break;   
    
  case SD_CLOSE:
    f_close(&SDFile);
    f_mount(&SDFatFS, (TCHAR const*)NULL, 0);
    Flags_SD_Mount = 0; 
    SD_State = SD_WAITING;
    break;     
    
  default:
    SD_State = SD_CLOSE;    
  }  
}

/* Private Functions ---------------------------------------------------------*/
void get_data_csv(char *wtext)
{
  // wtext[] = "DD,HH,MM,TT,HH,EEEE,PH,TT"; 
  char aux;
  char *str;
  
  aux = (char)bin_to_bcd(Global_Date[DAY]);
  wtext[0] = 0x30 + (aux >> 4);
  wtext[1] = 0x30 + (aux & 0x0F);
  aux = (char)bin_to_bcd(Global_Date[HOUR]);
  wtext[3] = 0x30 + (aux >> 4);
  wtext[4] = 0x30 + (aux & 0x0F);
  aux = (char)bin_to_bcd(Global_Date[MIN]);
  wtext[6] = 0x30 + (aux >> 4);
  wtext[7] = 0x30 + (aux & 0x0F);
  sprintf(str, "%2d", temperature);
  wtext[9] = str[0];
  wtext[10] = str[1];
  sprintf(str, "%2d", humidity);
  wtext[9] = str[0];
  wtext[10] = str[1];
  
}