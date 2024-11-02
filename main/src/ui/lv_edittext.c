#include "lv_edittext.h"
#include "lvgl.h"

#include "lv_custom_font.h"

static void __textarea_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_edittext_t *data = lv_event_get_user_data(e);

    lv_obj_t *kb = data->kb;
    lv_group_t *g = lv_obj_get_group(kb);

    if (code == LV_EVENT_CLICKED)
    {
        lv_group_focus_obj(kb);
        lv_keyboard_set_textarea(kb, ta);
        lv_group_set_editing(g, true);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);

        data->original_width = lv_obj_get_width(ta);
        data->x = lv_obj_get_x(ta);
        data->y = lv_obj_get_y(ta);

        // 设置输入框位置在键盘上方
        lv_obj_set_parent(ta, kb->parent);
        lv_obj_align_to(ta, kb, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
        lv_obj_set_width(ta, lv_obj_get_width(kb));
        lv_obj_add_state(ta, LV_STATE_FOCUSED);

        // 设置键盘层级最高
        lv_obj_move_foreground(kb);
    }
    else if ((code == LV_EVENT_READY || code == LV_EVENT_CANCEL) && lv_group_get_editing(g) == true)
    {
        // 恢复原来布局
        lv_obj_set_pos(ta, data->x, data->y);
        lv_obj_set_parent(ta, data->edittext);
        lv_obj_set_width(ta, data->original_width);

        lv_group_focus_obj(ta);
        lv_keyboard_set_textarea(kb, NULL);
        lv_group_set_editing(g, false);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

        if (data->event_cb)
        {
            data->event_cb(data, LV_EDITTEXT_EVENT_APPLY, lv_textarea_get_text(ta));
        }
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        if (data->event_cb)
        {
            data->event_cb(data, LV_EDITTEXT_EVENT_VALUE_CHANGED, lv_textarea_get_text(ta));
        }
    }
}

lv_edittext_t *lv_edittext_create(lv_obj_t *parent, lv_obj_t *kb, const char *label, const char *text)
{
    lv_edittext_t *edittext = (lv_edittext_t *)malloc(sizeof(lv_edittext_t));
    memset(edittext, 0, sizeof(lv_edittext_t));

    edittext->kb = kb;
    edittext->parent = parent;

    edittext->edittext = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(edittext->edittext, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(edittext->edittext, 210, 50);
    lv_obj_set_layout(edittext->edittext, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(edittext->edittext, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_opa(edittext->edittext, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(edittext->edittext, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(edittext->edittext, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(edittext->edittext, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(edittext->edittext, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    edittext->label = lv_label_create(edittext->edittext);
    lv_obj_set_size(edittext->label, 80, 40);
    lv_label_set_text(edittext->label, label);
    lv_obj_align(edittext->label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_font(edittext->label, &lv_font_siyuan_chinese_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(edittext->label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(edittext->label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    edittext->textarea = lv_textarea_create(edittext->edittext);
    lv_textarea_set_one_line(edittext->textarea, true);
    lv_obj_set_size(edittext->textarea, 120, 40);
    lv_textarea_set_text(edittext->textarea, text);
    lv_obj_align(edittext->textarea, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(edittext->textarea, __textarea_event_handler, LV_EVENT_ALL, edittext);
    lv_obj_set_style_text_font(edittext->textarea, &lv_font_siyuan_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(edittext->textarea, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(edittext->textarea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    return edittext;
}

void lv_edittext_destroy(lv_edittext_t *edittext)
{
    lv_obj_clean(edittext->edittext);
    free(edittext);
}

void lv_edittext_set_text(lv_edittext_t *edittext, const char *text)
{
    lv_label_set_text(edittext->textarea, text);
}

const char *lv_edittext_get_text(lv_edittext_t *edittext)
{
    return lv_label_get_text(edittext->textarea);
}

void lv_edittext_set_max_length(lv_edittext_t *edittext, uint32_t max_length)
{
    lv_textarea_set_max_length(edittext->textarea, max_length);
}

void lv_edittext_set_callback(lv_edittext_t *edittext, lv_edittext_cb_t event_cb)
{
    edittext->event_cb = event_cb;
}

void lv_edittext_set_group(lv_edittext_t *edittext, lv_group_t *group)
{
    lv_group_add_obj(group, edittext->textarea);
}

void lv_edittext_set_accepted_chars(lv_edittext_t *edittext, const char *accepted_chars)
{
    lv_textarea_set_accepted_chars(edittext->textarea, accepted_chars);
}

void lv_edittext_set_placeholder_text(lv_edittext_t *edittext, const char *text)
{
    lv_textarea_set_placeholder_text(edittext->textarea, text);
}

void lv_edittext_set_user_data(lv_edittext_t *edittext, void *user_data)
{
    edittext->user_data = user_data;
}

void *lv_edittext_get_user_data(lv_edittext_t *edittext)
{
    return edittext->user_data;
}

void lv_edittext_set_id(lv_edittext_t *edittext, uint32_t id)
{
    edittext->id = id;
}

uint32_t lv_edittext_get_id(lv_edittext_t *edittext)
{
    return edittext->id;
}