#ifndef __SETTINGS_PAGE_H__
#define __SETTINGS_PAGE_H__

#include "mui_scene_dispatcher.h"

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) SETTINGS_SCENE_##id,
typedef enum
{
#include "settings_scene_config.h"
    SETTINGS_SCENE_MAX,
} settings_scene_id_t;
#undef ADD_SCENE

extern const mui_scene_t settings_scene_defines[];


#endif // __SETTINGS_PAGE_H__