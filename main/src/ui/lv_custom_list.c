#include "lv_custom_list.h"

#include "lv_custom_font.h"

void __event_cb(lv_event_t *event)
{
    list_item_t *item = (list_item_t *)lv_event_get_user_data(event);
    if (item->list->event_cb)
    {
        item->list->event_cb(item->list, event, item->list->user_data, item->user_data);
    }
}

list_t *custom_list_create(lv_obj_t *parent, lv_group_t *group)
{
    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_border_side(list, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(list, &lv_font_siyuan_bold_16, LV_PART_MAIN | LV_STATE_ANY);

    list_t *list_obj = (list_t *)malloc(sizeof(list_t));
    list_obj->list = list;
    list_obj->event_cb = NULL;
    list_obj->group = group;
    list_obj->user_data = NULL;
    list_obj->items = NULL;
    list_obj->item_count = 0;
    return list_obj;
}

void custom_list_deinit(list_t *list_obj)
{
    for (int i = 0; i < list_obj->item_count; i++)
    {
        free(list_obj->items[i]);
    }
    free(list_obj->items);
    lv_obj_clean(list_obj->list);
    list_obj->group = NULL;
    list_obj->event_cb = NULL;
    list_obj->list = NULL;
    list_obj->user_data = NULL;
    free(list_obj);
}

void set_list_event_cb(list_t *list_obj, lv_list_event_cb_t event_cb)
{
    list_obj->event_cb = event_cb;
}

void set_list_user_data(list_t *list_obj, void *user_data)
{
    list_obj->user_data = user_data;
}

lv_obj_t *add_list_item(list_t *list, const char *icon, const char *text, void *id)
{
    lv_obj_t *btn = lv_list_add_btn(list->list, icon, text);

    lv_obj_set_style_text_font(btn, &lv_font_siyuan_chinese_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *btn2 = lv_obj_get_child(btn, 1);
    lv_obj_set_style_pad_top(btn2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(btn, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    list_item_t *item = (list_item_t *)malloc(sizeof(list_item_t));
    if (list->item_count == 0)
    {
        list->items = (list_item_t *)malloc(sizeof(list_item_t *));
    }
    else
    {
        list->items = (list_item_t *)realloc(list->items, (list->item_count + 1) * sizeof(list_item_t *));
    }

    list->items[list->item_count] = item;
    list->item_count++;

    item->icon = icon;
    item->text = text;
    item->user_data = id;
    item->list = list;

    lv_obj_add_event_cb(btn, __event_cb, LV_EVENT_CLICKED, item);
    lv_group_add_obj(list->group, btn);
    return btn;
}