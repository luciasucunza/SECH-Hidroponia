#ifndef SDMMC_H
#define SDMMC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "main.h"

/* Function-------------------------------------------------------------------*/   
bool sdmmc_write(uint8_t *data, uint32_t block_addr);
bool sdmmc_read(uint8_t *data, uint32_t block_addr);
bool sdmmc_erase(uint32_t start, uint32_t end);
uint32_t sdmmc_get_size(void);
bool sdmmc_is_busy(void);

/* Defines -------------------------------------------------------------------*/
#define SD_BLOCK_SIZE_POWER   9
#define SD_BLOCK_SIZE		      (1<<9)

#define SD_RD_BLK_TIMEOUT 15
#define SD_WR_BLK_TIMEOUT 20

#endif  /* API_SENSOR_DHT11_H  */