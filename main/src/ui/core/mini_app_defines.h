#ifndef MINI_APP_DEFINES_H
#define MINI_APP_DEFINES_H

#include <stdint.h>
#include "lvgl.h"
#include "pm.h"

struct mini_app_inst_s;
typedef struct mini_app_inst_s mini_app_inst_t;

struct mini_app_s;
typedef struct mini_app_s mini_app_t;

typedef enum
{
    APP_STATE_RUNNING,
    APP_STATE_KILLED
} mini_app_state_t;

struct mini_app_inst_s
{
    lv_obj_t *page;
    const mini_app_t *p_app;
    mini_app_state_t state;
    void *p_handle;
};

typedef void (*mini_app_load_cb_t)(lv_obj_t *page);
typedef void (*mini_app_unload_cb_t)(lv_obj_t *page);

struct mini_app_s
{
    uint32_t id;
    char name[16];
    uint8_t hide;
    lv_pm_lifecycle onLoad;
    lv_pm_lifecycle willAppear;
    lv_pm_lifecycle didAppear;
    lv_pm_lifecycle willDisappear;
    lv_pm_lifecycle didDisappear;
    lv_pm_lifecycle unLoad;
};

#endif