#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// Boot reason code
enum boot_reason_code_code {
    boot_reason_code_update = 0xF001,   /// Restart caused by the update request
    boot_reason_code_recovery, /// Restart caused by the recovery request
    boot_reason_code_factory,  /// Restart caused by the factory reset request
    boot_reason_code_pgm_keys, /// Load keys request (close configuration)
    boot_reason_code_usb_mc_mode, /// Restart caused by the USB MSC request
    boot_reason_code_backup, /// Restart caused by the backup request
    boot_reason_code_restore, /// Restart caused by the restore request
    boot_reason_code_os, /// Restart caused by the os itself
    boot_reason_code_unknown,  //! Unknown boot reason code
};

/** Get the system boot reason code
 * @return @boot_reason_code_code
 */
enum boot_reason_code_code boot_reason(void);

/** Get the system boot reason code encoded as a string
 * @return string describing boot reason
 */
const char *boot_reason_code_str(enum boot_reason_code_code code);

/** Set the system boot reason code
 * @param code @boot_reason_code_code
 */
void set_boot_reason(enum boot_reason_code_code code);

/** Set the status of the OS boot
 * @param status true if the os booted successfully, otherwise false
 */
void set_os_boot_status(bool status);

/** Checks if the OS booted successfully
 * @return true if the os booted successfully, otherwise false
 */
bool get_os_boot_status();

#ifdef __cplusplus
}
#endif