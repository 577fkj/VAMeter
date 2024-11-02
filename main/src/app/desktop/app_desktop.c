#include "app_desktop.h"
#include "mini_app_registry.h"
#include "mini_app_lancher.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "pm.h"

#include "lv_custom_font.h"

#include <stdio.h>

#define VOLTAGE_COLOR lv_color_hex(0x00ff16)
#define CURRENT_COLOR lv_color_hex(0xfffe00)
#define POWER_COLOR lv_color_hex(0x00fffc)
#define AH_COLOR lv_color_hex(0x9800ff)
#define WH_COLOR lv_color_hex(0xff6500)
#define LINE_COLOR lv_color_hex(0x757575)

lv_obj_t *create_label(lv_obj_t *page, const char *text, lv_color_t color, const lv_font_t *font, int16_t x, int16_t y)
{
    lv_obj_t *label = lv_label_create(page);
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    lv_obj_set_align(label, LV_TEXT_ALIGN_RIGHT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_pos(label, x, y);
    lv_obj_set_size(label, 180, 46);

    lv_obj_set_style_text_color(label, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

lv_obj_t *create_unit(lv_obj_t *page, const char *text, lv_color_t color, const lv_font_t *font, int16_t x, int16_t y)
{
    lv_obj_t *label = lv_label_create(page);
    lv_label_set_text(label, text);
    lv_obj_set_align(label, LV_TEXT_ALIGN_CENTER);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_pos(label, x, y);
    lv_obj_set_size(label, 60, 46);

    lv_obj_set_style_text_color(label, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);

    return label;
}

lv_obj_t *create_button(lv_obj_t *page, const char *text, int16_t x, int16_t y)
{
    // Write codes main_btn_setting
    lv_obj_t *btn = lv_btn_create(page);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, text);
    lv_label_set_long_mode(btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(btn_label, LV_PCT(100));
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, 32, 32);

    // Write style for main_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x757575), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for main_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    // Write style for main_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    // Write style for main_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(btn, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(btn, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(btn, 5, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(btn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    return btn;
}

lv_obj_t *create_line(lv_obj_t *page, lv_color_t color, int16_t x, int16_t y, int16_t w, int16_t h, lv_point_t *point)
{
    lv_obj_t *line = lv_line_create(page);
    lv_line_set_points(line, point, 2);
    lv_obj_set_pos(line, x, y);
    lv_obj_set_size(line, w, h);

    // Write style for main_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(line, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(line, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(line, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    return line;
}

void button_event_handle(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t id = (uint8_t)lv_event_get_user_data(e);
    printf("button_event_handle code: %d, id: %d\n", code, id);
    if (code == LV_EVENT_CLICKED)
    {
        mini_app_launcher_run(mini_app_launcher(), id);
    }
}

void page_desktop_onLoad(lv_pm_page_inst_t *inst)
{
    lv_obj_t *page = inst->page;
    printf("desktop onLoad\n");
    lv_obj_set_size(page, 240, 240);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    // Write style for main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(page, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(page, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(page, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_coord_t x, y;

    x = -40;
    y = -15;

#define Y_OFFSET 40

#define VALUE_FONT &lv_font_siyuan_bold_34

    lv_obj_t *label_voltage = create_label(page, "00.0000", VOLTAGE_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    lv_obj_t *label_current = create_label(page, "00.0000", CURRENT_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    lv_obj_t *label_power = create_label(page, "00.0000", POWER_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    lv_obj_t *label_ah = create_label(page, "00.0000", AH_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    lv_obj_t *label_wh = create_label(page, "00.0000", WH_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    x = 100;
    y = -18;

    create_unit(page, "V", VOLTAGE_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    create_unit(page, "A", CURRENT_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    create_unit(page, "W", POWER_COLOR, VALUE_FONT, x, y);
    y += Y_OFFSET;

    y += 17;
    create_unit(page, "Ah", AH_COLOR, &lv_font_siyuan_bold_24, x, y);
    y += Y_OFFSET;

    create_unit(page, "Wh", WH_COLOR, &lv_font_siyuan_bold_24, x, y);

    // Write codes run_info
    lv_obj_t *run_info = lv_label_create(page);
    lv_label_set_text(run_info, "RUNNING");
    lv_label_set_long_mode(run_info, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(run_info, 82, 205);
    lv_obj_set_size(run_info, 80, 25);

    // Write style for run_info, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(run_info, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(run_info, &lv_font_siyuan_bold_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(run_info, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(run_info, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(run_info, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(run_info, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes main_label_13
    lv_obj_t *time = lv_label_create(page);
    lv_label_set_text(time, "00:00:00");
    lv_label_set_long_mode(time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(time, 0, 209);
    lv_obj_set_size(time, 74, 29);

    // Write style for main_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(time, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(time, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(time, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    static lv_point_t line_1[] = {
        {0, 0},
        {240, 0},
    };
    create_line(page, LINE_COLOR, 0, 204, 240, 2, line_1);
    static lv_point_t line_2[] = {
        {0, 0},
        {0, 34},
    };
    create_line(page, LINE_COLOR, 205, 205, 2, 34, line_2);
    static lv_point_t line_3[] = {
        {0, 0},
        {0, 34},
    };
    create_line(page, LINE_COLOR, 167, 205, 2, 34, line_3);
    static lv_point_t line_4[] = {
        {0, 0},
        {0, 34},
    };
    create_line(page, LINE_COLOR, 77, 205, 2, 34, line_4);

    lv_obj_t *settings = create_button(page, LV_SYMBOL_LIST, 207, 206);
    lv_obj_add_event_cb(settings, button_event_handle, LV_EVENT_CLICKED, (void *)MINI_APP_ID_SETTINGS);

    lv_obj_t *save = create_button(page, LV_SYMBOL_PLAY, 170, 206);
    lv_obj_add_event_cb(save, button_event_handle, LV_EVENT_CLICKED, (void *)MINI_APP_ID_SETTINGS);

    lv_group_set_editing(inst->group, false);
    lv_group_add_obj(inst->group, save);
    lv_group_add_obj(inst->group, settings);

    lv_group_focus_obj(settings);

    printf("group count %ld\n", lv_group_get_obj_count(inst->group));

    mini_app_inst_t *app_inst = (mini_app_inst_t *)inst->user_data;
    app_inst->p_handle = malloc(sizeof(desktop_data_t));
    desktop_data_t *data = (desktop_data_t *)app_inst->p_handle;

    inst->user_data = data;
    data->voltage = label_voltage;
    data->current = label_current;
    data->power = label_power;
    data->ah = label_ah;
    data->wh = label_wh;
    data->time = time;
    data->run_info = run_info;
}

void page_desktop_unLoad(lv_pm_page_inst_t *inst)
{
    printf("desktop unLoad\n");
    mini_app_inst_t *app_inst = (mini_app_inst_t *)inst->user_data;
    desktop_data_t *data = (desktop_data_t *)app_inst->p_handle;
    free(data);
}

const mini_app_t app_desktop_info = {
    .id = MINI_APP_ID_DESKTOP,
    .name = "desktop",
    .hide = true,
    .onLoad = page_desktop_onLoad,
    .unLoad = page_desktop_unLoad,
};