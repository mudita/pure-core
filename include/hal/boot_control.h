// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    Slot_A,
    Slot_B,
    Slots,
} slot_t;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Perform any initialization tasks.
 * This is called only once.
 */
int boot_control_init(const char *boot_file);

/**
 * Perform deinitialization tasks.
 * This is called only once.
 * Not calling deinit can cause memory leaks.
 */
void boot_control_deinit();

/**
 * Returns the number of available slots.
 * For instance, a system with a single set of partitions would return
 * 1, a system with A/B would return 2, A/B/C -> 3...
 */
size_t get_slots_number();

/**
 * Returns the value letting the system know
 * whether the current slot is A or B. It is assumed that if the current slot
 * is A, then the block devices underlying B can be accessed directly
 * without any risk of corruption.
 */
slot_t get_current_slot();

/**
 * Marks the current slot
 * as having booted successfully.
 * @return 0 if success, otherwise negative error code
 */
int mark_as_successful();

/**
 * Marks the slot passed in parameter as
 * the active boot slot.
 * @return 0 if success, otherwise negative error code
 */
int mark_as_active(slot_t slot);

/**
 * Marks the slot passed in parameter as
 * an bootable.
 * @return 0 if success, otherwise negative error code
 */
int mark_as_bootable(slot_t slot);

/**
 * Marks the slot passed in parameter as
 * an unbootable. This can be used while updating the contents of the slot's
 * partitions, so that the system will not attempt to boot a known bad set up.
 * @return 0 if success, otherwise negative error code
 */
int mark_as_unbootable(slot_t slot);

/**
 * Decreases boot attempt counter of the currently active slot by one.
 * If it is already 0, then such an operation have no effect.
 * @return 0 if success, otherwise negative error code
 */
int decrease_boot_attempt();

/**
 * Returns if the slot passed in parameter is bootable.
 */
bool is_bootable(slot_t slot);

/**
 * Returns if the slot passed in parameter has
 * been marked as successful using mark_as_successful.
 */
bool is_successful(slot_t slot);

/**
 * Returns how many boot attempts left for specified slot.
 */
size_t get_boot_attempts_left(slot_t slot);

/**
 * Returns the active slot to boot into on the next boot.
 */
slot_t get_next_active();

/**
 * Returns the string prefix used by partitions that
 * correspond to the slot number passed in parameter. The returned string
 * is expected to be statically allocated and not need to be freed.
 * Returns NULL if slot does not match an existing slot.
 */
const char *get_prefix(slot_t slot);

/**
 * Returns the os binary name. The returned string
 * is expected to be statically allocated and not need to be freed.
 */
const char *get_os();

/**
 * Returns the recovery binary name. The returned string
 * is expected to be statically allocated and not need to be freed.
 */
const char *get_recovery();

/**
 * Returns the directory that stores binaries(os,recovery). The returned string
 * is expected to be statically allocated and not need to be freed.
 */
const char *get_binary_dir();

/**
 * Returns the directory that stores unpacked update package contents
 */
const char *get_update_dir();

#ifdef __cplusplus
}
#endif