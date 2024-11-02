#include "task.h"
#include "main.h"
#include "ina228.h"
#include "app_desktop.h"
#include "mini_app_defines.h"
#include "mini_app_registry.h"
#include "esp_timer.h"
#include "config.h"

const char *get_format_string(float value)
{
    if (value < 0)
    {
        value = -value; // 处理负数
    }

    if (value < 10)
    {
        return "%.5f"; // 小于10，使用0.00000格式
    }
    else if (value < 100)
    {
        return "%.4f"; // 大于10小于100，使用00.0000格式
    }
    else if (value < 1000)
    {
        return "%.3f"; // 大于100小于1000，使用000.000格式
    }
    else
    {
        return "%.2f"; // 大于1000，使用0000.00格式
    }
}

char voltage_str[9];
char current_str[9];
char power_str[9];
char charge_str[9];
char wh_str[9];

double charge = 0; // Ah
double energy = 0; // Wh

void task_receive(void *arg)
{
    mini_app_inst_t *inst = (mini_app_inst_t *)arg;
    desktop_data_t *data = (desktop_data_t *)inst->p_handle;
    config_t *config = get_config();

    float voltage = ina228_voltage(&ina228_conf);
    float current = ina228_current(&ina228_conf);

    if (current < 0.0)
    {
        current = -current; // 处理负数
    }

    if (voltage < 0.0)
    {
        return;
    }

    if (voltage >= 0.05)
    {
        voltage += (config->voltage_offset * 0.001);
    }

    voltage = voltage / (config->down_resistor / (config->up_resistor + config->down_resistor));

    double power = voltage * current; // 计算功率，单位是W

    if (power < 0.00001) // 低于0.00001W的功率不计算
    {
        current = 0;
        power = 0;
    }
    else
    {
        charge += (current * 0.2) / 3600; // 累加电量，单位是Ah
        energy += (power * 0.2) / 3600;   // 累加能量，单位是Wh
    }

    sprintf(voltage_str, get_format_string(voltage), voltage);
    sprintf(current_str, get_format_string(current), current);
    sprintf(power_str, get_format_string(power), power);
    sprintf(charge_str, get_format_string(charge), charge);

    sprintf(wh_str, get_format_string(energy), energy);

    if (lv_obj_is_visible(data->voltage))
    {
        lv_label_set_text_static(data->voltage, voltage_str);
        lv_label_set_text_static(data->current, current_str);
        lv_label_set_text_static(data->power, power_str);
        lv_label_set_text_static(data->ah, charge_str);
        lv_label_set_text_static(data->wh, wh_str);
    }
}

#define US2MS(us) ((us) / 1000)
#define MS2US(ms) ((ms) * 1000)

void init_ina228_task(mini_app_inst_t *inst)
{
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &task_receive,
        .arg = inst,
        .name = "ina228_task",
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, MS2US(200))); // 200ms
}