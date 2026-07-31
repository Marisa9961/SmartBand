/**
 * @file   core.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  core define
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __TASKS_CORE_HPP__
#define __TASKS_CORE_HPP__

namespace bd::task {
bool prepareRuntime();
[[noreturn]] void CoreTask(void *argument);
} // namespace bd::task

#endif
