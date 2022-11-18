#include "json.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#ifndef debug_log
#define debug_log(...)
#endif

static void autoclose(FILE **f) {
    if (*f != NULL) {
        fclose(*f);
    }
}

static void autofree(void **ptr) {
    free(*ptr);
}

#define AUTOCLOSE(var) FILE* var __attribute__((__cleanup__(autoclose)))
#define AUTOFREE(var) void* var __attribute__((__cleanup__(autofree)))

static size_t get_file_size(FILE *fd) {
    fseek(fd, 0, SEEK_END);
    const size_t file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    return file_size;
}


cJSON *json_from_file(const char *path) {
    AUTOCLOSE(fd) = fopen(path, "r");
    if (fd == NULL) {
        debug_log("JSON: failed to open path: %s", path);
        return NULL;
    }

    const size_t file_size = get_file_size(fd);

    AUTOFREE(buffer) = calloc(1, file_size + 1);
    size_t bytes_read = fread(buffer, 1, file_size, fd);
    if (bytes_read != file_size) {
        debug_log("JSON: failed to read data from file %s", path);
        return NULL;
    }

    cJSON *ret = cJSON_Parse(buffer);
    if (ret == NULL) {
        if (cJSON_GetErrorPtr() != NULL) {
            debug_log("JSON: parsing failed: %s", err);
        }
    }
    return ret;
}

cJSON *json_get_item_from(const cJSON *json, const char *name) {
    cJSON *object = NULL;

    if (json != NULL && name != NULL) {
        object = cJSON_GetObjectItemCaseSensitive(json, name);
    } else {
        debug_log("JSON: Failed to get object '%s'", name);
    }

    return object;
}

int json_to_file(const char *path, const cJSON *json) {
    AUTOCLOSE(fd) = fopen(path, "w");
    if (fd == NULL) {
        debug_log("JSON: failed to open path: %s", path);
        return -1;
    }

    char *contents = cJSON_Print(json);
    if (contents == NULL) {
        debug_log("JSON: failed to generate json string: %s", path);
        return -1;
    }

    const size_t contents_len = strlen(contents);
    const size_t written = fwrite(contents, 1, contents_len, fd);
    cJSON_free(contents);
    if (contents_len != written) {
        debug_log("JSON: failed to write data to file %s", path);
        return -1;
    }
    return 0;
}
