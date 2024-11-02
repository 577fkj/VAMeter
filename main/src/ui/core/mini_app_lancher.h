#ifndef MINI_APP_LAUNCHER_H
#define MINI_APP_LAUNCHER_H

#include <stdint.h>
#include "m-dict.h"
#include "pm.h"

#include "mini_app_defines.h"

DICT_DEF2(mui_app_inst_dict, uint32_t, M_DEFAULT_OPLIST, mini_app_inst_t *, M_PTR_OPLIST)

typedef struct
{
    lv_pm_instance_t *pm_instance;
    mui_app_inst_dict_t app_inst_dict;
} mini_app_launcher_t;

mini_app_launcher_t *mini_app_launcher();
void mini_app_launcher_init(mini_app_launcher_t *p_launcher);
void mini_app_launcher_run(mini_app_launcher_t *p_launcher, uint32_t id);
void mini_app_launcher_back(mini_app_launcher_t *p_launcher);
mini_app_inst_t *mini_app_get_inst(mini_app_launcher_t *p_launcher, uint32_t id);

mini_app_inst_t *mini_app_get_inst_by_page(lv_pm_page_inst_t *inst);

#endif