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

void FLASH_SectorChip(void);
void FLASH_SectorErase(uint32_t Address);
void FLASH_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
void FLASH_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);

#endif
