#include "mini_app_lancher.h"
#include "mini_app_defines.h"
#include "mini_app_registry.h"
#include "pm.h"

lv_pm_instance_t *pm_instance()
{
    static lv_pm_instance_t pm_instance;
    return &pm_instance;
}

mini_app_launcher_t *mini_app_launcher()
{
    static mini_app_launcher_t mini_app_launcher;
    return &mini_app_launcher;
}

void mini_app_launcher_init(mini_app_launcher_t *p_launcher)
{
    mui_app_inst_dict_init(p_launcher->app_inst_dict);
    p_launcher->pm_instance = pm_instance();
    uint8_t err = lv_pm_init(p_launcher->pm_instance, mini_app_registry_get_app_num());
    printf("lv_pm_init %d\n", err);
    for (uint32_t i = 0; i < mini_app_registry_get_app_num(); i++)
    {
        const mini_app_t *app = mini_app_registry_find_by_index(i);
        lv_pm_page_t *page = lv_pm_create_page(p_launcher->pm_instance, app->id);
        page->onLoad = app->onLoad;
        page->willAppear = app->willAppear;
        page->didAppear = app->didAppear;
        page->willDisappear = app->willDisappear;
        page->didDisappear = app->didDisappear;
        page->unLoad = app->unLoad;
    }

    mini_app_launcher_run(p_launcher, MINI_APP_ID_DESKTOP);
}

mini_app_inst_t *mini_app_get_inst(mini_app_launcher_t *p_launcher, uint32_t id)
{
    mini_app_inst_t **pp_app_inst = mui_app_inst_dict_get(p_launcher->app_inst_dict, id);
    mini_app_inst_t *p_app_inst = NULL;
    if (pp_app_inst)
    {
        p_app_inst = *pp_app_inst;
    }
    return p_app_inst;
}

void mini_app_launcher_run(mini_app_launcher_t *p_launcher, uint32_t id)
{
    mini_app_inst_t **pp_app_inst = mui_app_inst_dict_get(p_launcher->app_inst_dict, id);
    mini_app_inst_t *p_app_inst = NULL;
    if (pp_app_inst)
    {
        p_app_inst = *pp_app_inst;
    }
    if (p_app_inst)
    {
        lv_pm_open_page(p_launcher->pm_instance, id, NULL, p_app_inst);
    }
    else
    {
        p_app_inst = malloc(sizeof(mini_app_inst_t));
        p_app_inst->p_app = mini_app_registry_find_by_id(id);
        p_app_inst->state = APP_STATE_RUNNING;
        mui_app_inst_dict_set_at(p_launcher->app_inst_dict, id, p_app_inst);
        lv_pm_open_page(p_launcher->pm_instance, id, NULL, p_app_inst);
    }
}

void mini_app_launcher_back(mini_app_launcher_t *p_launcher)
{
    lv_pm_back(p_launcher->pm_instance);
}

mini_app_inst_t *mini_app_get_inst_by_page(lv_pm_page_inst_t *inst)
{
    return (mini_app_inst_t *)inst->user_data;
}