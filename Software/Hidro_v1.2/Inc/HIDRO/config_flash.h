#ifndef CONFIG_FLASH_H
#define CONFIG_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "api_flash.h"
#include "common.h"

/* Function ------------------------------------------------------------------*/
uint32_t read_flash (uint32_t);
void write_flash (uint32_t, uint32_t);
void inc_flash (uint32_t, uint32_t);
void dec_flash (uint32_t, uint32_t);
void get_flash (void);
bool update_flash_safe (uint8_t);

/* Defines -------------------------------------------------------------------*/
enum{
ID_Temp_Min,            // Temperatura m�nima del ambiente
ID_Temp_Max,            // Temperatura m�xima del ambiente
ID_Hum_Min,             // Humedad m�nima del ambiente
ID_Hum_Max,             // Humedad m�xima del ambiente

// Nombre                              (RECETA 1)
ID_Recipe1_0, ID_Recipe1_1, ID_Recipe1_2,  ID_Recipe1_3,  ID_Recipe1_4,  ID_Recipe1_5,  ID_Recipe1_6,  ID_Recipe1_7,
ID_Recipe1_8, ID_Recipe1_9, ID_Recipe1_10, ID_Recipe1_11, ID_Recipe1_12, ID_Recipe1_13, ID_Recipe1_14, ID_Recipe1_15,
ID_Ec_Min,              // Conductividad m�nima de la soluci�n 
ID_Ec_Max,              // Conductividad m�xima de la soluci�n 
ID_Ph_Min,              // Ph m�nimo de la soluci�n (x10)        
ID_Ph_Max,              // Ph m�ximo de la soluci�n (x10)             
ID_Time_Flow_Day,       //s Tiempo con la bomba principal prendida para regar (d�a)
ID_Time_No_Flow_Day,    //s Tiempo entre dos regados, tiene en cuenta el tiempo que se perdi� en preparar la sust (d�a)
ID_Time_Flow_Night,     //s Tiempo con la bomba principal prendida para regar (noche)
ID_Time_No_Flow_Night,  //s Tiempo entre dos regados, tiene en cuenta el tiempo que se perdi� en preparar la sust (noche)
ID_Sunrise,             //h Hora en la que comienza el d�a
ID_Sunset,              //h Hora en la que finaliza el d�a
ID_Grow_Time,           //d Tiempo en d�as que se deber regar el cultivo

// Nombre                              (RECETA 2)

ID_Recipe2_0, ID_Recipe2_1, ID_Recipe2_2,  ID_Recipe2_3,  ID_Recipe2_4,  ID_Recipe2_5,  ID_Recipe2_6,  ID_Recipe2_7,
ID_Recipe2_8, ID_Recipe2_9, ID_Recipe2_10, ID_Recipe2_11, ID_Recipe2_12, ID_Recipe2_13, ID_Recipe2_14, ID_Recipe2_15,ID_Ec_Min_2,            // Conductividad m�nima de la soluci�n 
ID_Ec_Max_2,            // Conductividad m�xima de la soluci�n 
ID_Ph_Min_2,            // Ph m�nimo de la soluci�n  (x10)  
ID_Ph_Max_2,            // Ph m�ximo de la soluci�n  (x10)  
ID_Time_Flow_Day_2,     //s Tiempo con la bomba principal prendida para regar (d�a)
ID_Time_No_Flow_Day_2,  //s Tiempo entre dos regados, tiene en cuenta el tiempo que se perdi� en preparar la sust (d�a)
ID_Time_Flow_Night_2,   //s Tiempo con la bomba principal prendida para regar (noche)
ID_Time_No_Flow_Night_2,//s Tiempo entre dos regados, tiene en cuenta el tiempo que se perdi� en preparar la sust (noche)
ID_Sunrise_2,           //h Hora en la que comienza el d�a
ID_Sunset_2,            //h Hora en la que finaliza el d�a
ID_Grow_Time_2,         //d Tiempo en d�as que se deber regar el cultivo

ID_Ph_Y_Intercept,      // Calibraci�n lineal del Ph (Dividir por -1000)
ID_Ph_Slope,            // Calibraci�n lineal del Ph (Dividir por 1000)
ID_Ec_Y_Intercept,      // Calibraci�n lineal de la Conductividad
ID_Ec_Slope,            // Calibraci�n lineal de la Conductividad

ID_Pass_Op1_0, ID_Op1_P1, ID_Op1_P2, ID_Op1_P3, ID_Op1_P4, ID_Op1_P5, ID_Op1_P6, ID_Op1_P7,
ID_Pass_Op2_0, ID_Op2_P1, ID_Op2_P2, ID_Op2_P3, ID_Op2_P4, ID_Op2_P5, ID_Op2_P6, ID_Op2_P7,
ID_Pass_Sup1_0, ID_Sup1_P1, ID_Sup1_P2, ID_Sup1_P3, ID_Sup1_P4, ID_Sup1_P5, ID_Sup1_P6, ID_Sup1_P7,
ID_Pass_Sup2_0, ID_Sup2_P1, ID_Sup2_P2, ID_Sup2_P3, ID_Sup2_P4, ID_Sup2_P5, ID_Sup2_P6, ID_Sup2_P7,

ID_Contrast,
FLASH_INFO_SIZE             // Always last
};

#endif  /* CONFIG_FLASH_H  */











