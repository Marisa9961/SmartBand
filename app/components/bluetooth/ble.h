/**
 * @file   ble.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port define
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __BLE_H__
#define __BLE_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

void BLE_init();

void BLE_enable();
void BLE_disable();

bool BLE_flag();
char BLE_readNextChar();

void BLE_setCallback();
void BLE_ClearCallback();

void BLE_transmit(const char *str);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
