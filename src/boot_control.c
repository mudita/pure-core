// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "boot_control.h"

#include "common/json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    cJSON *bootable;
    cJSON *successful;
    cJSON *boot_attempts_left;
    cJSON *prefix;
} json_slot;

static char file_path[128] = {};

static cJSON *root = NULL;
static cJSON *boot = NULL;
static cJSON *slots = NULL;
static cJSON *active = NULL;
static cJSON *boot_attempts_max = NULL;
static cJSON *os_bin = NULL;
static cJSON *recovery_bin = NULL;
static cJSON *bin_dir = NULL;
static cJSON *update_dir = NULL;
static json_slot slot_a = {};
static json_slot slot_b = {};

static size_t json_string_to_unsigned(const cJSON *json) {
    const char *string = cJSON_GetStringValue(json);
    if (string) {
        return strtoul(string, NULL, 10);
    }
    return 0;
}

static void set_json_string_with_unsigned(cJSON *json, size_t value) {
    static char conversion_buffer[21] = {};
    snprintf(conversion_buffer, sizeof(conversion_buffer), "%lu", (unsigned long) value);
    cJSON_SetValuestring(json, conversion_buffer);
}

static bool is_slot_valid(const json_slot *const slot) {
    return slot->bootable != NULL && slot->prefix != NULL && slot->boot_attempts_left != NULL &&
           slot->successful != NULL;
}

static bool init_slot(const cJSON *slot, json_slot *slot_out) {
    slot_out->successful = json_get_item_from(slot, "successful");
    slot_out->boot_attempts_left = json_get_item_from(slot, "boot_attempts_left");
    slot_out->prefix = json_get_item_from(slot, "prefix");
    slot_out->bootable = json_get_item_from(slot, "bootable");
    return is_slot_valid(slot_out);
}

static json_slot *get_json_slot(slot_t slot) {
    return slot == Slot_A ? &slot_a : &slot_b;
}

static const char *get_slot_str(slot_t slot) {
    return slot == Slot_A ? "a" : "b";
}

static size_t get_reboot_count_max() {
    return json_string_to_unsigned(boot_attempts_max);
}

static int reload() {
    int ret = json_to_file(file_path, root);
    if (ret != 0) {
        return ret;
    }
    //Reload internal structures with the new data
    boot_control_deinit();
    return boot_control_init(file_path);
}


int boot_control_init(const char *boot_file) {
    boot_control_deinit();

    /* Prevent crash due to strings overlap */
    if (boot_file != file_path) {
        strncpy(file_path, boot_file, sizeof(file_path)-1);
    }
    root = json_from_file(file_path);
    if (root == NULL) {
        return -1;
    }
    boot = json_get_item_from(root, "boot");
    if (boot == NULL) {
        return -1;
    }
    slots = json_get_item_from(boot, "slots");
    if (slots == NULL) {
        return -1;
    }
    active = json_get_item_from(slots, "active");
    if (active == NULL) {
        return -1;
    }
    boot_attempts_max = json_get_item_from(slots, "boot_attempts_max");
    if (boot_attempts_max == NULL) {
        return -1;
    }
    os_bin = json_get_item_from(boot, "os");
    if (os_bin == NULL) {
        return -1;
    }
    recovery_bin = json_get_item_from(boot, "recovery");
    if (recovery_bin == NULL) {
        return -1;
    }
    bin_dir = json_get_item_from(boot, "bin_dir");
    if (bin_dir == NULL) {
        return -1;
    }
    update_dir = json_get_item_from(boot, "update_dir");
    if (update_dir == NULL) {
        return -1;
    }
    if (!init_slot(json_get_item_from(slots, "a"), &slot_a) ||
        !init_slot(json_get_item_from(slots, "b"), &slot_b)) {
        return -1;
    }
    return 0;
}

void boot_control_deinit() {
    cJSON_Delete(root);
    root = NULL;
}

size_t get_slots_number() {
    return Slots;
}

slot_t get_current_slot() {
    if (*active->valuestring == 'a') {
        return Slot_A;
    } else {
        return Slot_B;
    }
}

int mark_as_successful() {
    get_json_slot(get_current_slot())->successful->type = cJSON_True;

    return reload();
}

int mark_as_active(slot_t slot) {
    cJSON_SetValuestring(active, get_slot_str(slot));
    set_json_string_with_unsigned(get_json_slot(get_current_slot())->boot_attempts_left, get_reboot_count_max());

    return reload();
}

int mark_as_bootable(slot_t slot) {
    get_json_slot(slot)->bootable->type = cJSON_True;
    return reload();
}

int mark_as_unbootable(slot_t slot) {
    get_json_slot(slot)->bootable->type = cJSON_False;
    get_json_slot(slot)->successful->type = cJSON_False;

    set_json_string_with_unsigned(get_json_slot(get_current_slot())->boot_attempts_left, get_reboot_count_max());
    return reload();
}

bool is_bootable(slot_t slot) {
    return cJSON_IsTrue(get_json_slot(slot)->bootable);
}

bool is_successful(slot_t slot) {
    return cJSON_IsTrue(get_json_slot(slot)->successful);
}

slot_t get_next_active() {
    return get_current_slot() == Slot_A ? Slot_B : Slot_A;
}

size_t get_boot_attempts_left(slot_t slot) {
    return json_string_to_unsigned(get_json_slot(slot)->boot_attempts_left);
}

const char *get_prefix(slot_t slot) {
    return get_json_slot(slot)->prefix->valuestring;
}

const char *get_os() {
    return os_bin->valuestring;
}

const char *get_recovery() {
    return recovery_bin->valuestring;
}

const char *get_binary_dir() {
    return bin_dir->valuestring;
}

int decrease_boot_attempt() {
    size_t value = get_boot_attempts_left(get_current_slot());
    if (value > 0) {
        set_json_string_with_unsigned(get_json_slot(get_current_slot())->boot_attempts_left, --value);
        return reload();
    }
    return 0;
}

const char *get_update_dir() {
    return update_dir->valuestring;
}
