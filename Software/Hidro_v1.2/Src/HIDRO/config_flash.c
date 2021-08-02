/* Includes ------------------------------------------------------------------*/
#include "config_flash.h"

/* Private Functions ---------------------------------------------------------*/
bool update_flash (void);









/* Initial flash values ------------------------------------------------------*/
__root const uint32_t config_flash[] @ FLASH_PAGE1_ADDRESS =
{
15,
35,
60,
95,

// Nombre                              (RECETA 1)
'L', 'E', 'C', 'H', 'U', 'G', 'A', ' ',	
'1', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
400,
1700,
51,
72,
180,
600,
120,
1200,
7,
19, 	 		 
70,
5, 


// Nombre                              (RECETA 2)
'P', 'E', 'P', 'E', ' ', ' ', ' ', ' ',	
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
500,
1800,
55,
60,
180,
600,
120,
1200,
6,
19, 	 		 
70,
10,
                 

237794, 		// Dividir por -1000
0074,			// Dividir por 1000
0,      		// 
0,            		// 

'0','1','2','3',' ',' ',' ',' ',// Password Operator 1
'0','1','2','3',' ',' ',' ',' ',// Password Operator 2
'3','2','1','0',' ',' ',' ',' ',// Password Supervisor 1
'3','2','1','0',' ',' ',' ',' ',// Password Supervisor 2

16,   // Contrast
0x000ADDED // binary file needs to be multiple of 4x32bits words for DFU generation to work
};

/* Flash min. & max. values --------------------------------------------------*/
__root const uint32_t config_min[] =
{
0, 		
0, 		
0, 		
0,

// Nombre                              (RECETA 1)
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
0, 		
0, 		
0, 		
0,
0, 		
0, 	
0, 		
0, 	
3,
16, 	 		
1,
0,


// Nombre                              (RECETA 2)
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	
0, 		
0, 		
0, 		
0,
0, 		
0,
0, 		
0, 
3,
16, 	 		
1,
0,
  

0, 		
0,			  
0,      		
0,        

' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	// Password min
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	// Password min
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	// Password min
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	// Password min

0   // Contrast min
};
__root const uint32_t config_max[] =
{
50,
50,
100,
100,

// Nombre                              (RECETA 1)
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',
6000,
6000,
140,
140,
3600,
21600,
3600,
21600, 
10,
23,
365,
100,


// Nombre                              (RECETA 2)
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',
6000,
6000,
140,
140,
3600,
21600,
3600,
21600, 
10,
23, 
365,
100,
    

0xffff,			// Dividir por -1000
0xffff,			// Dividir por 1000
0xffff,      		 
0xffff,

'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',	// Password max
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',	// Password max
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',	// Password max
'z', 'z', 'z', 'z', 'z', 'z', 'z', 'z',	// Password max

0x001F   		// Contrast max
};

/* Private Data --------------------------------------------------------------*/
uint32_t flash_copy[FLASH_INFO_SIZE];
uint8_t Flash_State = 0;
uint32_t Flash_Timer;


/* Functions -----------------------------------------------------------------*/
uint32_t read_flash(uint32_t index)
{
	return flash_copy[index];
}

void write_flash(uint32_t index, uint32_t val)
{
	if(index >= FLASH_INFO_SIZE)
    return;
  
	if(val > config_max[index])
    val = config_max[index];
	else if(val < config_min[index])
    val = config_min[index];
  
	flash_copy[index] = val;
}

void inc_flash(uint32_t index, uint32_t add)
{
	if((config_max[index] - flash_copy[index]) >= add )
    flash_copy[index] += add;
	else
    flash_copy[index] = config_max[index];
}

void dec_flash(uint32_t index, uint32_t sub)
{
	if((flash_copy[index] - config_min[index]) >= sub)
    flash_copy[index] -= sub;
	else
    flash_copy[index] = config_min[index];
}

void get_flash(void)
{
	uint32_t i;
  uint32_t flash_addr = FLASH_PAGE1;
  
	for( i=0; i < FLASH_INFO_SIZE; i++ )
	{
		flash_copy[i] = flash_read(flash_addr);
    flash_addr += 4; // select next flash 32bit word

		if((flash_copy[i] > config_max[i])
		|| (flash_copy[i] < config_min[i]))
			flash_copy[i] = config_min[i];
	}
}

bool update_flash_safe(uint8_t user)
{
  bool modified = false;
  uint8_t i;
  uint32_t flash_addr = FLASH_PAGE1;
  
  // look for changes
	for( i=0; i < FLASH_INFO_SIZE; i++ )
	{
		if(flash_copy[i] != flash_read(flash_addr))
    {
      modified = true;
		}
    flash_addr += 4; // select next flash 32bit word
	}
  
  if(modified)
  {
    if(update_flash())
      return true;
    // Retry on error
    else
      return update_flash();
  }
  
  return true;
}

/* Private Functions ---------------------------------------------------------*/
bool update_flash(void)
{
    // HAL bug, uninitialization returns PGAERR on the first try  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
  
  if(flash_erase(FLASH_PAGE1, FLASH_PAGE1_END) != true)
  {
    return false;
  }
  
  if(flash_write(FLASH_PAGE1, flash_copy, (FLASH_INFO_SIZE*4)) != true)
  {
    return false;
  }
  
  if(flash_verify(FLASH_PAGE1, flash_copy, (FLASH_INFO_SIZE*4)) != true)
  {
    return false;
  }    

  return true;
}