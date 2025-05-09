/**
 * @file   w25q16.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  w25q16 define
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __W25Q16_H__
#define __W25Q16_H__

#include "components/prf.h"

void FLASH_sync(void);
void FLASH_erase(uint32_t address);
void FLASH_read(uint32_t address, uint8_t *p_data, uint32_t size);
void FLASH_prog(uint32_t address, uint8_t *p_data, uint16_t size);

#endif
