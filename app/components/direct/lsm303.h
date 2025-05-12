/**
 * @file   lsm303.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  LSM303 Director Driver
 * @date   2025-04-05
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __LSM303_H__
#define __LSM303_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_DIRECT;

void DIRECT_init();
void DIRECT_fresh();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
