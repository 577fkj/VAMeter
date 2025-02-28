#include "mini_app_defines.h"
#include "app_desktop.h"
#include "app_settings.h"

#include <stddef.h>

const mini_app_t *mini_app_registry[] = {
    &app_desktop_info,
    &app_settings_info,
};

const uint32_t mini_app_num = sizeof(mini_app_registry) / sizeof(mini_app_registry[0]);
