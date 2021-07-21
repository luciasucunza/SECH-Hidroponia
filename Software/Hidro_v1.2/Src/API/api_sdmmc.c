/* Includes ------------------------------------------------------------------*/
#include "api_sdmmc.h"

/* Private Functions ---------------------------------------------------------*/

/* Exported Data -------------------------------------------------------------*/
extern SD_HandleTypeDef hsd1;

/* Functions -----------------------------------------------------------------*/
bool sdmmc_write(uint8_t *data, uint32_t block_addr)
{
//  __HAL_SD_ENABLE(&hsd1);
  
  if(HAL_SD_WriteBlocks(&hsd1, data, block_addr, 1, SD_WR_BLK_TIMEOUT) != HAL_OK)
  {
//    __HAL_SD_DISABLE(&hsd1);
    return false;
  }
//  __HAL_SD_DISABLE(&hsd1);
  return true;
}

bool sdmmc_read(uint8_t *data, uint32_t block_addr)
{
//  __HAL_SD_ENABLE(&hsd1);
  
  if(HAL_SD_ReadBlocks(&hsd1, data, block_addr, 1, SD_RD_BLK_TIMEOUT) != HAL_OK)
  {
//    __HAL_SD_DISABLE(&hsd1);
    return false;
  }
  // In the read the waiting is short, then we wait for the end in the same function
  uint32_t count = 0;
  while(count < 50)
  {
    if(!sdmmc_is_busy())
      return true;
    count++;
  }
//  __HAL_SD_DISABLE(&hsd1);
  return false;
}

bool sdmmc_erase(uint32_t start, uint32_t end)
{
//  __HAL_SD_ENABLE(&hsd1);

  if(HAL_SD_Erase(&hsd1, start, end) != HAL_OK)
  {
//    __HAL_SD_DISABLE(&hsd1);
    return false;
  }
//  __HAL_SD_DISABLE(&hsd1);
  return true;
}

/**
  * @brief Get the SD's size in blocks.
  * @return size
  */
uint32_t sdmmc_get_size(void)
{
  HAL_SD_CardCSDTypedef csd;
  
  if(HAL_SD_GetCardCSD(&hsd1, &csd) != HAL_OK)
  {
    return 0;
  }
  return hsd1.SdCard.BlockNbr;
}

bool sdmmc_is_busy(void)
{
//  if(HAL_SD_GetState(&hsd1) != HAL_SD_STATE_READY)
//    return true;
  if(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
    return true;
  return false;
}