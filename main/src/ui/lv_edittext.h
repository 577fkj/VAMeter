#ifndef __LV_EDITTEXT__H
#define __LV_EDITTEXT__H

#include "lvgl.h"

static const char *const kb_map_num[] = {"1", "2", "3", LV_SYMBOL_OK, "\n",
                                         "4", "5", "6", LV_SYMBOL_BACKSPACE, "\n",
                                         "7", "8", "9", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, "\n",
                                         " ", "0", ".", " ", " ", ""};

static const lv_btnmatrix_ctrl_t kb_ctrl_num_map[] = {
    1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS | 2,
    1, 1, 1, 2,
    1, 1, 1, 1, 1,
    LV_BTNMATRIX_CTRL_HIDDEN, 1, 1, LV_BTNMATRIX_CTRL_HIDDEN, LV_BTNMATRIX_CTRL_HIDDEN};

typedef enum
{
    LV_EDITTEXT_EVENT_VALUE_CHANGED = 0,
    LV_EDITTEXT_EVENT_APPLY,
} lv_edittext_event_t;

typedef void (*lv_edittext_cb_t)(struct lv_edittext_t *edittext, lv_edittext_event_t event, const char *text);

typedef struct
{
    lv_obj_t *parent;
    lv_obj_t *edittext;
    lv_obj_t *label;
    lv_obj_t *textarea;
    lv_obj_t *kb;
    lv_edittext_cb_t event_cb;
    uint32_t max_length;

    void *user_data;
    uint32_t id;

    lv_coord_t original_width;
    lv_coord_t x;
    lv_coord_t y;
} lv_edittext_t;

lv_edittext_t *lv_edittext_create(lv_obj_t *parent, lv_obj_t *kb, const char *label, const char *text);
void lv_edittext_destroy(lv_edittext_t *edittext);

void lv_edittext_set_text(lv_edittext_t *edittext, const char *text);

const char *lv_edittext_get_text(lv_edittext_t *edittext);

void lv_edittext_set_max_length(lv_edittext_t *edittext, uint32_t max_length);

void lv_edittext_set_callback(lv_edittext_t *edittext, lv_edittext_cb_t event_cb);

void lv_edittext_set_group(lv_edittext_t *edittext, lv_group_t *group);

void lv_edittext_set_accepted_chars(lv_edittext_t *edittext, const char *accepted_chars);

void lv_edittext_set_user_data(lv_edittext_t *edittext, void *user_data);

void *lv_edittext_get_user_data(lv_edittext_t *edittext);

void lv_edittext_set_id(lv_edittext_t *edittext, uint32_t id);

uint32_t lv_edittext_get_id(lv_edittext_t *edittext);

void lv_edittext_set_placeholder_text(lv_edittext_t *edittext, const char *text);

#endif // __LV_EDITTEXT__H