#include "main.h"

#include "st7789.h"
#include "ina228.h"
#include "iic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_lvgl_port.h"
#include "lv_port_indev.h"

#include "lvgl.h"

#include "pm.h"
#include "mini_app_lancher.h"

#include <stdio.h>

#include "esp_timer.h"
#include "app_desktop.h"
#include "mini_app_registry.h"

#include "task.h"
#include "config.h"

lv_disp_t *lvgl_disp = NULL;

esp_err_t lvgl_init(lcd_config_t lcd_config)
{
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    esp_err_t err = lvgl_port_init(&lvgl_cfg);
    printf("lvgl_port_init %d\n", err);

    /* Add LCD screen */
    printf("Add LCD scree\n");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = lcd_config.lcd_height_res * lcd_config.lcd_draw_buffer_height * sizeof(uint16_t),
        .double_buffer = 1,
        .hres = lcd_config.lcd_height_res,
        .vres = lcd_config.lcd_vertical_res,
        .monochrome = false,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
            .sw_rotate = true, // software rotation
        },
    };

    lvgl_disp = lvgl_port_add_disp(&disp_cfg);
    return ESP_OK;
}

ina228_config_t ina228_conf = {
    .i2c_master_port = I2C_CONTROLLER_0,
    .address = INA228_ADDR,
    .max_current = 50,
    .r_shunt = 0.003, // 3mOhm
};

void app_main(void)
{
    config_init();
    config_t *config = get_config();

    ina228_conf.r_shunt = config->rshunt * 0.001;
    ina228_conf.max_current = config->max_current;

    i2c_init(I2C_CONTROLLER_0, INA228_SDA, INA228_SCL);
    ina228_init(&ina228_conf);
    printf("Config Register:    0x%04X\r\n", *(uint16_t *)&ina228_conf.config_reg);
    printf("Current LSB: %f\n", ina228_conf.current_lsb);
    printf("Shunt Cal: %d\n", ina228_conf.shunt_cal);

    lcd_config_t lcd_config = {
        .sclk = ST7789_SCLK,
        .mosi = ST7789_SDIN,
        .cs = ST7789_CS,
        .dc = ST7789_DC,
        .rst = ST7789_RST,
        .backlight = ST7789_BL,
        .spi_host_device = SPI2_HOST,
        .lcd_height_res = 240,
        .lcd_vertical_res = 240,
        .lcd_draw_buffer_height = 50,
    };
    printf("lcd_init\n");
    lcd_init(lcd_config);
    printf("lvgl_init\n");
    lvgl_init(lcd_config);
    printf("start\n");
    lv_port_indev_init();

    lv_disp_set_rotation(lvgl_disp, LV_DISPLAY_ROTATION_270);

    mini_app_launcher_init(mini_app_launcher());

    init_ina228_task(mini_app_get_inst(mini_app_launcher(), MINI_APP_ID_DESKTOP));

    printf("init end\n");
}
