// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 9.2.2
// Project name: SmartBand

#include "../ui.h"

void ui_Home_screen_init(void)
{
    ui_Home = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Home, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Home, lv_color_hex(0x222831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Home, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_Date = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_Date, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_Date, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_Date, 0);
    lv_obj_set_y(ui_Home_Date, -100);
    lv_obj_set_align(ui_Home_Date, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_Date, "05-01");
    lv_obj_set_style_text_color(ui_Home_Date, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_Date, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Home_Date, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_Weekday = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_Weekday, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_Weekday, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_Weekday, -80);
    lv_obj_set_y(ui_Home_Weekday, -100);
    lv_obj_set_align(ui_Home_Weekday, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_Weekday, "Thu");
    lv_obj_set_style_text_color(ui_Home_Weekday, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_Weekday, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Home_Weekday, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_TimeHour = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_TimeHour, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_TimeHour, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_TimeHour, -70);
    lv_obj_set_y(ui_Home_TimeHour, -60);
    lv_obj_set_align(ui_Home_TimeHour, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_TimeHour, "12");
    lv_obj_set_style_text_color(ui_Home_TimeHour, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_TimeHour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Home_TimeHour, &ui_font_Font, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_TimeSplit = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_TimeSplit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_TimeSplit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_TimeSplit, -40);
    lv_obj_set_y(ui_Home_TimeSplit, -65);
    lv_obj_set_align(ui_Home_TimeSplit, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_TimeSplit, ":");
    lv_obj_set_style_text_color(ui_Home_TimeSplit, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_TimeSplit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transform_scale(ui_Home_TimeSplit, 512, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_TimeMinute = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_TimeMinute, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_TimeMinute, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_TimeMinute, 0);
    lv_obj_set_y(ui_Home_TimeMinute, -60);
    lv_obj_set_align(ui_Home_TimeMinute, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_TimeMinute, "34");
    lv_obj_set_style_text_color(ui_Home_TimeMinute, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_TimeMinute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Home_TimeMinute, &ui_font_Font, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_PanelApplication = lv_obj_create(ui_Home);
    lv_obj_set_width(ui_Home_PanelApplication, 200);
    lv_obj_set_height(ui_Home_PanelApplication, 80);
    lv_obj_set_x(ui_Home_PanelApplication, 0);
    lv_obj_set_y(ui_Home_PanelApplication, 60);
    lv_obj_set_align(ui_Home_PanelApplication, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Home_PanelApplication, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Home_PanelApplication, lv_color_hex(0x393C41), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Home_PanelApplication, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_LabelApplication = lv_label_create(ui_Home_PanelApplication);
    lv_obj_set_width(ui_Home_LabelApplication, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_LabelApplication, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Home_LabelApplication, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_LabelApplication, "Today's Step: 103");
    lv_obj_set_style_text_color(ui_Home_LabelApplication, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_LabelApplication, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_ImageWeather = lv_image_create(ui_Home);
    lv_image_set_src(ui_Home_ImageWeather, &ui_img_weather_sunny_png);
    lv_obj_set_width(ui_Home_ImageWeather, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_ImageWeather, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_ImageWeather, -80);
    lv_obj_set_y(ui_Home_ImageWeather, -15);
    lv_obj_set_align(ui_Home_ImageWeather, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Home_ImageWeather, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Home_ImageWeather, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Home_LabelWeather = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_LabelWeather, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_LabelWeather, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_LabelWeather, -35);
    lv_obj_set_y(ui_Home_LabelWeather, -15);
    lv_obj_set_align(ui_Home_LabelWeather, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_LabelWeather, "Sunny\n28/15 °C");
    lv_obj_set_style_text_color(ui_Home_LabelWeather, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_LabelWeather, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_ImageHeart = lv_image_create(ui_Home);
    lv_image_set_src(ui_Home_ImageHeart, &ui_img_heartbeat_png);
    lv_obj_set_width(ui_Home_ImageHeart, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_ImageHeart, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_ImageHeart, 20);
    lv_obj_set_y(ui_Home_ImageHeart, -15);
    lv_obj_set_align(ui_Home_ImageHeart, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Home_ImageHeart, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Home_ImageHeart, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Home_LabelHeart = lv_label_create(ui_Home);
    lv_obj_set_width(ui_Home_LabelHeart, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_LabelHeart, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_LabelHeart, 70);
    lv_obj_set_y(ui_Home_LabelHeart, -15);
    lv_obj_set_align(ui_Home_LabelHeart, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_LabelHeart, "75 BPM");
    lv_obj_set_style_text_color(ui_Home_LabelHeart, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_LabelHeart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Home_ArcBattery = lv_arc_create(ui_Home);
    lv_obj_set_width(ui_Home_ArcBattery, 50);
    lv_obj_set_height(ui_Home_ArcBattery, 55);
    lv_obj_set_x(ui_Home_ArcBattery, 70);
    lv_obj_set_y(ui_Home_ArcBattery, -80);
    lv_obj_set_align(ui_Home_ArcBattery, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_Home_ArcBattery, LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_arc_set_value(ui_Home_ArcBattery, 78);
    lv_obj_set_style_arc_width(ui_Home_ArcBattery, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Home_ArcBattery, lv_color_hex(0xEEEEEE), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Home_ArcBattery, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_Home_ArcBattery, lv_color_hex(0x00ADB5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_Home_ArcBattery, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_Home_ArcBattery, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_opa(ui_Home_ArcBattery, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_Home_ImageBattery = lv_image_create(ui_Home_ArcBattery);
    lv_image_set_src(ui_Home_ImageBattery, &ui_img_battery_png);
    lv_obj_set_width(ui_Home_ImageBattery, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_ImageBattery, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Home_ImageBattery, 0);
    lv_obj_set_y(ui_Home_ImageBattery, 20);
    lv_obj_set_align(ui_Home_ImageBattery, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Home_ImageBattery, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_Home_ImageBattery, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Home_LabelBattery = lv_label_create(ui_Home_ArcBattery);
    lv_obj_set_width(ui_Home_LabelBattery, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Home_LabelBattery, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Home_LabelBattery, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Home_LabelBattery, "78%");
    lv_obj_set_style_text_color(ui_Home_LabelBattery, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Home_LabelBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Home_PanelApplication, ui_event_Home_PanelApplication, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Home, ui_event_Home, LV_EVENT_ALL, NULL);

}
