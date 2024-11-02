#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#include "ina228.h"

extern ina228_config_t ina228_conf;

#define BEEP_GPIO GPIO_NUM_16
#define NTC_GPIO GPIO_NUM_3 // ADC1_CH2

// 扩展接口
#define extend_gpio_1 GPIO_NUM_1 // ADC1_CH0
#define extend_gpio_2 GPIO_NUM_2 // ADC1_CH1
#define extend_gpio_4 GPIO_NUM_42
#define extend_gpio_5 GPIO_NUM_41
#define extend_gpio_6 GPIO_NUM_40

// 内部接口
#define internal_gpio_1 GPIO_NUM_4 // ADC1_CH3
#define internal_gpio_2 GPIO_NUM_5 // ADC1_CH4
#define internal_gpio_3 GPIO_NUM_6 // ADC1_CH5
#define internal_gpio_4 GPIO_NUM_7 // ADC1_CH6

// INA228
#define INA228_ADDR 0x40
#define INA228_SCL GPIO_NUM_48
#define INA228_SDA GPIO_NUM_47
#define INA228_ALERT GPIO_NUM_21

// st7789
#define ST7789_SDIN GPIO_NUM_11
#define ST7789_SCLK GPIO_NUM_12
#define ST7789_CS GPIO_NUM_10
#define ST7789_DC GPIO_NUM_13
#define ST7789_RST GPIO_NUM_9
#define ST7789_BL GPIO_NUM_46

// key
#define LEFT_KEY GPIO_NUM_36
#define RIGHT_KEY GPIO_NUM_35
#define CENTER_KEY GPIO_NUM_0

void app_main(void);

#endif // __MAIN_H