#include "ui.h"
#include "../wallpaper/wallpaper800400.h"
#include "../fonts/my_arial24.h"
#include "../../settings/time/rtc_time.h"
#include "esp_log.h"
#include <stdio.h>

static const char *TAG = "UI";
static lv_obj_t *main_screen = NULL;
static lv_obj_t *time_label = NULL;
static int last_minute = -1;


void update_time_timer_cb(lv_timer_t *timer)
{
    if (!time_label) return;
    
    time_t now = rtc_time_get_timestamp();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    // Обновляем только если минута изменилась
    if (timeinfo.tm_min != last_minute) {
        last_minute = timeinfo.tm_min;
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M", &timeinfo);
        lv_label_set_text(time_label, time_str);
    }
}

void ui_create_wallpaper(void)
{
    ESP_LOGI(TAG, "UI initialization");
    
    main_screen = lv_scr_act();
    lv_obj_clean(main_screen);
    
    // Фоновое изображение
    if (wallpaper800400.data != NULL && wallpaper800400.data_size > 0) {
        lv_obj_t *wallpaper_img = lv_image_create(main_screen);
        lv_image_set_src(wallpaper_img, &wallpaper800400);
        lv_obj_set_pos(wallpaper_img, 0, 0);
        lv_obj_set_size(wallpaper_img, 800, 480);
        lv_obj_remove_flag(wallpaper_img, LV_OBJ_FLAG_CLICKABLE);
    } else {
        lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x000033), 0);
        lv_obj_set_style_bg_opa(main_screen, LV_OPA_COVER, 0);
    }
    
    // Панель
    lv_obj_t *panel = lv_obj_create(main_screen);
    lv_obj_set_size(panel, 350, 140);
    lv_obj_set_pos(panel, 10, 10);
    lv_obj_set_style_bg_color(panel, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_60, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_remove_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    
    // Заголовок (русский текст с my_arial)
    lv_obj_t *title_label = lv_label_create(panel);
    lv_label_set_text(title_label, "ESP32-S3 Дисплей Готов");
    lv_obj_set_pos(title_label, 10, 5);
    lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(title_label, &my_arial24, 0);
    
    // Wi-Fi и IP (русский текст с my_arial)
    lv_obj_t *info_label = lv_label_create(panel);
    lv_label_set_text(info_label, "Wi-Fi: ESP32_S3_Display\nIP: 192.168.4.1\nПорт: 8888");
    lv_obj_set_pos(info_label, 10, 35);
    lv_obj_set_style_text_color(info_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(info_label, &my_arial24, 0);
    
    // Время (формат dd.mm.yyyy hh:mm)
    time_label = lv_label_create(panel);
    time_t now = rtc_time_get_timestamp();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    last_minute = timeinfo.tm_min;
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M", &timeinfo);
    lv_label_set_text(time_label, time_str);
    lv_obj_set_pos(time_label, 10, 95);
    lv_obj_set_style_text_color(time_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_set_style_text_font(time_label, &my_arial24, 0);
    
    // Запускаем таймер (каждую секунду, но обновляем только раз в минуту)
    lv_timer_create(update_time_timer_cb, 1000, NULL);
    
    ESP_LOGI(TAG, "UI ready, time: %s", time_str);
}

void ui_handle_touch(void)
{
}

lv_obj_t *ui_get_main_screen(void)
{
    if (!main_screen) {
        main_screen = lv_scr_act();
    }
    return main_screen;
}