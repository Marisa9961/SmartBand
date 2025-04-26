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

  osThreadFlagsSet(core.getHandle(), gui.getUid());

  {
    lv_obj_t *spinner = lv_spinner_create(lv_screen_active());
    lv_obj_set_size(spinner, 64, 64);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
    while (true) {
      if (osThreadFlagsGet() == core.getUid()) {
        break;
      }
      lv_task_handler();
      osDelayUntil(5);
    }
    lv_obj_delete(spinner);
  }

  ui_init();

  while (true) {
    lv_task_handler();
    osDelayUntil(5);
  }
}

Task gui{GuiTask, "Gui", 128 * 24, osPriorityLow};
} // namespace bd::task
