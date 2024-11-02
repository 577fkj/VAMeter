#pragma once
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

typedef struct
{
    uint16_t lcd_height_res;
    uint16_t lcd_vertical_res;
    uint16_t lcd_draw_buffer_height;
    uint16_t lcd_bits_per_pixel;
    esp_lcd_color_space_t lcd_color_space;
    spi_host_device_t spi_host_device;
    gpio_num_t backlight;
    gpio_num_t sclk;
    gpio_num_t mosi;
    gpio_num_t dc;
    gpio_num_t cs;
    gpio_num_t rst;
} lcd_config_t;

extern esp_lcd_panel_io_handle_t lcd_io;
extern esp_lcd_panel_handle_t lcd_panel;

esp_err_t lcd_init(lcd_config_t lcd_config);
