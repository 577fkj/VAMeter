#include "mui_scene_dispatcher.h"

#include "lvgl.h"

#include "pm.h"

void scene_onLoad(const lv_pm_page_inst_t *inst)
{
    mui_scene_dispatcher_t *dispatcher = (mui_scene_dispatcher_t *)inst->user_data;
    mui_scene_t *scene = &dispatcher->scene_defines[dispatcher->current_scene_id];
    // lv_obj_clean(dispatcher->pm_instance->screen);
    if (scene->enter_cb)
    {
        scene->enter_cb(inst, dispatcher->user_data);
    }
}

void scene_onUnload(const lv_pm_page_inst_t *inst)
{
    mui_scene_dispatcher_t *dispatcher = (mui_scene_dispatcher_t *)inst->user_data;
    mui_scene_t *scene = &dispatcher->scene_defines[dispatcher->current_scene_id];
    if (scene->exit_cb)
    {
        scene->exit_cb(inst, dispatcher->user_data);
    }
    // lv_obj_clean(dispatcher->pm_instance->screen);
}

mui_scene_dispatcher_t *mui_scene_dispatcher_create(lv_pm_page_inst_t *inst, const mui_scene_t *scene_defines, uint32_t max_scene_num)
{
    mui_scene_dispatcher_t *p_dispatcher = (mui_scene_dispatcher_t *)malloc(sizeof(mui_scene_dispatcher_t));
    p_dispatcher->user_data = NULL;

    p_dispatcher->max_scene_num = max_scene_num;
    p_dispatcher->scene_defines = scene_defines;

    p_dispatcher->pm_instance = (lv_pm_instance_t *)malloc(sizeof(lv_pm_instance_t));
    memset(p_dispatcher->pm_instance, 0, sizeof(lv_pm_instance_t));
    p_dispatcher->pm_instance->screen = inst->page;
    lv_pm_init(p_dispatcher->pm_instance, max_scene_num);

    for (uint32_t i = 0; i < max_scene_num; i++)
    {
        mui_scene_t *p_scene = &scene_defines[i];
        mui_scene_dispatcher_add(p_dispatcher, i, p_scene);
    }

    return p_dispatcher;
}

void mui_scene_dispatcher_free(mui_scene_dispatcher_t *p_dispatcher)
{
    free(p_dispatcher->pm_instance);
    free(p_dispatcher);
}

void mui_scene_dispatcher_set_user_data(mui_scene_dispatcher_t *p_dispatcher, void *user_data)
{
    p_dispatcher->user_data = user_data;
}

void mui_scene_dispatcher_previous_scene(mui_scene_dispatcher_t *p_dispatcher)
{
    lv_pm_back(p_dispatcher->pm_instance);
}

void mui_scene_dispatcher_goto_scene(mui_scene_dispatcher_t *p_dispatcher, uint32_t scene_id)
{
    p_dispatcher->current_scene_id = scene_id;
    uint8_t err = lv_pm_open_page(p_dispatcher->pm_instance, scene_id, NULL, p_dispatcher);
    printf("lv_pm_open_page %d\n", err);
}

void mui_scene_dispatcher_add(mui_scene_dispatcher_t *p_dispatcher, uint32_t scene_id, mui_scene_t *p_scene)
{
    lv_pm_page_t *page = lv_pm_create_page(p_dispatcher->pm_instance, scene_id);
    page->onLoad = scene_onLoad;
    page->unLoad = scene_onUnload;
}