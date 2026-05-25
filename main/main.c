#include "settings/display/display_init.h"
#include "settings/touch/touch.h"
#include "settings/wifi/wifi_ap.h"
#include "settings/wifi/tcp_server.h"
#include "settings/time/rtc_time.h"
#include "GUI/ui/ui.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

void app_main(void)
{
    ESP_LOGI("main", "Запуск приложения");

    // 1. Инициализация NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW("main", "NVS: стирание и повторная инициализация...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "NVS инициализирован");

    // 2. Инициализация RTC времени
    rtc_time_init();

    // 3. Инициализация дисплея и LVGL
    lv_display_t *disp = display_init();
    if (!disp) {
        ESP_LOGE("main", "Ошибка инициализации дисплея");
        while (1) vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ESP_LOGI("main", "Display initialized");

    // 4. Инициализация сенсорного экрана
    touch_init();

    // 5. Создание UI
    ui_create_wallpaper();

    // 6. Запуск Wi-Fi точки доступа
    wifi_init_softap();

    // 7. Запуск TCP-сервера на порту 8888
    tcp_server_start(8888);

    // 8. Главный цикл
    while (1) {
        ui_handle_touch();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}