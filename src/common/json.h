#pragma once

#include <cJSON.h>

#ifdef __cplusplus
extern "C"
{
#endif

cJSON *json_from_file(const char *path);

int json_to_file(const char *path, const cJSON *json);

cJSON *json_get_item_from(const cJSON *json, const char *name);

#ifdef __cplusplus
}
#endif
