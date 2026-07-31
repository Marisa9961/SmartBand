/**
 * @file   w25q16.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  w25q16 define
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_W25Q16_H
#define BD_COMPONENTS_W25Q16_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool FLASH_sync(void);
bool FLASH_erase(uint32_t address);
bool FLASH_read(uint32_t address, uint8_t *p_data, uint32_t size);
bool FLASH_prog(uint32_t address, const uint8_t *p_data, uint32_t size);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
