#ifndef __MUI_SCENE_DISPATCHER_H__
#define __MUI_SCENE_DISPATCHER_H__

#include "lvgl.h"

#include "pm.h"

typedef void (*mui_scene_enter_cb_t)(const lv_pm_page_inst_t *inst, void *user_data);
typedef void (*mui_scene_exit_cb_t)(const lv_pm_page_inst_t *inst, void *user_data);

typedef struct
{
    uint32_t scene_id;
    mui_scene_enter_cb_t enter_cb;
    mui_scene_exit_cb_t exit_cb;
} mui_scene_t;

typedef struct
{
    void *user_data;
    uint32_t current_scene_id;
    const mui_scene_t *scene_defines;
    uint32_t max_scene_num;
    lv_pm_instance_t *pm_instance;
} mui_scene_dispatcher_t;

mui_scene_dispatcher_t *mui_scene_dispatcher_create(lv_pm_page_inst_t *inst, const mui_scene_t *scene_defines, uint32_t max_scene_num);
void mui_scene_dispatcher_free(mui_scene_dispatcher_t *p_dispatcher);

void mui_scene_dispatcher_add(mui_scene_dispatcher_t *p_dispatcher, uint32_t scene_id, mui_scene_t *p_scene);
void mui_scene_dispatcher_set_user_data(mui_scene_dispatcher_t *p_dispatcher, void *user_data);
void mui_scene_dispatcher_previous_scene(mui_scene_dispatcher_t *p_dispatcher);
void mui_scene_dispatcher_goto_scene(mui_scene_dispatcher_t *p_dispatcher, uint32_t scene_id);

#endif // __MUI_SCENE_DISPATCHER_H__