#ifndef MINI_APP_REGISTRY_H
#define MINI_APP_REGISTRY_H

#include "mini_app_defines.h"

typedef enum
{
    MINI_APP_ID_DESKTOP = 0,
    MINI_APP_ID_SETTINGS,
} mini_app_id_t;

const mini_app_t *mini_app_registry_find_by_id(uint32_t id);
uint32_t mini_app_registry_get_app_num();
const mini_app_t *mini_app_registry_find_by_index(uint32_t idx);

#endif