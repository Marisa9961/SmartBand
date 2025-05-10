/**
 * @file   bl24.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BL24C02 eeprom driver
 * @date   2025-04-06
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __BL24_H__
#define __BL24_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

void EEPROM_write(uint8_t addr, uint8_t *data, uint8_t length);
void EEPROM_read(uint8_t addr, uint8_t *data, uint8_t length);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
