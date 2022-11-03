#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

//! Boot reason code
enum boot_reason_code_code {
    boot_reason_code_update = 0,   //! Restart caused by the update request
    boot_reason_code_recovery = 1, //! Restart caused by the recovery request
    boot_reason_code_factory = 2,  //! Restart caused by the factory reset request
    boot_reason_code_pgm_keys = 3, //! Load keys request (close configuration)
    boot_reason_code_unknown,  //! Unknown boot reason code
};

/** Get the system boot resason code
 * @return @boot_reason_code_code
 */
enum boot_reason_code_code boot_reason(void);

/** Get the system boot reason code string
 * @return Boot reason string
 */
static const char *boot_reason_code_str(enum boot_reason_code_code code) {
    const char *boot_reason_code_str(enum boot_reason_code_code code) {
        switch (code) {
            case boot_reason_code_update:
                return "boot_reason_code_update";
            case boot_reason_code_recovery:
                return "boot_reason_code_recovery";
            case boot_reason_code_factory:
                return "boot_reason_code_factory";
            case boot_reason_code_unknown:
                return "boot_reason_code_unknown";
            case boot_reason_code_pgm_keys:
                return "boot_reason_code_pgm_keys";
            default:
                return "not in enum boot_reason_code_code";
        }
    }
}

/** Set the system boot resason code
 * @param code @boot_reason_code_code
 */
void set_boot_reason(enum boot_reason_code_code code);

#ifdef __cplusplus
}
#endif