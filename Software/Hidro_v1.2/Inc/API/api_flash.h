#ifndef API_FLASH_H
#define API_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"
//#include "memory_map.h"

/* Function ------------------------------------------------------------------*/
bool flash_write(uint32_t , uint32_t *, uint32_t );
bool flash_erase(uint32_t , uint32_t );
bool flash_verify(uint32_t , uint32_t *, uint32_t );
uint32_t flash_read(uint32_t);

/* Defines -------------------------------------------------------------------*/
/**
 * Flash in L4xx is organized in 2kByte sectors:
 * FLASH_PAGE_SIZE, FLASH_BANK_SIZE and FLASH_SIZE are defined at
 * stm32l4xx_hal_flash.h
 * Note 1: FLASH_SIZE is read from the MCU at execution time.
 * Note 2: some STML4 devices use double bank flash.
 */
#define FLASH_PAGE_NBPERBANK  (FLASH_BANK_SIZE / FLASH_PAGE_SIZE)

/**
 * Project specific
 * Two pages at the end of the flash (minimum erase size = one 2k page)
 */
#define FLASH_USER_START_ADDRESS  (FLASH_BASE + FLASH_SIZE - (2*FLASH_PAGE_SIZE))
#define FLASH_USER_END_ADDRESS    (FLASH_BASE + FLASH_SIZE - 1)

#define FLASH_PAGE0     FLASH_USER_START_ADDRESS
#define FLASH_PAGE0_ADDRESS 0x0807F000U //Same as FLASH_PAGE0 but as an integer to avoid an error
#define FLASH_PAGE0_END (FLASH_PAGE0 + FLASH_PAGE_SIZE - 1)  
#define FLASH_PAGE1     (FLASH_USER_START_ADDRESS + FLASH_PAGE_SIZE)
#define FLASH_PAGE1_ADDRESS 0x0807F800U //Same as FLASH_PAGE1 but as an integer to avoid an error
#define FLASH_PAGE1_END (FLASH_PAGE1 + FLASH_PAGE_SIZE - 1)

#endif  /* API_FLASH_H  */
