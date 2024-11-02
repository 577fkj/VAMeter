#include "app_settings.h"
#include "mini_app_registry.h"
#include "mini_app_lancher.h"
#include "lvgl.h"
#include "pm.h"

#include "lv_custom_list.h"
#include "lv_custom_font.h"

#include "settings_scene.h"

#define DESKTOP_ID 65535

typedef struct
{
    list_t *list;
} settings_main_page_data_t;

static void button_event_handle(list_t *list, lv_event_t *event, void *user_data, void *user_data_id)
{
    lv_event_code_t code = lv_event_get_code(event);
    uint16_t id = (uint16_t)user_data_id;
    app_settings_data_t *app = (app_settings_data_t *)user_data;
    printf("button_event_handle code %d id %d\n", code, id);
    if (code == LV_EVENT_CLICKED)
    {
        switch (id)
        {
        case SETTINGS_SCENE_VOLTAGE:
            mui_scene_dispatcher_goto_scene(app->dispatcher, SETTINGS_SCENE_VOLTAGE);
            break;

        case SETTINGS_SCENE_CURRENT:
            mui_scene_dispatcher_goto_scene(app->dispatcher, SETTINGS_SCENE_CURRENT);
            break;

        case DESKTOP_ID:
            mini_app_launcher_back(mini_app_launcher());

        default:
            break;
        }
    }
}

void settings_scene_main_on_enter(const lv_pm_page_inst_t *inst, void *user_data)
{
    printf("settings_scene_main_on_enter\n");
    app_settings_data_t *app = (app_settings_data_t *)user_data;
    settings_main_page_data_t *page_data = malloc(sizeof(settings_main_page_data_t));
    app->user_data = page_data;
    lv_obj_t *page = inst->page;

    lv_obj_set_size(page, 240, 240);
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(page, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_group_set_editing(inst->group, false);

    // settings item path top bar
    lv_obj_t *top_bar = lv_obj_create(page);
    lv_obj_set_size(top_bar, 240, 30);
    lv_obj_set_pos(top_bar, 0, 0);
    lv_obj_set_style_bg_color(top_bar, lv_color_hex(0x1C1C1C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(top_bar, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(top_bar, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *label = lv_label_create(top_bar);
    lv_obj_set_size(label, 240, 30);
    lv_obj_set_pos(label, 3, 5);
    lv_label_set_text(label, "设置");
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_siyuan_chinese_bold_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // settings item list
    list_t *list = custom_list_create(page, inst->group);
    page_data->list = list;
    lv_obj_set_size(list->list, 240, 210);
    lv_obj_set_pos(list->list, 0, 30);
    set_list_event_cb(list, button_event_handle);
    set_list_user_data(list, app);

    lv_obj_t *btn = add_list_item(list, LV_SYMBOL_SETTINGS, "电压", SETTINGS_SCENE_VOLTAGE);
    lv_group_focus_obj(btn);

    add_list_item(list, LV_SYMBOL_SETTINGS, "电流", SETTINGS_SCENE_CURRENT);

    add_list_item(list, LV_SYMBOL_CLOSE, "返回", DESKTOP_ID);

    printf("group count %ld\n", lv_group_get_obj_count(inst->group));
}

void settings_scene_main_on_exit(const lv_pm_page_inst_t *inst, void *user_data)
{
    printf("settings_scene_main_on_exit\n");
    app_settings_data_t *app = (app_settings_data_t *)user_data;
    settings_main_page_data_t *page_data = (settings_main_page_data_t *)app->user_data;
    custom_list_deinit(page_data->list);
    free(page_data);
}