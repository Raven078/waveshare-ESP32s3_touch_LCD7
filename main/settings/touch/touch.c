#include "touch.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lcd_touch_gt911.h"

static const char *TAG = "touch";
static esp_lcd_touch_handle_t tp = NULL;
static bool was_pressed = false;   // защита от повторных срабатываний

void touch_init(void)
{
    ESP_LOGI(TAG, "Init GT911 (manual polling)");

    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = 0,
        .sda_io_num = 8,
        .scl_io_num = 9,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags = { .enable_internal_pullup = true },
    };
    i2c_master_bus_handle_t i2c_bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus_handle));

    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    esp_lcd_panel_io_handle_t io_handle;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &io_config, &io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = 800,
        .y_max = 480,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = GPIO_NUM_NC,
        .levels = { .reset = 0, .interrupt = 0 },
        .flags = { .swap_xy = 0, .mirror_x = 0, .mirror_y = 0 },
    };
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(io_handle, &tp_cfg, &tp));
    ESP_LOGI(TAG, "GT911 ready");
}

bool touch_get_tap(uint16_t *x, uint16_t *y)
{
    if (!tp) return false;

    esp_lcd_touch_read_data(tp);
    esp_lcd_touch_point_data_t point;
    uint8_t touch_cnt = 0;
    esp_err_t err = esp_lcd_touch_get_data(tp, &point, &touch_cnt, 1);
    bool pressed = (err == ESP_OK && touch_cnt > 0);

    if (pressed && !was_pressed) {
        *x = point.x;
        *y = point.y;
        was_pressed = true;
        return true;
    }
    if (!pressed) {
        was_pressed = false;
    }
    return false;
}