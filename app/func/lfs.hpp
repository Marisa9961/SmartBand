/**
 * @file   lfs.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  lfs wrapper define
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __FUNC_LFS_HPP__
#define __FUNC_LFS_HPP__

#include <littlefs/lfs.h>

// TODO impl more Interface

class Lfs {
public:
  lfs_t littlefs;
};

#endif
