#include "app_settings.h"
#include "mini_app_registry.h"
#include "mini_app_lancher.h"
#include "lvgl.h"
#include "pm.h"

#include "lv_custom_list.h"
#include "lv_custom_font.h"

#include "settings_scene.h"

#include <stdio.h>

static void button_event_handle(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
        mini_app_launcher_back(mini_app_launcher());
    }
}

void page_settings_onLoad(lv_pm_page_inst_t *inst)
{
    printf("page_settings_onLoad\n");

    mini_app_inst_t *app_inst = (mini_app_inst_t *)inst->user_data;
    app_inst->p_handle = malloc(sizeof(app_settings_data_t));
    app_settings_data_t *app = (app_settings_data_t *)app_inst->p_handle;

    app->dispatcher = mui_scene_dispatcher_create(inst, settings_scene_defines, SETTINGS_SCENE_MAX);
    mui_scene_dispatcher_set_user_data(app->dispatcher, app);

    mui_scene_dispatcher_goto_scene(app->dispatcher, SETTINGS_SCENE_MAIN);
}

void page_settings_unLoad(lv_pm_page_inst_t *inst)
{
    printf("page_settings_unLoad\n");
    mini_app_inst_t *app_inst = (mini_app_inst_t *)inst->user_data;
    app_settings_data_t *app = (app_settings_data_t *)app_inst->p_handle;
    mui_scene_dispatcher_free(app->dispatcher);
    free(app);

    app_inst->p_handle = NULL;
}

const mini_app_t app_settings_info = {
    .id = MINI_APP_ID_SETTINGS,
    .name = "settings",
    .onLoad = page_settings_onLoad,
    .unLoad = page_settings_unLoad,
};