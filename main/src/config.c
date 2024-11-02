#include "config.h"

#include "nvs_flash.h"

#include <string.h>
#include <math.h>

void load_config(config_t *config)
{
    // 打开NVS命名空间
    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvsHandle);
    if (err != ESP_OK)
    {
        printf("Error opening NVS namespace!\n");
        return;
    }

    // 从NVS中读取二进制数据，并将其复制到结构体中
    size_t dataSize = sizeof(config_t);
    err = nvs_get_blob(nvsHandle, "config", config, &dataSize);
    if (err != ESP_OK)
    {
        printf("Error reading config from NVS!\n");
    }

    // 关闭NVS命名空间
    nvs_close(nvsHandle);
}

inline void check_float(float *value, float default_value)
{
    if (isinf(*value) || isnan(*value))
    {
        *value = default_value;
    }
}

void save_config(config_t *config)
{
    // 打开NVS命名空间
    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        printf("Error opening NVS namespace!\n");
        return;
    }

    // 将结构体作为二进制数据存储在NVS中
    err = nvs_set_blob(nvsHandle, "config", config, sizeof(config_t));
    if (err != ESP_OK)
    {
        printf("Error storing config in NVS!\n");
    }

    // 提交更改并关闭NVS命名空间
    err = nvs_commit(nvsHandle);
    if (err != ESP_OK)
    {
        printf("Error committing NVS!\n");
    }
    nvs_close(nvsHandle);

    printf("Save config to NVS!\n");
}

void config_init()
{
    // Initialize NVS.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    config_t *config = get_config();
    load_config(config);

    if (config->magic != 0x01)
    {
        memset(config, 0, sizeof(config_t));
        config->magic = 0x01;

        config->up_resistor = 7.5;
        config->down_resistor = 18.0;
        config->voltage_offset = 900.0;
        config->rshunt = 3.0;
        config->max_current = 50.0;

        save_config(config);
    }

    check_float(&config->up_resistor, 7.5);
    check_float(&config->down_resistor, 18.0);
    check_float(&config->voltage_offset, 900.0);
    check_float(&config->rshunt, 3.0);
    check_float(&config->max_current, 50.0);
}

config_t *get_config()
{
    static config_t config;
    return &config;
}