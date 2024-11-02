#include "app_settings.h"
#include "mini_app_registry.h"
#include "mini_app_lancher.h"
#include "lvgl.h"
#include "pm.h"

#include "lv_custom_list.h"
#include "lv_custom_font.h"
#include "lv_edittext.h"
#include "config.h"

static void button_event_handle(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    app_settings_data_t *app = (app_settings_data_t *)lv_event_get_user_data(event);
    if (code == LV_EVENT_CLICKED)
    {
        mui_scene_dispatcher_previous_scene(app->dispatcher);
    }
}

typedef enum
{
    SETTINGS_VOLTAGE_OFFSET,
    SETTINGS_VOLTAGE_UP_RESISTANCE,
    SETTINGS_VOLTAGE_DOWN_RESISTANCE
} settings_voltage_cb_id_t;

typedef struct
{
    lv_edittext_t *voltage_offset;
    lv_edittext_t *up_resistance;
    lv_edittext_t *down_resistance;
} settings_voltage_page_data_t;

void settings_scene_voltage_text_edit_cb(lv_edittext_t *edittext, lv_edittext_event_t event, const char *text)
{
    if (event == LV_EDITTEXT_EVENT_APPLY)
    {
        printf("settings_scene_voltage_text_edit_cb: %s\n", text);
        config_t *config = get_config();
        app_settings_data_t *app = (app_settings_data_t *)lv_edittext_get_user_data(edittext);
        settings_voltage_page_data_t *page_data = (settings_voltage_page_data_t *)app->user_data;
        switch (lv_edittext_get_id(edittext))
        {
        case SETTINGS_VOLTAGE_OFFSET:
            config->voltage_offset = atof(text);
            break;

        case SETTINGS_VOLTAGE_UP_RESISTANCE:
            config->up_resistor = atof(text);
            break;

        case SETTINGS_VOLTAGE_DOWN_RESISTANCE:
            config->down_resistor = atof(text);
            break;

        default:
            break;
        }
        save_config(config);
    }
}

void settings_scene_voltage_on_enter(const lv_pm_page_inst_t *inst, void *user_data)
{
    printf("settings_scene_voltage_on_enter\n");
    app_settings_data_t *app = (app_settings_data_t *)user_data;
    settings_voltage_page_data_t *page_data = (settings_voltage_page_data_t *)malloc(sizeof(settings_voltage_page_data_t));
    app->user_data = page_data;
    lv_obj_t *page = inst->page;

    lv_obj_set_size(page, 240, 240);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(page, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_group_set_editing(inst->group, false);

    lv_obj_t *kb = lv_keyboard_create(page);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_NUMBER, kb_map_num, kb_ctrl_num_map);

    lv_obj_t *cont = lv_obj_create(page);
    lv_obj_set_pos(cont, 0, 0);
    lv_obj_set_size(cont, 240, 240);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cont, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cont, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    config_t *config = get_config();

    char buf[32];

    sprintf(buf, "%f", config->voltage_offset);
    page_data->voltage_offset = lv_edittext_create(cont, kb, "电压(mV)\n补偿", buf);
    lv_edittext_set_group(page_data->voltage_offset, inst->group);
    lv_edittext_set_callback(page_data->voltage_offset, settings_scene_voltage_text_edit_cb);
    lv_edittext_set_placeholder_text(page_data->voltage_offset, "900");
    lv_edittext_set_id(page_data->voltage_offset, SETTINGS_VOLTAGE_OFFSET);

    sprintf(buf, "%f", config->up_resistor);
    page_data->up_resistance = lv_edittext_create(cont, kb, "上拉(K)\n电阻", buf);
    lv_edittext_set_group(page_data->up_resistance, inst->group);
    lv_edittext_set_callback(page_data->up_resistance, settings_scene_voltage_text_edit_cb);
    lv_edittext_set_placeholder_text(page_data->up_resistance, "10");
    lv_edittext_set_id(page_data->up_resistance, SETTINGS_VOLTAGE_UP_RESISTANCE);

    sprintf(buf, "%f", config->down_resistor);
    page_data->down_resistance = lv_edittext_create(cont, kb, "下拉(K)\n电阻", buf);
    lv_edittext_set_group(page_data->down_resistance, inst->group);
    lv_edittext_set_callback(page_data->down_resistance, settings_scene_voltage_text_edit_cb);
    lv_edittext_set_placeholder_text(page_data->down_resistance, "10");
    lv_edittext_set_id(page_data->down_resistance, SETTINGS_VOLTAGE_DOWN_RESISTANCE);

    // Write codes main_btn_setting
    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "返回");
    lv_label_set_long_mode(btn_label, LV_PART_MAIN | LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(btn_label, &lv_font_siyuan_chinese_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(btn, LV_PCT(90), 32);

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

    lv_obj_add_event_cb(btn, button_event_handle, LV_EVENT_CLICKED, app);

    lv_group_add_obj(inst->group, btn);
    lv_group_add_obj(inst->group, kb);
    lv_group_focus_obj(btn);

    printf("group count %ld\n", lv_group_get_obj_count(inst->group));
    printf("inst->group = %p\n", inst->group);
}

void settings_scene_voltage_on_exit(const lv_pm_page_inst_t *inst, void *user_data)
{
    printf("settings_scene_voltage_on_exit\n");
    app_settings_data_t *app = (app_settings_data_t *)user_data;
    settings_voltage_page_data_t *page_data = (settings_voltage_page_data_t *)app->user_data;
    lv_edittext_destroy(page_data->up_resistance);
    lv_edittext_destroy(page_data->down_resistance);
    free(page_data);
}