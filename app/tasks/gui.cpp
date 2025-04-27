/**
 * @file   gui.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  gui impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/gui.hpp"

#include <lvgl/lvgl.h>
#include <porting/lv_port_disp.h>
#include <porting/lv_port_indev.h>

#include "tasks/core.hpp"

#include "ui/ui.h"

namespace bd::task {
void GuiTask(void *argument) {
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  ui_init();

  while (true) {
    lv_task_handler();
    osDelayUntil(5);
  }
}

Task gui{GuiTask, "Gui", 128 * 24, osPriorityLow};
} // namespace bd::task
