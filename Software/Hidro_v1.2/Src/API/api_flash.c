/**
  * Flash in STM32L4xx is organized in 2kByte pages (erase size)
  * FLASH_PAGE_SIZE, FLASH_BANK_SIZE and FLASH_SIZE are defined at
  * stm32l4xx_hal_flash.h
  * Note 1: FLASH_SIZE is read from the MCU at execution time.
  * Note 2: some STML4 devices use double bank flash.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "api_flash.h"

/* Private Functions ---------------------------------------------------------*/
static uint32_t _get_page(uint32_t address);

/* Functions -----------------------------------------------------------------*/
bool flash_write(uint32_t address, uint32_t *data, uint32_t size_in_bytes)
{
  if( address < FLASH_USER_START_ADDRESS )
  {
    return false;
  }

  /* DataLength must be a multiple of 64 bit */
  uint32_t aux = size_in_bytes % 8;
  if( aux != 0 )
  {
    /* Pad it to the next 64bit integer, in this case an extra 32 bit word
       (the next word at the end of the source's last one) will be written
       to flash, this is word won't have any sense */ 
    size_in_bytes += (8 - aux);
  }  

  if( (address + size_in_bytes ) > FLASH_USER_END_ADDRESS )
  {
    return false;
  }

  /* Unlock the Flash to enable the flash control register access */
  HAL_FLASH_Unlock();
  /* Clear flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR
                       | FLASH_FLAG_OPTVERR);

  uint32_t i = 0;
  uint32_t size_in_dwords = size_in_bytes/8;
  for (i = 0; (i < size_in_dwords) && (address <= (FLASH_USER_END_ADDRESS-8)); i++)
  {
    /* Programing will be done by dword */
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *((uint64_t *)(data+2*i))) != HAL_OK)
    {
      HAL_FLASH_Lock();
      return false;
    }
    /* Increment FLASH destination address */
    address += 8;
  }
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();
  return true;
}

bool flash_erase(uint32_t start, uint32_t stop)
{
  FLASH_EraseInitTypeDef FLASH_EraseInit;
  uint32_t error;

  if((start > stop)
  || (start < FLASH_USER_START_ADDRESS)
  || (stop > FLASH_USER_END_ADDRESS) )
  {
    return false;
  }
  
  HAL_FLASH_Unlock();

  /* Fill the EraseInit structure: erase type */
  FLASH_EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

  /* Get the 1st sector to erase */
  uint32_t first_page = _get_page(start);
  uint32_t last_page = _get_page(stop);

  /* 1st bank */
  if( first_page <= FLASH_PAGE_NBPERBANK )
  {
    /* Fill EraseInit structure*/
    FLASH_EraseInit.Page = first_page;
    FLASH_EraseInit.Banks = FLASH_BANK_1;

    /* Get the number of sector to erase from 1st sector*/
    if( last_page > FLASH_PAGE_NBPERBANK )
    {
      FLASH_EraseInit.NbPages = FLASH_PAGE_NBPERBANK - first_page + 1;
    }
    else
    {
      FLASH_EraseInit.NbPages = last_page - first_page + 1;
    }

    if(HAL_FLASHEx_Erase(&FLASH_EraseInit, &error) != HAL_OK)
    {
      HAL_FLASH_Lock();
      return false;
    }
  }

  /* 2nd bank */
  if( last_page > FLASH_PAGE_NBPERBANK )
  {
    /* Fill EraseInit structure*/
    FLASH_EraseInit.Banks = FLASH_BANK_2;

    /* Get the number of sector to erase from 1st sector*/
    if( first_page > FLASH_PAGE_NBPERBANK )
    {
      FLASH_EraseInit.Page = first_page;
      FLASH_EraseInit.NbPages = last_page - first_page + 1;
    }
    else
    {
      FLASH_EraseInit.Page = FLASH_PAGE_NBPERBANK + 1;
      FLASH_EraseInit.NbPages = last_page - FLASH_PAGE_NBPERBANK;
    }

    if(HAL_FLASHEx_Erase(&FLASH_EraseInit, &error) != HAL_OK)
    {
      HAL_FLASH_Lock();
      return false;
    }
  }

  HAL_FLASH_Lock();
  return true;
}

uint32_t flash_read(uint32_t address)
{
  return *((uint32_t *)(address));
}

bool flash_verify(uint32_t address, uint32_t *pdata, uint32_t size_in_bytes)
{
  uint8_t *psource = (uint8_t*) address;
  uint8_t *pdest   = (uint8_t*) pdata;

  for(;(size_in_bytes > 0) && (*(psource++) == *(pdest++)) ;size_in_bytes--);

  if(size_in_bytes > 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/* Private Functions ---------------------------------------------------------*/
uint32_t _get_page(uint32_t address)
{
  uint32_t page;
  
  page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;
  
  // Adjust 
  if(page >= FLASH_PAGE_NBPERBANK)
  {
    page += (256 - FLASH_PAGE_NBPERBANK);
  }
  
  return page;
}
