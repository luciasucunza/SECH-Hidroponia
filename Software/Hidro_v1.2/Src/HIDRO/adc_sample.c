/* Includes ------------------------------------------------------------------*/
#include "adc_sample.h"

/* Private Functions ---------------------------------------------------------*/
void adc_select_channel (uint32_t);
bool start_sampling (uint32_t );
uint32_t finish_sampling (void);
float sample_math (uint32_t, uint32_t, float);

/* Private Data --------------------------------------------------------------*/
uint8_t ADC_State = 0;
//uint32_t ADC_Timer;

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_ADC;
float ph = 0;
float ec = 0;
float vdd = REF_VOLTAGE;
float sol_temp = 0;

/* Functions -----------------------------------------------------------------*/
void adc_new_sample(uint8_t cmd)
{
	Flags_ADC.Byte |= cmd;
}

bool adc_done(uint8_t cmd)
{
	if(Flags_ADC.Byte & cmd) 
  {
    return false;
  }
	return true;
}

//void adc_1s_tick(void)
//{
//  if(!ADC_Timer)
//    ADC_Timer--;
//}


void adc_task(void)
{
  uint8_t i;
  uint32_t adc_raw = 0;
  
  switch(ADC_State)
  {
  case ADC_WAITING:
    
    if(!Flag_ADC_Electrode_Ready)
    break;
    
    // Podr�a alimentarlos a toos por separado, pero mas o menos toman muestras
    // en los mismos lapsos de tiempo, as� que al pedo, directo prender todos aca
//      HAL_GPIO_WritePin (ENA_P0_Port, ENA_P0_Pin, GPIO_PIN_SET);	Para alimentar adecuadamente el sensor
    
    //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! 
    // Flag_ADC_Electrode_Ready = 0;            //BORRAR, SOLO PARA PUREBA
    //! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !

    if(Flag_ADC_Cmd_Ph)                  // Pedido de valor de ph
    {
      ADC_State = ADC_SAMPLING_VREF;
      break;
    }
    if(Flag_ADC_Cmd_Ec)                  // Pedido de valor de conductividad
    {
      ADC_State = ADC_SAMPLING_VREF;
      break;
    }    
    break;
    
    
  case ADC_SAMPLING_VREF:
    for(i=0 ; i<NUM_SAMP ; i++)           // Tomar n muestras y promediar
    {
      if(start_sampling(ADC_CHANNEL_VREFINT))           // Iniciar ADC
      {
          Flag_ADC_Error_Vrefint = 0;
          adc_raw += finish_sampling();                  // Obtener muestra
      }
        else
          Flag_ADC_Error_Vrefint = 1;
    }
    adc_raw >>= LOG2_NUM_SAMP;
    vdd =sample_math( adc_raw, ADC_CHANNEL_VREFINT, 0);
    
    ADC_State = ADC_SAMPLING_TEMP;
    break;
    
    
  case ADC_SAMPLING_TEMP:
    for(i=0 ; i<NUM_SAMP ; i++)           // Tomar n muestras y promediar
    {
      if(start_sampling(CH_TEMP))           // Iniciar ADC
      {
        Flag_ADC_Error_Temp = 0;
        adc_raw += finish_sampling();     // Obtener muestra
      }
      else
        Flag_ADC_Error_Temp = 1;
    }
    adc_raw >>= LOG2_NUM_SAMP;
    sol_temp = sample_math(adc_raw, CH_TEMP, 0);
    
    if(Flag_ADC_Cmd_Ph)                  // Pedido de valor de ph
    {
      ADC_State = ADC_SAMPLING_PH;
      break;
    }
    if(Flag_ADC_Cmd_Ec)                  // Pedido de valor de conductividad
    {
      ADC_State = ADC_SAMPLING_EC;
      break;
    }    
    break;
    
  case ADC_SAMPLING_PH:
    for(i=0 ; i<NUM_SAMP ; i++)           // Tomar n muestras y promediar
    {
      if(start_sampling(CH_PH))           // Iniciar ADC
      {
        Flag_ADC_Error_Ph = 0;
        adc_raw += finish_sampling();     // Obtener muestra
      }
      else
        Flag_ADC_Error_Ph = 1;
    }
    adc_raw >>= LOG2_NUM_SAMP;
    ph = sample_math(adc_raw, CH_PH, sol_temp);
    
    Flag_ADC_Cmd_Ph = 0;                 // Borrar pedido
    
    if(Flag_ADC_Cmd_Ec)        // Pedido de valor de conductividad
    {
      ADC_State = ADC_SAMPLING_EC;
      break;
    }
    
    Flag_ADC_Event_Storage = 1;           // Antes de esperar aviso
    ADC_State = ADC_WAITING;        
    break;
  
  case ADC_SAMPLING_EC:     
    for(i=0 ; i<NUM_SAMP ; i++)           // Tomar n muestras y promediar
    {
      if(start_sampling(CH_EC)) // Iniciar ADC
      {
        Flag_ADC_Error_Ec = 0;
        adc_raw += finish_sampling();		  // Obtener muestra
      }
      else
      {
        Flag_ADC_Error_Ec = 1;
      }
    }
    adc_raw >>= LOG2_NUM_SAMP;          
    ec = sample_math(adc_raw, CH_EC, sol_temp);
    
    //Done
    Flag_ADC_Cmd_Ec = 0;                 // Borrar pedido
       
    Flag_ADC_Event_Storage = 1;          // Antes de esperar aviso 
    ADC_State = ADC_WAITING;
    break;
    
  default:
    ADC_State = ADC_WAITING;
   
  }
}

/* Private Functions ---------------------------------------------------------*/
void adc_select_channel (uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
  /** Configure Regular Channel
  */
  sConfig.Channel = channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

bool start_sampling(uint32_t channel)
{
  adc_select_channel(channel);
    
  if(HAL_ADC_Start(&hadc1) != HAL_OK)
  {
    return false;
  }  
  return true;    
}

uint32_t finish_sampling(void)
{
  uint32_t raw;
  
  if(HAL_ADC_PollForConversion(&hadc1, 3) != HAL_OK)
  {
	  HAL_ADC_Stop(&hadc1);
    return false;
  }
  
  raw = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
  
  return raw;
  
}

float sample_math(uint32_t adc_data, uint32_t ch_type, float temp)
{
  float v_in;
  float value;
  
  
  switch(ch_type)
  {
  case ADC_CHANNEL_VREFINT:
    value = REF_VOLTAGE*(* VREFINT_CAL_ADDR)/adc_data;
    break;
  case CH_TEMP:
    v_in = vdd*(float)adc_data/ADC_RES;
    //value = v_in*lm35_resolution;
    value = v_in*10;
    break;
  case CH_PH:
    v_in = vdd*(float)adc_data/ADC_RES;
    //value = ((float)adc_data*read_flash(ID_Ph_Slope) - read_flash(ID_Ph_Y_Intercept))/1000;
    value = v_in;
    break;
  case CH_EC:
    v_in = vdd*(float)adc_data/ADC_RES;
    //value = (float)adc_data*read_flash(ID_Ec_Slope) +  read_flash(ID_Ec_Y_Intercept);
    value = v_in;
    break;
  }
  return value;
}
