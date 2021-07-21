/*
El sistema funciona de a ciclos:
  PREPARADO - ESPERAR - REGAR

- La única forma de pararlo es cuando termina un ciclo, es decir, 
después de que termine de regar, antes de preparar.
 Si bien hace que tarde en pausar el equipo, esto es para evitar que 
se ponga en pausa en cualquier momento con todo andando distinto

- La fecha de finalización la revisa luego de terminar cada proceso
de regado, en el último estado. Lo que hace es comparar la fecha actual
con la fecha de inicio más la duración del cultivo (ID_Grow_Time).
 No elegí cargar la fecha de final como Ini+Delta de una por si en algún
momento quiero cambiar y que al stopear tambien pare de contar el tiempo
(eso es para analizar despues, de momento parecería que con tan poco no 
va a cambiar)

*/

/* Includes ------------------------------------------------------------------*/
#include "irrigation.h"

/* Private Data --------------------------------------------------------------*/
uint8_t Irrigation_State = 0;
uint32_t Irrigation_Timer, No_Flow_Timer;

/* Exported Data -------------------------------------------------------------*/
volatile tREG08 Flags_Irrigation;
uint8_t Initial_Date[6];

/* Functions -----------------------------------------------------------------*/
uint8_t final_day(void)
{
  uint8_t final_date[6];
  
  add_days(final_date, Initial_Date, read_flash(ID_Grow_Time));
    
  if(final_date[DAY] == Global_Date[DAY])
  {
    if(final_date[MONTH] == Global_Date[MONTH])
    {
      if(final_date[YEAR] == Global_Date[YEAR])
        return 1;
    }
  }
  return 0;    
}

void open_water_tap(void)
{
  
}

void close_water_tap(void)
{
  
}
      
void operate_tank(uint8_t substance, uint8_t action)
{
  switch(substance)
  {
  case SOLUTION:
    HAL_GPIO_WritePin(O_PUMP_SOLU_GPIO_Port, O_PUMP_SOLU_Pin, (GPIO_PinState)action);
    break;
  case MICRO_NUT:
    HAL_GPIO_WritePin(O_PUMP_MICRO_GPIO_Port, O_PUMP_MICRO_Pin, (GPIO_PinState)action);
    break;
  case MACRO_NUT:
    HAL_GPIO_WritePin(O_PUMP_MACRO_GPIO_Port , O_PUMP_MACRO_Pin, (GPIO_PinState)action);
    break;
  case HERBICIDE:
    HAL_GPIO_WritePin(O_PUMP_HERB_GPIO_Port, O_PUMP_HERB_Pin, (GPIO_PinState)action);
    break;
  case ACIDULANT:
    HAL_GPIO_WritePin(O_PUMP_ACID_GPIO_Port, O_PUMP_ACID_Pin, (GPIO_PinState)action);
    break;
  case WATER:
    if(action == CLOSE)
      close_water_tap();
    else
      open_water_tap();
    break;    
  }
}

void mixer(uint8_t state)
{
  if(state)
    HAL_GPIO_WritePin(O_MIXER_GPIO_Port, O_MIXER_Pin, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(O_MIXER_GPIO_Port, O_MIXER_Pin, GPIO_PIN_RESET);    
}
                
void rail(uint8_t state)
{
  switch(state)
  {
  case DIP:   
    break;
  case ASCEND:
    break;
  case OFF: 
    break;
  }
}
 
void irrigation_status(char* buffer)
{
  if(!Flag_Irriga_Growing)
    sprintf(buffer, "SinCul ");
  else if(!Flag_Irriga_Operate)
    sprintf(buffer, "Pausa  ");
  else
  {
    switch(Irrigation_State) 
    {        
    case IRRIGA_INIT:
      sprintf(buffer, "Ini    ");
      break;
    case IRRIGA_REVOLVING:
    case IRRIGA_REVOLVING_2:
      sprintf(buffer, "Revol  ");
      break;
    case IRRIGA_DIPPING_ELECTRODE:
    case IRRIGA_ASCENDING_ELECTRODE:
      sprintf(buffer, "MovE   ");
      break;
    case IRRIGA_FILLING:
    case IRRIGA_STABILIZING_ELECTRODE:
    case IRRIGA_SAMPLING_PH:
    case IRRIGA_REGULATING_PH:
    case IRRIGA_SAMPLING_EC:
    case IRRIGA_REGULATING_EC:
      sprintf(buffer, "Prep   ");
      break;
    case IRRIGA_RESTING:
      sprintf(buffer, "Espe   ");
      break;
    case IRRIGA_FLOWING:
      sprintf(buffer, "Riego  ");
      break;        
    }  
  }
}

void irrigation_1s_tick(void)
{
	if(Irrigation_Timer)
    Irrigation_Timer--;
  if(No_Flow_Timer)
    No_Flow_Timer--;
}

void irrigation_task(void)
{
  static uint8_t aux_substance;
  
  switch(Irrigation_State)
  {    
  case IRRIGA_INIT:   
    if(!Flag_Irriga_Growing) 
      break;
    if(!Flag_Irriga_Operate)
      break;
    
    // Comienza el tiempo que NO está regando
    if(Global_Date[HOUR]>= read_flash(ID_Sunrise) && Global_Date[HOUR]<read_flash(ID_Sunset))
      No_Flow_Timer = read_flash(ID_Time_No_Flow_Day);
    else
      No_Flow_Timer = read_flash(ID_Time_No_Flow_Night);
           
    // Compruebo el nivel del tanque principal
    if(!Flag_Di_Level_Solucion)
    {
      Irrigation_State = IRRIGA_FILLING;
      open_water_tap();
      break;
    }

    // Si el nivel esta OK  => Revuelvo y comienzo a medir
    mixer(ON);
    Irrigation_State = IRRIGA_REVOLVING;
    Irrigation_Timer = T_REVOLVE_SOLUTION;    
    break;
    
  case IRRIGA_FILLING:
    if(Flag_Di_Level_Solucion)
    {
      close_water_tap();
      mixer(ON);
      Irrigation_State = IRRIGA_REVOLVING;
      Irrigation_Timer = T_REVOLVE_SOLUTION;
    }
    break;
    
  case IRRIGA_REVOLVING:
    if(!Irrigation_Timer)
    {
      mixer(OFF);
      rail(DIP);
      Irrigation_State = IRRIGA_DIPPING_ELECTRODE;
      Irrigation_Timer = T_MOVE_ELECTRODE;             // Ver que es mejor, si por tiempo bajar las sondas o con otro sensor
    }
    break;
    
  case IRRIGA_DIPPING_ELECTRODE:
    if(!Irrigation_Timer)
    {
      rail(OFF);
      Irrigation_State = IRRIGA_STABILIZING_ELECTRODE;
      Irrigation_Timer = T_STABILIZE;
    }
    break;
    
  case IRRIGA_STABILIZING_ELECTRODE:
    if(!Irrigation_Timer)
    {
      Flag_ADC_Electrode_Ready = 1;
      adc_new_sample(ADC_CMD_PH);
      Irrigation_State = IRRIGA_SAMPLING_PH;
    }
    break;  
    
  case IRRIGA_SAMPLING_PH:    
    // Espero obtener la muestra
    if(!adc_done(ADC_CMD_PH))
      break;
    
    // Si el pH está fuera de rango (menor o mayor)
    if(ph <= (float)(read_flash(ID_Ph_Min)/10))
    {
      Flag_Irriga_Ph_L = 1;
      aux_substance = WATER;
      operate_tank(aux_substance, OPEN);      
      Irrigation_State = IRRIGA_REGULATING_PH;
      Irrigation_Timer = T_OPEN_TAP;
      break;
    }
    Flag_Irriga_Ph_L = 0;
    
    if (ph >= (float)(read_flash(ID_Ph_Max)/10))
    {
      Flag_Irriga_Ph_H = 1;
      aux_substance = ACIDULANT;
      operate_tank(aux_substance, OPEN); 
      Irrigation_State = IRRIGA_REGULATING_PH;
      Irrigation_Timer = T_OPEN_TANK;
      break;
    }
    Flag_Irriga_Ph_H = 0;
    
    // Si el pH está en el rango
    Irrigation_State = IRRIGA_SAMPLING_EC;
    adc_new_sample(ADC_CMD_EC);
    break;      
     
  case IRRIGA_REGULATING_PH:
    if(!Irrigation_Timer)
    {      
      operate_tank(aux_substance, CLOSE); 
      adc_new_sample(ADC_CMD_PH);
      Irrigation_State--;
    }
    break; 
    
  case IRRIGA_SAMPLING_EC:    
    // Espero obtener la muestra
    if(!adc_done(ADC_CMD_EC))
      break;
    
    // Si la conductividad está fuera de rango (menor o mayor)
    if(ec < read_flash(ID_Ec_Min))
    {
      Flag_Irriga_Ec_L = 1;
      aux_substance = MICRO_NUT;                  //PREGUNTAR MICRO, MACRO O LOS DOS???
      operate_tank(aux_substance, OPEN);      
      Irrigation_State = IRRIGA_REGULATING_EC;
      Irrigation_Timer = T_OPEN_TANK;
      break;
    }
    Flag_Irriga_Ec_L = 0;
    
    if (ec > read_flash(ID_Ec_Max))
    {
      Flag_Irriga_Ec_H = 1;
      aux_substance = WATER;
      operate_tank(aux_substance, OPEN); 
      Irrigation_State = IRRIGA_REGULATING_EC;
      Irrigation_Timer = T_OPEN_TAP;
      break;
    }
    Flag_Irriga_Ec_H = 0;
    
    // Si la conductividad está en el rango
    rail(ASCEND);
    Flag_ADC_Electrode_Ready = 0;
    Irrigation_State = IRRIGA_ASCENDING_ELECTRODE;
    Irrigation_Timer = T_MOVE_ELECTRODE;    
    break;
    
  case IRRIGA_REGULATING_EC:
    if(!Irrigation_Timer)
    {
      operate_tank(aux_substance, CLOSE); 
      adc_new_sample(ADC_CMD_EC);
      Irrigation_State--;
    }
    break; 
    
  case IRRIGA_ASCENDING_ELECTRODE:    
    if(!Irrigation_Timer)
    {
      rail(OFF);
      operate_tank(SOLUTION, OPEN);
      Irrigation_State = IRRIGA_RESTING;
    }
    break;
    
  case IRRIGA_RESTING:
    //Para dejar de descanzar me fijo el tiempo que falta para NoFlow se cumpla
    // teniendo en cuenta que falta revolver antes
    if(No_Flow_Timer<T_REVOLVE_SOLUTION)
    {
        mixer(ON);
        Irrigation_State = IRRIGA_REVOLVING_2;
    }    
    break;
    
  case IRRIGA_REVOLVING_2: 
    if(!No_Flow_Timer)
    {
      mixer(OFF);
      Irrigation_State = IRRIGA_FLOWING;        
      if(Global_Date[HOUR]>= read_flash(ID_Sunrise) && Global_Date[HOUR]<read_flash(ID_Sunset))
        Irrigation_Timer = read_flash(ID_Time_Flow_Day);
      else
        Irrigation_Timer = read_flash(ID_Time_Flow_Night);
    }
    break;
    
  case IRRIGA_FLOWING:
    if(!Irrigation_Timer)
    {      
      operate_tank(SOLUTION, CLOSE);
      if(final_day())
      {
        Flag_Irriga_Growing = 0;
        Flag_Irriga_Operate = 0;
      }
      Irrigation_State = IRRIGA_INIT;
    }
    break;
    
  default:
      Irrigation_State = IRRIGA_INIT;
  }      
}