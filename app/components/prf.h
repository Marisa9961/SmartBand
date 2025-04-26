/**
 * @file   bsp.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Used For BSP
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */
#ifndef __PRF_H__
#define __PRF_H__

#include "main.h"

/**
* @brief  延时
*/
#ifndef USE_RTOS
#include <cmsis_os2.h>
#define delay(x) osDelay(x)
#else
#define delay(x) HAL_Delay(x)
#endif

#endif
