/**
 * @file   KT6368A.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port define
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __KT6368A_H__
#define __KT6368A_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

void BLE_init();
void BLE_deinit();

bool BLE_flag();
char BLE_readNextChar();

void BLE_transmit(const char *str);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
