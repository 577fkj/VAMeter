#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>

typedef struct
{
    uint8_t magic; // 0x57

    float up_resistor;
    float down_resistor;
    float voltage_offset;

    float rshunt;
    float max_current;
} config_t;

void config_init();
config_t *get_config();
void save_config(config_t *config);

#endif // __CONFIG_H