#ifndef __LV_CUSTOM_LIST_H
#define __LV_CUSTOM_LIST_H

#include "lvgl.h"

typedef struct _list_t list_t;
typedef struct _list_item_t list_item_t;

typedef void (*lv_list_event_cb_t)(list_t *list, lv_event_t *event, void *user_data, void *id);

typedef struct _list_t
{
    lv_obj_t *list;
    lv_group_t *group;
    lv_list_event_cb_t event_cb;
    void *user_data;
    list_item_t **items;
    uint16_t item_count;
} list_t;

typedef struct _list_item_t
{
    const char *icon;
    const char *text;
    void *user_data;
    list_t *list;
} list_item_t;

list_t *custom_list_create(lv_obj_t *parent, lv_group_t *group);

lv_obj_t *add_list_item(list_t *list, const char *icon, const char *text, void *id);

void set_list_event_cb(list_t *list_obj, lv_list_event_cb_t event_cb);

void custom_list_deinit(list_t *list_obj);

void set_list_user_data(list_t *list_obj, void *user_data);

#endif // __LV_CUSTOM_LIST_H