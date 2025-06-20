// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 9.2.2
// Project name: SmartBand

#include "../ui.h"

void ui_Timer_screen_init(void)
{
    ui_Timer = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Timer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Timer, lv_color_hex(0x393C41), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Timer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Timer_ArcTimer = lv_arc_create(ui_Timer);
    lv_obj_set_width(ui_Timer_ArcTimer, 150);
    lv_obj_set_height(ui_Timer_ArcTimer, 150);
    lv_obj_set_x(ui_Timer_ArcTimer, 0);
    lv_obj_set_y(ui_Timer_ArcTimer, -30);
    lv_obj_set_align(ui_Timer_ArcTimer, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_Timer_ArcTimer, 0);
    lv_arc_set_bg_angles(ui_Timer_ArcTimer, 0, 360);
    lv_arc_set_rotation(ui_Timer_ArcTimer, -90);

    lv_obj_set_style_arc_color(ui_Timer_ArcTimer, lv_color_hex(0x00AEB4), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_Timer_ArcTimer, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Timer_ArcTimer, lv_color_hex(0x83858B), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Timer_ArcTimer, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_Timer_LabelTime = lv_label_create(ui_Timer);
    lv_obj_set_width(ui_Timer_LabelTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Timer_LabelTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Timer_LabelTime, 0);
    lv_obj_set_y(ui_Timer_LabelTime, -30);
    lv_obj_set_align(ui_Timer_LabelTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Timer_LabelTime, "00:00:00");
    lv_obj_set_style_text_color(ui_Timer_LabelTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Timer_LabelTime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Timer_LabelTime, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Timer_ButtonStart = lv_button_create(ui_Timer);
    lv_obj_set_width(ui_Timer_ButtonStart, 50);
    lv_obj_set_height(ui_Timer_ButtonStart, 50);
    lv_obj_set_x(ui_Timer_ButtonStart, -80);
    lv_obj_set_y(ui_Timer_ButtonStart, 90);
    lv_obj_set_align(ui_Timer_ButtonStart, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Timer_ButtonStart, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Timer_ButtonStart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Timer_ButtonStart, lv_color_hex(0x83858B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Timer_ButtonStart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Timer_ImageStart = lv_image_create(ui_Timer_ButtonStart);
    lv_image_set_src(ui_Timer_ImageStart, &ui_img_start_png);
    lv_obj_set_width(ui_Timer_ImageStart, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Timer_ImageStart, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Timer_ImageStart, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Timer_ImageStart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Timer_ButtonRestart = lv_button_create(ui_Timer);
    lv_obj_set_width(ui_Timer_ButtonRestart, 50);
    lv_obj_set_height(ui_Timer_ButtonRestart, 50);
    lv_obj_set_x(ui_Timer_ButtonRestart, 80);
    lv_obj_set_y(ui_Timer_ButtonRestart, 90);
    lv_obj_set_align(ui_Timer_ButtonRestart, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Timer_ButtonRestart, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_Timer_ButtonRestart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Timer_ButtonRestart, lv_color_hex(0x83858B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Timer_ButtonRestart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Timer_ImageRestart = lv_image_create(ui_Timer_ButtonRestart);
    lv_image_set_src(ui_Timer_ImageRestart, &ui_img_restart_png);
    lv_obj_set_width(ui_Timer_ImageRestart, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Timer_ImageRestart, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Timer_ImageRestart, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Timer_ImageRestart, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Timer_ImageRestart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_Timer, ui_event_Timer, LV_EVENT_ALL, NULL);

}
