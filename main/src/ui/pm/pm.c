#include "pm.h"
#include "anima.h"
#include "pm_utils.h"

#include "lv_port_indev.h"

#include <stdlib.h>
#include <stdio.h>

static void _appear_complete_cb(lv_pm_page_t *pm_page, lv_pm_open_options_t options)
{
    if (pm_page->didAppear)
    {
        pm_page->didAppear(pm_page->inst);
    }
}

static void _back_appear_complete_cb(lv_pm_page_t *pm_page, lv_pm_open_options_t options)
{
    if (pm_page->didAppear)
    {
        pm_page->didAppear(pm_page->inst);
    }
}

static void _disAppear_complete_cb(lv_pm_page_t *pm_page, lv_pm_open_options_t options)
{
    if (options.animation != LV_PM_ANIMA_POPUP)
    {
        lv_obj_add_flag(pm_page->inst->page, LV_OBJ_FLAG_HIDDEN);
    }
    if (pm_page->didDisappear)
    {
        pm_page->didDisappear(pm_page->inst->page);
    }
    if (options.target == LV_PM_TARGET_SELF)
    {
        lv_group_remove_all_objs(pm_page->inst->group);
        pm_page->unLoad(pm_page->inst);
        lv_obj_clean(pm_page->inst->page);

        pm_page->inst->user_data = NULL;
    }
}

static void _back_disAppear_complete_cb(lv_pm_page_t *pm_page, lv_pm_open_options_t options)
{
    lv_obj_add_flag(pm_page->inst->page, LV_OBJ_FLAG_HIDDEN);
    if (pm_page->didDisappear)
    {
        pm_page->didDisappear(pm_page->inst);
    }
    lv_group_remove_all_objs(pm_page->inst->group);
    pm_page->unLoad(pm_page->inst);
    lv_obj_clean(pm_page->inst->page);

    pm_page->inst->user_data = NULL;
}

uint8_t lv_pm_init(lv_pm_instance_t *pm_instance, size_t page_num)
{
    if (pm_instance == NULL)
    {
        return 1;
    }
    pm_instance->page_num = page_num;
    pm_instance->page = (lv_pm_page_t **)malloc(sizeof(lv_pm_page_t *) * page_num);
    if (pm_instance->page == NULL)
    {
        return 2;
    }
    pm_instance->history = (uint8_t *)malloc(sizeof(uint8_t) * page_num);
    if (pm_instance->history == NULL)
    {
        return 3;
    }
    memset(pm_instance->history, 0, sizeof(uint8_t) * page_num);
    memset(pm_instance->page, 0, sizeof(lv_pm_page_t *) * page_num);

    if (pm_instance->screen == NULL)
    {
        pm_instance->screen = lv_scr_act();
    }
    // turn off the scroll bar
    lv_obj_set_scrollbar_mode(pm_instance->screen, LV_SCROLLBAR_MODE_OFF);

    return 0;
}

lv_pm_page_t *lv_pm_create_page(lv_pm_instance_t *pm_instance, uint8_t id)
{
    lv_pm_page_t *pm_page = (lv_pm_page_t *)malloc(sizeof(lv_pm_page_t));
    if (pm_page == NULL)
    {
        return NULL;
    }
    memset(pm_page, 0, sizeof(lv_pm_page_t));

    pm_instance->page[id] = pm_page;
    lv_obj_t *page = lv_obj_create(pm_instance->screen);
    // reset style. border radius etc...
    pm_reset_style(page);
    lv_obj_add_flag(page, LV_OBJ_FLAG_HIDDEN);
    lv_coord_t width = lv_disp_get_hor_res(NULL);
    lv_coord_t height = lv_disp_get_ver_res(NULL);
    lv_obj_set_width(page, width);
    lv_obj_set_height(page, height);

    pm_page->inst = (lv_pm_page_inst_t *)malloc(sizeof(lv_pm_page_inst_t));
    pm_page->inst->group = lv_group_create();
    pm_page->inst->page = page;

    printf("id = %d, pm_page = %p, inst = %p, page = %p\n", id, pm_page, pm_page->inst, page);

    return pm_page;
}

uint8_t lv_pm_open_page(lv_pm_instance_t *pm_instance, uint8_t id, lv_pm_open_options_t *behavior, void *user_data)
{
    if (pm_instance->page[id] == NULL)
    {
        return 4;
    }
    if (pm_instance->history_offset >= pm_instance->page_num)
    {
        return 5;
    }
    pm_instance->history[pm_instance->history_offset] = id;
    lv_pm_page_t *pm_page = pm_instance->page[id];
    lv_obj_t *page = pm_page->inst->page;
    if (behavior)
    {
        pm_page->_options = *behavior;
    }
    pm_page->_back = false;

    if (pm_instance->history_offset > 0)
    {
        uint8_t pid = pm_instance->history[pm_instance->history_offset - 1];
        lv_pm_page_t *prev_pm_page = pm_instance->page[pid];
        prev_pm_page->_back = false;
        if (prev_pm_page->willDisappear)
        {
            prev_pm_page->willDisappear(prev_pm_page->inst);
        }
        _pm_anima_disAppear(prev_pm_page, &pm_page->_options, _disAppear_complete_cb);
    }

    lv_indev_set_group(indev_keypad, pm_page->inst->group);

    pm_page->inst->user_data = user_data;
    pm_page->onLoad(pm_page->inst);

    lv_obj_clear_flag(page, LV_OBJ_FLAG_HIDDEN);
    if (pm_page->willAppear)
    {
        pm_page->willAppear(pm_page->inst);
    }
    _pm_anima_appear(pm_page, &pm_page->_options, _appear_complete_cb);

    if (behavior && behavior->target == LV_PM_TARGET_SELF)
    {
        if (pm_instance->history_offset == 0)
        {
            pm_instance->history_offset++;
        }
        else
        {
            pm_instance->page[pm_instance->history_offset - 1] = pm_instance->page[pm_instance->history_offset];
        }
    }
    else
    {
        pm_instance->history_offset++;
    }

    return 0;
}

uint8_t lv_pm_back(lv_pm_instance_t *pm_instance)
{
    if (pm_instance->history_offset < 2)
    {
        return 0;
    }
    uint8_t pid = pm_instance->history[pm_instance->history_offset - 1];
    lv_pm_page_t *pm_page = pm_instance->page[pid];
    pm_page->_back = true;

    if (pm_page->willDisappear)
    {
        pm_page->willDisappear(pm_page->inst);
    }
    _pm_anima_disAppear(pm_page, &pm_page->_options, _back_disAppear_complete_cb);

    pm_instance->history_offset--;
    uint8_t prev_pid = pm_instance->history[pm_instance->history_offset - 1];
    lv_pm_page_t *prev_pm_page = pm_instance->page[prev_pid];
    prev_pm_page->_back = true;

    lv_indev_set_group(indev_keypad, prev_pm_page->inst->group);

    if (prev_pm_page->willAppear)
    {
        prev_pm_page->willAppear(prev_pm_page->inst);
    }
    lv_obj_clear_flag(prev_pm_page->inst->page, LV_OBJ_FLAG_HIDDEN);
    _pm_anima_appear(prev_pm_page, &pm_page->_options, _back_appear_complete_cb);

    return 0;
}

void lv_pm_deinit(lv_pm_instance_t *pm_instance)
{
    for (uint8_t i = 0; i < pm_instance->page_num; i++)
    {
        if (pm_instance->page[i])
        {
            pm_instance->page[i]->unLoad(pm_instance->page[i]->inst);
            lv_group_remove_all_objs(pm_instance->page[i]->inst->group);
            lv_group_del(pm_instance->page[i]->inst->group);
            lv_obj_clean(pm_instance->page[i]->inst->page);
            free(pm_instance->page[i]->inst);
            free(pm_instance->page[i]);
        }
    }
    free(pm_instance->page);
    free(pm_instance->history);
    pm_instance->page = NULL;
    pm_instance->history = NULL;
    pm_instance->history_offset = 0;
    pm_instance->page_num = 0;

    lv_obj_clean(pm_instance->screen);
}