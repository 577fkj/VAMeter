#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include "mini_app_defines.h"
#include "lvgl.h"
#include "pm.h"
#include "lv_custom_list.h"

#include "mui_scene_dispatcher.h"

typedef struct
{
    list_t *list;
    mui_scene_dispatcher_t *dispatcher;
    void *user_data;
} app_settings_data_t;

extern const mini_app_t app_settings_info;

#endif // APP_SETTINGS_H