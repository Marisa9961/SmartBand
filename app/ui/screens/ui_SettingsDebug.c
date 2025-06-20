// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 9.2.2
// Project name: SmartBand

#include "../ui.h"

void ui_SettingsDebug_screen_init(void)
{
    ui_SettingsDebug = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_SettingsDebug, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SettingsDebug, lv_color_hex(0x393C41), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsDebug, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsDebug_Keyboard = lv_keyboard_create(ui_SettingsDebug);
    lv_obj_set_width(ui_SettingsDebug_Keyboard, 240);
    lv_obj_set_height(ui_SettingsDebug_Keyboard, 120);
    lv_obj_set_x(ui_SettingsDebug_Keyboard, 0);
    lv_obj_set_y(ui_SettingsDebug_Keyboard, 80);
    lv_obj_set_align(ui_SettingsDebug_Keyboard, LV_ALIGN_CENTER);

    ui_SettingsDebug_TextAreaRX = lv_textarea_create(ui_SettingsDebug);
    lv_obj_set_width(ui_SettingsDebug_TextAreaRX, 150);
    lv_obj_set_height(ui_SettingsDebug_TextAreaRX, 50);
    lv_obj_set_x(ui_SettingsDebug_TextAreaRX, 20);
    lv_obj_set_y(ui_SettingsDebug_TextAreaRX, -80);
    lv_obj_set_align(ui_SettingsDebug_TextAreaRX, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_SettingsDebug_TextAreaRX, "Placeholder...");

    ui_SettingsDebug_TextAreaTX = lv_textarea_create(ui_SettingsDebug);
    lv_obj_set_width(ui_SettingsDebug_TextAreaTX, 150);
    lv_obj_set_height(ui_SettingsDebug_TextAreaTX, 50);
    lv_obj_set_x(ui_SettingsDebug_TextAreaTX, 20);
    lv_obj_set_y(ui_SettingsDebug_TextAreaTX, -20);
    lv_obj_set_align(ui_SettingsDebug_TextAreaTX, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_SettingsDebug_TextAreaTX, "Placeholder...");

    ui_SettingsDebug_LabelTX = lv_label_create(ui_SettingsDebug);
    lv_obj_set_width(ui_SettingsDebug_LabelTX, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingsDebug_LabelTX, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SettingsDebug_LabelTX, -80);
    lv_obj_set_y(ui_SettingsDebug_LabelTX, -20);
    lv_obj_set_align(ui_SettingsDebug_LabelTX, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsDebug_LabelTX, "TX");
    lv_obj_set_style_text_color(ui_SettingsDebug_LabelTX, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsDebug_LabelTX, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SettingsDebug_LabelTX, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsDebug_LabelRX = lv_label_create(ui_SettingsDebug);
    lv_obj_set_width(ui_SettingsDebug_LabelRX, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingsDebug_LabelRX, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SettingsDebug_LabelRX, -80);
    lv_obj_set_y(ui_SettingsDebug_LabelRX, -80);
    lv_obj_set_align(ui_SettingsDebug_LabelRX, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsDebug_LabelRX, "RX");
    lv_obj_set_style_text_color(ui_SettingsDebug_LabelRX, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsDebug_LabelRX, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SettingsDebug_LabelRX, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_keyboard_set_textarea(ui_SettingsDebug_Keyboard, ui_SettingsDebug_TextAreaTX);
    lv_obj_add_event_cb(ui_SettingsDebug, ui_event_SettingsDebug, LV_EVENT_ALL, NULL);

}
