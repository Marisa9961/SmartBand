/**
 * @file   KT6368A.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port define
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_KT6368A_H
#define BD_COMPONENTS_KT6368A_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*BLE_RxReadyCallback)(void);

void BLE_init(BLE_RxReadyCallback callback);

void BLE_enable(void);
void BLE_disable(void);

char BLE_readNextChar(void);

void BLE_transmit(const char *str);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
