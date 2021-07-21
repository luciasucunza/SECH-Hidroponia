#ifndef UTILS_H__
#define UTILS_H__

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Function ------------------------------------------------------------------*/
void bin_16_a_bcd_5 (uint16_t, uint8_t *);
uint8_t bin_dec_pos_to_disp_int (uint8_t);
void binary_to_ascii (int16_t, uint8_t *, uint8_t, uint8_t);
uint16_t bin_to_bcd (uint8_t);
uint8_t bcd_to_bin (uint8_t);
uint8_t two_ascii_to_bin (uint8_t *);
uint8_t nibble_to_ascii (uint8_t data);
uint8_t ascii_to_nibble (uint8_t data);
void digit_to_ascii (uint8_t c, uint8_t * buffer);
void exit_sleep (void);

#endif  /* UTILS_H__  */