// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 9.2.2
// Project name: SmartBand

#include "../ui.h"

void ui_SettingsBluetooth_screen_init(void)
{
    ui_SettingsBluetooth = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_SettingsBluetooth, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SettingsBluetooth, lv_color_hex(0x393C41), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingsBluetooth, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsBluetooth_LabelBluetooth = lv_label_create(ui_SettingsBluetooth);
    lv_obj_set_width(ui_SettingsBluetooth_LabelBluetooth, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingsBluetooth_LabelBluetooth, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SettingsBluetooth_LabelBluetooth, 0);
    lv_obj_set_y(ui_SettingsBluetooth_LabelBluetooth, -40);
    lv_obj_set_align(ui_SettingsBluetooth_LabelBluetooth, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsBluetooth_LabelBluetooth, "Bluetooth");
    lv_obj_set_style_text_color(ui_SettingsBluetooth_LabelBluetooth, lv_color_hex(0xFFFFFF),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsBluetooth_LabelBluetooth, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SettingsBluetooth_LabelBluetooth, &lv_font_montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsBluetooth_ImageBluetooth = lv_image_create(ui_SettingsBluetooth);
    lv_image_set_src(ui_SettingsBluetooth_ImageBluetooth, &ui_img_bluetooth_png);
    lv_obj_set_width(ui_SettingsBluetooth_ImageBluetooth, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingsBluetooth_ImageBluetooth, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SettingsBluetooth_ImageBluetooth, 0);
    lv_obj_set_y(ui_SettingsBluetooth_ImageBluetooth, -80);
    lv_obj_set_align(ui_SettingsBluetooth_ImageBluetooth, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SettingsBluetooth_ImageBluetooth, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_SettingsBluetooth_ImageBluetooth, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_SettingsBluetooth_SwitchBluetooth = lv_switch_create(ui_SettingsBluetooth);
    lv_obj_set_width(ui_SettingsBluetooth_SwitchBluetooth, 50);
    lv_obj_set_height(ui_SettingsBluetooth_SwitchBluetooth, 30);
    lv_obj_set_align(ui_SettingsBluetooth_SwitchBluetooth, LV_ALIGN_CENTER);

    ui_SettingsBluetooth_LabelBluetoothHint = lv_label_create(ui_SettingsBluetooth);
    lv_obj_set_width(ui_SettingsBluetooth_LabelBluetoothHint, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SettingsBluetooth_LabelBluetoothHint, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_SettingsBluetooth_LabelBluetoothHint, 0);
    lv_obj_set_y(ui_SettingsBluetooth_LabelBluetoothHint, 60);
    lv_obj_set_align(ui_SettingsBluetooth_LabelBluetoothHint, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingsBluetooth_LabelBluetoothHint, "Enable to connect\nwith phone and\nother devices.");
    lv_obj_set_style_text_color(ui_SettingsBluetooth_LabelBluetoothHint, lv_color_hex(0xFFFFFF),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SettingsBluetooth_LabelBluetoothHint, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_SettingsBluetooth_LabelBluetoothHint, LV_TEXT_ALIGN_CENTER,
                                LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_SettingsBluetooth_ImageBluetooth, ui_event_SettingsBluetooth_ImageBluetooth, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingsBluetooth, ui_event_SettingsBluetooth, LV_EVENT_ALL, NULL);

}
