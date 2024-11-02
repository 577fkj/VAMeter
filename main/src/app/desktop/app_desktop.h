#ifndef APP_DESKTOP_H
#define APP_DESKTOP_H

#include "mini_app_defines.h"

typedef struct
{
    lv_obj_t *voltage;
    lv_obj_t *current;
    lv_obj_t *power;
    lv_obj_t *ah;
    lv_obj_t *wh;
    lv_obj_t *time;
    lv_obj_t *run_info;
} desktop_data_t;

extern const mini_app_t app_desktop_info;

#endif // APP_DESKTOP_H