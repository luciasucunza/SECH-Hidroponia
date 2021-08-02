/* Includes ------------------------------------------------------------------*/
#include "ui.h"
extern volatile tREG32 Flags;
  
/* Private Functions ---------------------------------------------------------*/
uint8_t filter_chars_inc(uint16_t);
uint8_t filter_chars_dec(uint16_t);

/* Extern Data ---------------------------------------------------------------*/
uint8_t User_Logged_In = USR_NONE;

/* Private Data --------------------------------------------------------------*/
uint8_t Ui_State=0;
uint8_t Ui_Timer;
uint8_t Ui_User_Login;
uint32_t Ui_Sleep;

/* Private Constant ----------------------------------------------------------*/
const uint8_t user_pass_id_0[] = {ID_Pass_Op1_0, ID_Pass_Op2_0, ID_Pass_Sup1_0, ID_Pass_Sup2_0};
const uint8_t master_pass[] = {'0','0','1','0','1','0','1','0'}; //42

/* Functions -----------------------------------------------------------------*/
void ui_init(void)
{  
  User_Logged_In = USR_NONE;
  Ui_State = 0;
  Ui_Timer = HAL_GetTick();
  Ui_Sleep = T_LOW_POWER;
}

void ui_wake(void)
{
  Ui_State = 0;
  Ui_Timer = HAL_GetTick();
  Ui_Sleep = T_LOW_POWER;
}

void ui_1s_tick(void)
{
  if(Ui_Sleep)
  {
    Ui_Sleep--;
  }
  else
  {
    // Refrescar los botones, si uno es precionado, entonces vuelvo a encender
    //process_buttons();
  }
}

void ui_task(void)
{
  static uint8_t Ui_Disp_Version[LINE_SIZE+1];
  static uint8_t Ui_Disp_Buffer[LINE_SIZE+1];
  static uint8_t Ui_Clock_Buffer[6];
  static uint8_t Ui_Index;
  static uint8_t Ui_Menu_Index;
  static uint8_t Ui_Config_Var, Ui_Config_Ini, Ui_Config_Max;
  static uint8_t id_offset;
     
  //--------------------------------------
  //    ESTO SE VA A DE ACÁ ES SOLO PORQUE NO ESTÁ EN FUNCIONAMIENTO IRRIGA
  //--------------------------------------
  Flag_Weather_Hum_L = 1;
  Flag_Weather_Temp_L = 1;
  Flag_Irriga_Ph_H = 1;
  Flag_Irriga_Ec_H = 1;
  Flag_Di_Level_Micro_Nut = 1;
  Flag_Di_Level_Macro_Nut = 1;
  Flag_Di_Level_Herbicide = 1;
  Flag_Di_Level_Acidulant = 1;
  Flags_SD_Mount = 1;
  
  
  // Modo: Low power -----------------------------------------------------------
  if(!Ui_Sleep)
  {
    if(Flag_LCD_On)             // Apagar LCD
    {
      User_Logged_In = USR_NONE;
      Ui_State = UI_MAIN_BUTTON_A;
      LCDSleep();
      Flag_LCD_On = 0;
    }        
    
    if(!Flag_Button_M)
      ui_wake();
    
    return;
  }
  
  // Modo: Normal --------------------------------------------------------------
  if(!Flag_LCD_On)              // Encender LCD
  {
    LCDInit();
    Flag_LCD_On = 1;
  }
  
  if((Flags_Buttons.Byte & BUTTON_MASK) != BUTTON_MASK)
    Ui_Sleep = T_LOW_POWER;	                     // Recargar el timer Low-Power

	if((HAL_GetTick() - Ui_Timer) >= UI_TIME)      // Tick 250ms 
	{
    Ui_Timer = HAL_GetTick(); 
        
    // Pantalla Principal ------------------------------------------------------
    if((Ui_State==UI_MAIN_BUTTON_A) || (Ui_State==UI_MAIN_BUTTON_B))
    {
      LCDClearLines(L0, L7);
      /* Linea 0 -------------------------------------------------------------*/
      /* Linea 1 -------------------------------------------------------------*/
      for (Ui_Index = 0; Ui_Index < NAME_SIZE; Ui_Index++)
        Ui_Disp_Buffer[Ui_Index] = (uint8_t)read_flash(ID_Recipe1_0 + Ui_Index);
      Ui_Disp_Buffer[NAME_SIZE] = '\0';
      
      LCDStr_2(0, L0, Ui_Disp_Buffer);
      
      if(Flags_SD_Mount)
        LCDStr(18, L1, "SD" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(18, L1, "  " );             // Blink alarma
      
      /* Linea 2 -------------------------------------------------------------*/
      sprintf ((char*)Ui_Disp_Buffer, "PH  %.2f (%.1f-%.1f)", ph, (float)read_flash(ID_Ph_Min)/10, (float)read_flash(ID_Ph_Max)/10);
      LCDStr(0,L2,Ui_Disp_Buffer);
      if(Flag_Irriga_Ph_L || Flag_Irriga_Ph_H)
      {
        if(Global_Date[SEC] & 0x01)
          LCDStr(0, L2, "  " );             // Blink alarma 
      }
      
      /* Linea 3 -------------------------------------------------------------*/
      sprintf ((char*)Ui_Disp_Buffer, "EC  %.2f (%u-%u)", ec, read_flash(ID_Ec_Min), read_flash(ID_Ec_Max));
      LCDStr(0,L3,Ui_Disp_Buffer);
      if(Flag_Irriga_Ec_L || Flag_Irriga_Ec_H)
      {
        if(Global_Date[SEC] & 0x01)
          LCDStr(0, L3, "  " );             // Blink alarma 
      }
      
      /* Linea 4 -------------------------------------------------------------*/    
      if(Flag_Di_Level_Micro_Nut)
        LCDStr(14, L4, "MIC" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(14, L4, "   " );             // Blink alarma
      if(Flag_Di_Level_Macro_Nut)
        LCDStr(18, L4, "MAC" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(18, L4, "   " );             // Blink alarma
      
      /* Linea 5 -------------------------------------------------------------*/
      sprintf((char*)Ui_Disp_Buffer, "Tem  %dC", temperature);
      LCDStr(0,L5,Ui_Disp_Buffer);
      if(Flag_Weather_Temp_H)
        LCDStr(9, L5, "T" );
      else if(Flag_Weather_Temp_L)
        LCDStr(9, L5, "t" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(9, L5, " " );             // Blink alarma
      
      if(Flag_Di_Level_Herbicide)
        LCDStr(14, L5, "HER" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(14, L5, "   " );             // Blink alarma
      if(Flag_Di_Level_Acidulant)
        LCDStr(18, L5, "ACI" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(18, L5, "   " );             // Blink alarma
         
      
      /* Linea 6 -------------------------------------------------------------*/
      sprintf((char*)Ui_Disp_Buffer, "Hum  %d%", humidity);
      LCDStr(0,L6,Ui_Disp_Buffer);
      if(Flag_Weather_Hum_H)
        LCDStr(9, L6, "H" );
      else if(Flag_Weather_Hum_L)
        LCDStr(9, L6, "h" );
      if(Global_Date[SEC] & 0x01)
        LCDStr(9, L6, " " );             // Blink alarma 
      
      irrigation_status((char*)Ui_Disp_Buffer);
      LCDStr(14, L6, Ui_Disp_Buffer);
      
      /* Linea 7 -----Clock---------------------------------------------------*/
      clock_to_ascii(Ui_Disp_Buffer, Global_Date);   // Recargar la hora
      LCDStr(0, L7, Ui_Disp_Buffer);
      if(Global_Date[SEC] & 0x01)
        LCDStr(11, L7, " " );             // Blink ':' 
    }

    // Pantalla Configuración --------------------------------------------------
    else
    {      
      LCDClearLines(L0, L3);
      
      /* Linea 0 -------------------------------------------------------------*/
      LCDStr(0, L0, "HIDRO" );
      
      /* Linea 1 -------------------------------------------------------------*/
      sprintf((char *)Ui_Disp_Version, "v%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, REVISION_VERSION);
      LCDStr(0, L1, Ui_Disp_Version);

      /* Linea 2 -------------------------------------------------------------*/
      /* Linea 3 -------------------------------------------------------------*/
      /* Linea 4 -------------------------------------------------------------*/
      /* Linea 5 -------------------------------------------------------------*/
      /* Linea 6 -------------------------------------------------------------*/
      /* Linea 7 -------------------------------------------------------------*/
    }
  }
  
  switch (Ui_State)
  {    
  //----------------------------------------------------------------------------
  case UI_MAIN_BUTTON_B:
    if(!Flag_Button_M)
    {
      LCDClearLines(L0, L7);  
      Ui_Menu_Index = 0;    
      if(User_Logged_In != USR_NONE)
      {
        Ui_State = UI_ENTER_SCREEN_STOPPED;
      }
      else
      {
        Ui_State = UI_USER_0;
      }
    }
    break;
    
  //----------------------------------------------------------------------------
  case UI_USER_0:
    LCDStr(0, L4, "Seleccione usuario");
    Ui_Menu_Index = 0;
    Ui_State++;
    break;

  case UI_USER_1:
    switch (Ui_Menu_Index)
    {
    case M_USR_OP1: 	LCDStr(0, L5, "Operador 1    "); break;
    case M_USR_OP2: 	LCDStr(0, L5, "Operador 2    "); break;
    case M_USR_SUP1: 	LCDStr(0, L5, "Supervisor 1  "); break;
    case M_USR_SUP2: 	LCDStr(0, L5, "Supervisor 2  "); break;
    case M_USR_MASTER:LCDStr(0, L5, "Master        "); break;
    }
    Ui_State++;
    break;
  
  case UI_USER_BUTTON_B:    
    if(!Flag_Button_M)        
    {
      LCDClearLines(L4, L7);
      Ui_State = UI_MAIN_BUTTON_A;
      break;
    }
    if(!Flag_Button_I)
    {
      if(Ui_Menu_Index < M_USR_MASTER)
      {
        Ui_Menu_Index++;
        Ui_State -= 2;
      }
      break;
    }
    if(!Flag_Button_D)
    {
      if(Ui_Menu_Index > M_USR_OP1)
      {
        Ui_Menu_Index--;
        Ui_State -= 2;
      }
      break;
    }    
    if(!Flag_Button_R)
    {
      LCDClearLines(L4, L7);
      Ui_State = UI_LOGIN_0;
      switch (Ui_Menu_Index)
      {
      case M_USR_OP1:   Ui_User_Login = USR_OP1;   break;
      case M_USR_OP2:   Ui_User_Login = USR_OP2;   break;
      case M_USR_SUP1:  Ui_User_Login = USR_SUP1;  break;
      case M_USR_SUP2:  Ui_User_Login = USR_SUP2;  break;
      case M_USR_MASTER:Ui_User_Login = USR_MASTER;break;
      default:
        Ui_State = UI_USER_BUTTON_A;
      }
    }
    break; 
    
  case UI_LOGIN_0:
    for(Ui_Index = 0; Ui_Index < PASS_SIZE; Ui_Index++)
      Ui_Disp_Buffer[Ui_Index] = ' ';

    Ui_Disp_Buffer[PASS_SIZE] = '\0';
    Ui_Index=0;
    LCDStr(0, L4, "Ingrese clave" );
    Ui_State++;
    break;
    
  case UI_LOGIN_1:
    LCDStr(0, L5, Ui_Disp_Buffer);
    LCDBlinkOn(Ui_Index, L5, &Ui_Disp_Buffer[Ui_Index], 1);
    Ui_State++;
    break;
    
  case UI_LOGIN_BUTTON_B:
    if((!Flag_Button_I) && (Ui_Index < PASS_SIZE))
    {
      Ui_Disp_Buffer[Ui_Index]++;
      Ui_Disp_Buffer[Ui_Index] = filter_chars_inc(Ui_Disp_Buffer[Ui_Index]);        
      Flag_LCD_Update = 1;
      Ui_State -= 2;
      break;
    }
    
    if((!Flag_Button_D) && (Ui_Index < PASS_SIZE))
    {
      Ui_Disp_Buffer[Ui_Index]--;
      Ui_Disp_Buffer[Ui_Index] = filter_chars_dec(Ui_Disp_Buffer[Ui_Index]);
      Flag_LCD_Update = 1;
      Ui_State -= 2;
      break;
    }
    
    if(!Flag_Button_R  && Ui_Index > 0)
    {
      Ui_Index--;
      Ui_State -=2;
      break;
    }
    
    if(!Flag_Button_M)
    {
      Ui_Index++;
      if(Ui_Index >= PASS_SIZE)
      {
        LCDClearLines(L4, L7);
        for(Ui_Index = 0; Ui_Index < PASS_SIZE; Ui_Index++)	  // Password ok?			
        {
          if(Ui_User_Login == USR_MASTER)
          {
            if(Ui_Disp_Buffer[Ui_Index] != master_pass[Ui_Index])
              break;
          }
          else
          {
            if( Ui_Disp_Buffer[Ui_Index] != (uint8_t)read_flash(user_pass_id_0[Ui_User_Login-1] + Ui_Index))
              break;
          }
        }
        if(Ui_Index == PASS_SIZE)
        {
          User_Logged_In = Ui_User_Login; // Logged in
          Ui_Menu_Index = 0;
          
          Ui_State = UI_ENTER_SCREEN_STOPPED;
          break;
        }
        Ui_State = UI_MAIN_BUTTON_A;
        break;
      }
      Ui_State -= 2;
      break;
    }    
    break;  

  //----------------------------------------------------------------------------
  case UI_ENTER_SCREEN_STOPPED:
      switch(Ui_Menu_Index)
      {
      case MS_START: 	 
        if(!Flag_Irriga_Growing) 
          LCDStr(0, L4, "Iniciar       "); 
        else if(!Flag_Irriga_Operate) 
          LCDStr(0, L4, "Operar        "); 
        else
          LCDStr(0, L4, "Pausar        "); 
        break;
      case MS_WEATHER: LCDStr(0, L4, "Clima Ambiente"); break;
      case MS_RECIPE_1:LCDStr(0, L4, "Receta N 1    "); break;
      case MS_RECIPE_2:LCDStr(0, L4, "Receta N 2    "); break;
      case MS_SYS:     LCDStr(0, L4, "Sistema       "); break;
      }
      LCDStr(0, L5, "              ");
      Ui_State++;
      break;
      
  case UI_ENTER_BUTTON_STOPPED_B:
      if(!Flag_Button_M)
      {
        LCDClearLines(L4, L7);
        Ui_Menu_Index++;        
        if(Ui_Menu_Index >= MS_LAST)
        {
          LCDClearLines(L4, L7);          
          Ui_State = UI_MAIN_BUTTON_A;
        }
        else
          Ui_State = UI_ENTER_SCREEN_STOPPED;
        break;
      }

      if(!Flag_Button_R)
      {
        LCDClearLines(L4, L7);
        switch(Ui_Menu_Index)
        {
        case MS_START:
          if(User_Logged_In != USR_NONE)
          {
            if(!Flag_Irriga_Growing) 
            {
              Flag_Irriga_Growing = 1;
              Flag_Irriga_Operate = 0;  //Con esto en la siguiente linea cuando toggleo queda bien
              memcpy(Initial_Date , Global_Date, 6);
            }
            if(!Flag_Irriga_Operate)
              Flag_Irriga_Operate = 1;
            else
              Flag_Irriga_Operate = 0;           
          }
          Ui_State = UI_MAIN_BUTTON_A;
          break;

        case MS_WEATHER:
          Ui_State = WEATHER_T_H_0;
          break;

        case MS_RECIPE_1:
          Ui_State = RECIPE_1_NAME_0;
          id_offset = 0;
          
          break;

        case MS_RECIPE_2:
          Ui_State = RECIPE_2_NAME_0;
          id_offset = ID_Recipe2_0 - ID_Recipe1_0;
          break;

        case MS_SYS:
          Ui_State = SYS_CLOCK_0;
          break;

        default:
          Ui_State = UI_MAIN_BUTTON_A;
        }
      }
      break;

  //----------------------------------------------------------------------------
  case WEATHER_T_H_0:
    Ui_Config_Var = ID_Temp_Max;
    LCDScrollOn( L4, "> Temperatura maxima del ambiente" );
    Ui_State++;
    break;     
    
  case WEATHER_T_L_0:
    Ui_Config_Var = ID_Temp_Min;
    LCDScrollOn( L4, "> Temperatura minima del ambiente" );
    Ui_State++;
    break;    
    
  case WEATHER_H_H_0:
    Ui_Config_Var = ID_Hum_Max;
    LCDScrollOn( L4, "> Humedad maxima del ambiente" );
    Ui_State++;
    break;    
    
  case WEATHER_H_L_0:
    Ui_Config_Var = ID_Hum_Min;
    LCDScrollOn( L4, "> Humedad minima del ambiente" );
    Ui_State++;
    break;    
      
  case WEATHER_T_H_1:
  case WEATHER_T_L_1:
    sprintf((char*)Ui_Disp_Buffer, "%d C", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;    
      
  case WEATHER_H_H_1:
  case WEATHER_H_L_1:
    sprintf((char*)Ui_Disp_Buffer, "%d %", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;
    
  case WEATHER_END:
    Ui_State = UI_ENTER_SCREEN_STOPPED;
    break;
      
  //----------------------------------------------------------------------------
  case RECIPE_1_NAME_0:
  case RECIPE_2_NAME_0:
    LCDStr(0, L4, "> Nombre de la receta");
    Ui_Config_Var = ID_Recipe1_0+id_offset;
    Ui_Config_Ini = ID_Recipe1_0+id_offset;
    Ui_Config_Max = NAME_SIZE;
    Ui_State++;
    break;
    
  case RECIPE_1_GROW_0:
  case RECIPE_2_GROW_0:
    Ui_Config_Var = ID_Grow_Time+id_offset;
    LCDScrollOn( L4, "> Periodo de cultivo" );
    Ui_State++;
    break; 
    
  case RECIPE_1_SUNRISE_0:
  case RECIPE_2_SUNRISE_0:
    Ui_Config_Var = ID_Sunrise+id_offset;
    LCDScrollOn( L4, "> Hora en la que comienza el dia" );
    Ui_State++;
    break; 
    
  case RECIPE_1_SUNSET_0:
  case RECIPE_2_SUNSET_0:
    Ui_Config_Var = ID_Sunset+id_offset;
    LCDScrollOn( L4, "> Hora en la que termina el dia" );
    Ui_State++;
    break;
    
  case RECIPE_1_FLOW_D_0:
  case RECIPE_2_FLOW_D_0:
    Ui_Config_Var = ID_Time_Flow_Day+id_offset;
    LCDScrollOn( L4, "> Ciclo de riego de dia: Bomba encendida" );
    Ui_State++;
    break; 
    
  case RECIPE_1_NOFLOW_D_0:
  case RECIPE_2_NOFLOW_D_0:
    Ui_Config_Var = ID_Time_No_Flow_Day+id_offset;
    LCDScrollOn( L4, "> Ciclo de riego de dia: Bomba apagada" );
    Ui_State++;
    break; 
    
  case RECIPE_1_FLOW_N_0:
  case RECIPE_2_FLOW_N_0:
    Ui_Config_Var = ID_Time_Flow_Night+id_offset;
    LCDScrollOn( L4, "> Ciclo de riego de noche: Bomba encendida" );
    Ui_State++;
    break; 
    
  case RECIPE_1_NOFLOW_N_0:
  case RECIPE_2_NOFLOW_N_0:
    Ui_Config_Var = ID_Time_No_Flow_Night+id_offset;
    LCDScrollOn( L4, "> Ciclo de riego de noche: Bomba apagada" );
    Ui_State++;
    break; 
    
  case RECIPE_1_EC_H_0:
  case RECIPE_2_EC_H_0:
    Ui_Config_Var = ID_Ec_Max+id_offset;
    LCDScrollOn( L4, "> ElectroConductividad  maxima de la solucion" );
    Ui_State++;
    break;     
    
  case RECIPE_1_EC_L_0:
  case RECIPE_2_EC_L_0:
    Ui_Config_Var = ID_Ec_Min+id_offset;
    LCDScrollOn( L4, "> ElectroConductividad  minima de la solucion" );
    Ui_State++;
    break;    
    
  case RECIPE_1_PH_H_0:
  case RECIPE_2_PH_H_0:
    Ui_Config_Var = ID_Ph_Max+id_offset;
    LCDScrollOn( L4, "> Ph maximo de la solucion" );
    Ui_State++;
    break;    
    
  case RECIPE_1_PH_L_0:
  case RECIPE_2_PH_L_0:
    Ui_Config_Var = ID_Ph_Min+id_offset;
    LCDScrollOn( L4, "> Ph minimo de la solucion" );
    Ui_State++;
    break;    

  case RECIPE_1_REG_FREQ_0:
  case RECIPE_2_REG_FREQ_0:
    Ui_Config_Var = ID_Regulation_Freq+id_offset;
    LCDScrollOn( L4, "> Frecuencia de regulacion de la solucion" );
    Ui_State++;
    break;    
    
  case RECIPE_1_GROW_1:
  case RECIPE_2_GROW_1:
    sprintf((char*)Ui_Disp_Buffer, "%d dias", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;        
    
  case RECIPE_1_SUNRISE_1:
  case RECIPE_1_SUNSET_1:
  case RECIPE_2_SUNRISE_1:
  case RECIPE_2_SUNSET_1:
    sprintf((char*)Ui_Disp_Buffer, "%d:00", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;        
    
  case RECIPE_1_FLOW_D_1:
  case RECIPE_1_NOFLOW_D_1:
  case RECIPE_1_FLOW_N_1:
  case RECIPE_1_NOFLOW_N_1:
  case RECIPE_2_FLOW_D_1:
  case RECIPE_2_NOFLOW_D_1:
  case RECIPE_2_FLOW_N_1:
  case RECIPE_2_NOFLOW_N_1:
    sprintf((char*)Ui_Disp_Buffer, "%d s", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;    
    
  case RECIPE_1_EC_H_1:
  case RECIPE_1_EC_L_1:
  case RECIPE_2_EC_H_1:
  case RECIPE_2_EC_L_1:
    sprintf((char*)Ui_Disp_Buffer, "%d ppm", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;    
      
  case RECIPE_1_PH_H_1:
  case RECIPE_1_PH_L_1:
  case RECIPE_2_PH_H_1:
  case RECIPE_2_PH_L_1:
    sprintf((char*)Ui_Disp_Buffer, "%d.%d  ", (uint32_t)read_flash(Ui_Config_Var)/10, read_flash(Ui_Config_Var)%10);
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;      

  case RECIPE_1_REG_FREQ_1:
  case RECIPE_2_REG_FREQ_1:
    sprintf((char*)Ui_Disp_Buffer, "Cada %d riegos", read_flash(Ui_Config_Var));
    LCDStr(5, L5, Ui_Disp_Buffer);
    Ui_State++;
    break;
    
  case RECIPE_1_NAME_1:
  case RECIPE_2_NAME_1:
    for (Ui_Index = 0; Ui_Index < Ui_Config_Max; Ui_Index++)
    {
      Ui_Disp_Buffer[Ui_Index] = (uint8_t)read_flash(Ui_Config_Ini + Ui_Index);
    }
    Ui_Disp_Buffer[Ui_Config_Max] = '\0';
    LCDStr(0, L5, Ui_Disp_Buffer);
    LCDBlinkOn(Ui_Config_Var - Ui_Config_Ini, L5, &Ui_Disp_Buffer[Ui_Config_Var - Ui_Config_Ini], 1);
    Ui_State++;
    break;
    
  case RECIPE_1_END:
  case RECIPE_2_END:
    Ui_State = UI_ENTER_SCREEN_STOPPED;
    break;
    
  //----------------------------------------------------------------------------
  case SYS_CLOCK_0:
    memcpy(Ui_Clock_Buffer, Global_Date, 6);
    LCDScrollOn( L4, "> Fecha y Hora" );
    Ui_Index = DAY;
    Ui_State++;
    break;

  case SYS_CLOCK_1:
    validate_clock(Ui_Clock_Buffer, Ui_Index);
    clock_to_ascii(Ui_Disp_Buffer, Ui_Clock_Buffer);
    LCDStr(0, L5, Ui_Disp_Buffer);
    LCDBlinkOn(Ui_Index*3, L5, &Ui_Disp_Buffer[Ui_Index*3], 2);
    Ui_State++;
    break;
    
  case SYS_CLOCK_BUTTON_B:
    /* Supervisor or master user? */
    if((User_Logged_In != USR_OP1) && (User_Logged_In != USR_OP2))
    {
      if(!Flag_Button_I && Ui_Clock_Buffer[Ui_Index] < 0xFF) // Ver si dio la vuelta e inc.
      {
        Ui_Clock_Buffer[Ui_Index]++;
        Flag_Main_Ui_Changed = 1;
        Ui_State -= 2;
        break;
      }

      if(!Flag_Button_D && Ui_Clock_Buffer[Ui_Index] > 0x00) // Ver si dio la vuelta y dec.
      {
        Ui_Clock_Buffer[Ui_Index]--;
        Flag_Main_Ui_Changed = 1;
        Ui_State -= 2;
        break;
      }
    }

    if(!Flag_Button_M)
    {
      if(User_Logged_In == USR_OP1 || User_Logged_In == USR_OP2)
      {
        LCDClearLines(L4, L7);
        Ui_State++;
        break;
      }

      if(Ui_Index < MIN)
      {
        Ui_Index++;
        Ui_State -= 2;
      }
      else
      {
        if(Flag_Main_Ui_Changed)
        {
          Flag_Main_Ui_Changed = 0;
          Ui_Clock_Buffer[SEC] = 0;										// Save clock
          set_datetime(Ui_Clock_Buffer);
        }
        LCDClearLines(L4, L7);
        Ui_State++;
      }
      break;
    }

    if(!Flag_Button_R && Ui_Index > DAY)
    {
      Ui_Index--;
      Ui_State -= 2;
      break;
    }
    break;
            
  case SYS_CONTRAST_0:
    Ui_Config_Var = ID_Contrast;
    LCDScrollOn( L4, "> Contraste" );
    Ui_State++;
    break;

  case SYS_CONTRAST_1:
    // Format: 0000EEEEEEEEEEEE
    binary_to_ascii(read_flash(Ui_Config_Var), Ui_Disp_Buffer, 0, 4); // Careful, might write 11 bytes!
    Ui_Disp_Buffer[4] = 0x00;
    LCDStr(10, L5, Ui_Disp_Buffer);
    LCDContrast();
    Ui_State++;
    break;
     
  case SYS_PASS_OP1_0:
    if(User_Logged_In != USR_OP2)
    {
      LCDStr(0, L4, "> Clave Operador 1");
      Ui_Config_Var = ID_Pass_Op1_0;
      Ui_Config_Ini = ID_Pass_Op1_0;
      Ui_Config_Max = PASS_SIZE;
      Ui_State++;
    }
    else
      Ui_State = SYS_PASS_OP2_0;
    break;

  case SYS_PASS_OP2_0:
    if(User_Logged_In != USR_OP1)
    {
      LCDStr(0, L4, "> Clave Operador 2");
      Ui_Config_Var = ID_Pass_Op2_0;
      Ui_Config_Ini = ID_Pass_Op2_0;
      Ui_Config_Max = PASS_SIZE;
      Ui_State++;
    }
    else
      Ui_State = SYS_PASS_SUP1_0;
    break;

  case SYS_PASS_SUP1_0:
    if(User_Logged_In == USR_SUP1 || User_Logged_In == USR_MASTER)
    {
      LCDStr(0, L4, "> Clave Supervisor 1");
      Ui_Config_Var = ID_Pass_Sup1_0;
      Ui_Config_Ini = ID_Pass_Sup1_0;
      Ui_Config_Max = PASS_SIZE;
      Ui_State++;
    }
    else
      Ui_State = SYS_PASS_SUP2_0;
    break;

 case SYS_PASS_SUP2_0:
    if(User_Logged_In == USR_SUP2 || User_Logged_In == USR_MASTER)
    {
      LCDStr(0, L4, "> Clave Supervisor 2");
      Ui_Config_Var = ID_Pass_Sup2_0;
      Ui_Config_Ini = ID_Pass_Sup2_0;
      Ui_Config_Max = PASS_SIZE;
      Ui_State++;
    }
    else
      Ui_State = SYS_ERASE_SD_0;
    break;
  
  case SYS_PASS_OP1_1:
  case SYS_PASS_OP2_1:
  case SYS_PASS_SUP1_1:
  case SYS_PASS_SUP2_1:
    for (Ui_Index = 0; Ui_Index < Ui_Config_Max; Ui_Index++)
      Ui_Disp_Buffer[Ui_Index] = (uint8_t)read_flash(Ui_Config_Ini + Ui_Index);
    Ui_Disp_Buffer[Ui_Config_Max] = '\0';
    LCDStr(0, L5, Ui_Disp_Buffer);
    LCDBlinkOn(Ui_Config_Var - Ui_Config_Ini, L5, &Ui_Disp_Buffer[Ui_Config_Var - Ui_Config_Ini], 1);
    Ui_State++;
    break;
    
  case SYS_END:
    Ui_State = UI_ENTER_SCREEN_STOPPED;
    break;
    
  //----------------------------------------------------------------------------
  case UI_MAIN_BUTTON_A:
    
  case UI_USER_BUTTON_A:  
  case UI_LOGIN_BUTTON_A:
    
  case UI_ENTER_BUTTON_STOPPED_A:
  
  case WEATHER_T_H_BUTTON_A:
  case WEATHER_T_L_BUTTON_A:
  case WEATHER_H_H_BUTTON_A:
  case WEATHER_H_L_BUTTON_A:
  
  case RECIPE_1_NAME_BUTTON_A:
  case RECIPE_1_GROW_BUTTON_A:
  case RECIPE_1_SUNRISE_BUTTON_A:
  case RECIPE_1_SUNSET_BUTTON_A:
  case RECIPE_1_FLOW_D_BUTTON_A:
  case RECIPE_1_NOFLOW_D_BUTTON_A:
  case RECIPE_1_FLOW_N_BUTTON_A:
  case RECIPE_1_NOFLOW_N_BUTTON_A:
  case RECIPE_1_EC_H_BUTTON_A:
  case RECIPE_1_EC_L_BUTTON_A:
  case RECIPE_1_PH_H_BUTTON_A:
  case RECIPE_1_PH_L_BUTTON_A:
  case RECIPE_1_REG_FREQ_BUTTON_A:
  case RECIPE_2_NAME_BUTTON_A:
  case RECIPE_2_SUNRISE_BUTTON_A:
  case RECIPE_2_SUNSET_BUTTON_A:
  case RECIPE_2_FLOW_D_BUTTON_A:
  case RECIPE_2_NOFLOW_D_BUTTON_A:
  case RECIPE_2_FLOW_N_BUTTON_A:
  case RECIPE_2_NOFLOW_N_BUTTON_A:
  case RECIPE_2_GROW_BUTTON_A:
  case RECIPE_2_EC_H_BUTTON_A:
  case RECIPE_2_EC_L_BUTTON_A:
  case RECIPE_2_PH_H_BUTTON_A:
  case RECIPE_2_PH_L_BUTTON_A:
  case RECIPE_2_REG_FREQ_BUTTON_A:
    
  case SYS_CLOCK_BUTTON_A:
  case SYS_CONTRAST_BUTTON_A:
  case SYS_PASS_OP1_BUTTON_A:
  case SYS_PASS_OP2_BUTTON_A:
  case SYS_PASS_SUP1_BUTTON_A:
  case SYS_PASS_SUP2_BUTTON_A:
  case SYS_ERASE_SD_BUTTON_A:
                  
    if((Flags_Buttons.Byte & BUTTON_MASK) == BUTTON_MASK)
    {
      Ui_State++;	// Buttons released?
    }
    else if(inc_buttons_fast())
    {
      Ui_State++;	// Fast mode?
    }
    break;
  
  //----------------------------------------------------------------------------
  case WEATHER_T_H_BUTTON_B:
  case WEATHER_T_L_BUTTON_B:
  case WEATHER_H_H_BUTTON_B:
  case WEATHER_H_L_BUTTON_B:
   
  case RECIPE_1_GROW_BUTTON_B:
  case RECIPE_1_SUNRISE_BUTTON_B:
  case RECIPE_1_SUNSET_BUTTON_B:
  case RECIPE_1_FLOW_D_BUTTON_B:
  case RECIPE_1_NOFLOW_D_BUTTON_B:
  case RECIPE_1_FLOW_N_BUTTON_B:
  case RECIPE_1_NOFLOW_N_BUTTON_B:
  case RECIPE_1_EC_H_BUTTON_B:
  case RECIPE_1_EC_L_BUTTON_B:
  case RECIPE_1_PH_H_BUTTON_B:
  case RECIPE_1_PH_L_BUTTON_B:
  case RECIPE_1_REG_FREQ_BUTTON_B:
  case RECIPE_2_GROW_BUTTON_B:
  case RECIPE_2_EC_H_BUTTON_B:
  case RECIPE_2_EC_L_BUTTON_B:
  case RECIPE_2_PH_H_BUTTON_B:
  case RECIPE_2_PH_L_BUTTON_B:
  case RECIPE_2_SUNRISE_BUTTON_B:
  case RECIPE_2_SUNSET_BUTTON_B:
  case RECIPE_2_FLOW_D_BUTTON_B:
  case RECIPE_2_NOFLOW_D_BUTTON_B:
  case RECIPE_2_FLOW_N_BUTTON_B:
  case RECIPE_2_NOFLOW_N_BUTTON_B:
  case RECIPE_2_REG_FREQ_BUTTON_B:
    
  case SYS_CONTRAST_BUTTON_B:
    
    if((User_Logged_In != USR_OP1) && (User_Logged_In != USR_OP2))
    {
      if(!Flag_Button_I)
      {
        inc_flash(Ui_Config_Var, 1);
        if(Timer_Buttons == 0) inc_flash(Ui_Config_Var, 4);       // Fast mode?
        Ui_State -= 2;
        Flag_Main_Ui_Changed = 1;
        break;
      }
      if(!Flag_Button_D)
      {
        dec_flash(Ui_Config_Var, 1);
        if(Timer_Buttons == 0) dec_flash(Ui_Config_Var, 4);       // Fast mode?
        Ui_State -= 2;
        Flag_Main_Ui_Changed = 1;
        break;
      }
    }
    if(!Flag_Button_M)
    {
      if(Flag_Main_Ui_Changed)
      {
        Flag_Main_Ui_Changed = 0;
        update_flash_safe(User_Logged_In);
      }
      LCDClearLines(L4, L7);
      Ui_State++;
      break;
    }
    if(!Flag_Button_R )
    {
      if(Flag_Main_Ui_Changed)
      {
        Flag_Main_Ui_Changed = 0;
        update_flash_safe(User_Logged_In);
      }
      LCDClearLines(L4, L7);
      
      if(Ui_State == WEATHER_T_H_BUTTON_B)
        Ui_State = WEATHER_END;   
      else
        Ui_State -= 7;
    }
    break;
    
    //-------------------------------------------------------------------
  case RECIPE_1_NAME_BUTTON_B:
  case RECIPE_2_NAME_BUTTON_B:
    
  case SYS_PASS_OP1_BUTTON_B:
  case SYS_PASS_OP2_BUTTON_B:
  case SYS_PASS_SUP1_BUTTON_B:
  case SYS_PASS_SUP2_BUTTON_B:

    if(!Flag_Button_M)
    {
      Ui_Config_Var++;
      if((Ui_Config_Var - Ui_Config_Ini) >= Ui_Config_Max) //Si voy más delante menu siguiene
      {
        if(Flag_Main_Ui_Changed)
        {
          Flag_Main_Ui_Changed = 0;
          update_flash_safe(User_Logged_In);
        }
        LCDClearLines(L4, L7);
        Ui_State++;
      }
      else Ui_State -= 2;
    }

    if(!Flag_Button_I)
    {
      inc_flash(Ui_Config_Var, 1);      
      if(Timer_Buttons == 0) inc_flash(Ui_Config_Var, 4); // Fast mode?
      write_flash(Ui_Config_Var, filter_chars_inc(read_flash(Ui_Config_Var)));
      Ui_State -= 2;
      Flag_Main_Ui_Changed = 1;
      break;
    }

    if(!Flag_Button_D)
    {
      dec_flash(Ui_Config_Var, 1);      
      if(Timer_Buttons == 0) dec_flash(Ui_Config_Var, 4); // Fast mode?
      write_flash(Ui_Config_Var, filter_chars_dec(read_flash(Ui_Config_Var)));
      Ui_State -= 2;
      Flag_Main_Ui_Changed = 1;
      break;
    }

    if(!Flag_Button_R)
    {
      Ui_Config_Var--;
      if(Ui_Config_Var < Ui_Config_Ini) // Si vuelvo más para atras menu anterior
      {        
        if(Flag_Main_Ui_Changed)
        {
          Flag_Main_Ui_Changed = 0;
          update_flash_safe(User_Logged_In);
        }
        LCDClearLines(L4, L7);
        
        if(Ui_State == RECIPE_1_NAME_BUTTON_B)
          Ui_State = RECIPE_1_END;   
        else if(Ui_State == RECIPE_2_NAME_BUTTON_B)
          Ui_State = RECIPE_2_END;  
        else
          Ui_State -= 7;
      }
      else Ui_State -=2;
    }
    break;

  //-------------------------------------------------------------------
  default:
    Ui_State = UI_MAIN_BUTTON_A;  
  }
  LCDUpdate();  
}

/* Private Functions ---------------------------------------------------------*/
// Remove special chars (incremental)
uint8_t filter_chars_inc(uint16_t data)
{
  if(data < '0')
    data = '0';
  else if((data > '9') && (data < 'A'))
    data = 'A';
  else if((data > 'Z') && (data < 'a'))
    data = 'a';
  else if(data > 'z')
    data = 'z';

  return data;
}

// Remove special chars (decremental)
uint8_t filter_chars_dec(uint16_t data)
{
  if(data > 'z')
    data = 'z';
  else if((data > 'Z') && (data < 'a'))
    data = 'Z';
  else if((data > '9') && (data < 'A'))
    data = '9';
  else if(data < '0')
    data = ' ';  

  return data;
}