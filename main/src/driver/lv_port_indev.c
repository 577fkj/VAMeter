/**
 * @file lv_port_indev.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "lvgl.h"
#include "driver/gpio.h"
#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static uint32_t keypad_get_key(const lv_group_t *group);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t *indev_keypad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Keypad
     * -----------------*/

    /*Initialize your keypad or keyboard if you have*/
    keypad_init();

    /*Register a keypad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{
    /*Your code comes here*/
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << LEFT_KEY) | (1ULL << RIGHT_KEY) | (1ULL << CENTER_KEY),
    };
    gpio_config(&io_conf);
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;

    /*Get the current x and y coordinates*/
    // mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key(indev_keypad->group);
    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch (act_key)
        {
        case 1:
            act_key = LV_KEY_NEXT;
            break;
        case 2:
            act_key = LV_KEY_PREV;
            break;
        case 3:
            act_key = LV_KEY_LEFT;
            break;
        case 4:
            act_key = LV_KEY_RIGHT;
            break;
        case 5:
            act_key = LV_KEY_ENTER;
            break;
        }

        // printf("act_key = %ld, count = %ld\n", act_key, lv_group_get_obj_count(indev_keypad->group));
        // printf("indev_keypad->group = %p\n", indev_keypad->group);
        // printf("edit = %d, act_key = %ld\n", lv_group_get_editing(indev_keypad->group), act_key);

        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(const lv_group_t *group)
{
    /*Your code comes here*/
    if (gpio_get_level(LEFT_KEY) == 0)
    {
        vTaskDelay(40 / portTICK_PERIOD_MS);
        if (gpio_get_level(LEFT_KEY) == 0)
        {
            if (lv_group_get_editing(group))
            {
                return 3;
            }
            else
                return 2;
        }
    }
    else if (gpio_get_level(RIGHT_KEY) == 0)
    {
        vTaskDelay(40 / portTICK_PERIOD_MS);
        if (gpio_get_level(RIGHT_KEY) == 0)
        {
            if (lv_group_get_editing(group))
            {
                return 4;
            }
            else
                return 1;
        }
    }
    else if (gpio_get_level(CENTER_KEY) == 0)
    {
        vTaskDelay(40 / portTICK_PERIOD_MS);
        if (gpio_get_level(CENTER_KEY) == 0)
            return 5;
    }

    return 0;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
