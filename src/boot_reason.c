#include "hal/boot_reason.h"
#include <stdint.h>

extern uint32_t boot_reason_get_raw();

extern void boot_reason_set_raw(uint32_t raw);

static const uint16_t os_dead = 0xDEAD;
static const uint16_t os_alive = 0xF00D;

static void update_boot_reason_code(const uint16_t code) {
    uint32_t current = boot_reason_get_raw();
    current &= ~(0xFFFF);
    boot_reason_set_raw(current | code);
}

static void update_os_status_code(const uint16_t code) {
    uint32_t current = boot_reason_get_raw();
    current &= ~(0xFFFF0000);
    boot_reason_set_raw(current | code << 16U);
}

static enum boot_reason_code deconstruct_boot_reason_code(const uint32_t reg) {
    const uint16_t value = reg & 0xFFFF;
    if (value >= boot_reason_code_unknown || value < boot_reason_code_update) {
        return boot_reason_code_unknown;
    }
    return (enum boot_reason_code) (value);
}

static uint16_t deconstruct_os_status_code(const uint32_t reg) {
    const uint16_t status = (boot_reason_get_raw() & 0xFFFF0000) >> 16U;
    return status;
}

enum boot_reason_code boot_reason(void) {
    return deconstruct_boot_reason_code(boot_reason_get_raw());
}

void set_boot_reason(enum boot_reason_code code) {
    update_boot_reason_code(code);
}

const char *boot_reason_code_str(enum boot_reason_code code) {
    switch (code) {
        case boot_reason_code_update:
            return "update";
        case boot_reason_code_recovery:
            return "recovery";
        case boot_reason_code_factory:
            return "factory";
        case boot_reason_code_unknown:
            return "unknown";
        case boot_reason_code_pgm_keys:
            return "pgm_keys";
        case boot_reason_code_backup:
            return "backup";
        case boot_reason_code_restore:
            return "restore";
        case boot_reason_code_usb_mc_mode:
            return "usb_mc_mode";
        case boot_reason_code_os:
            return "code_os";
        default:
            return "not in enum boot_reason_code";
    }
}

void set_os_boot_status(bool state) {
    state ? update_os_status_code(os_alive) : update_os_status_code(os_dead);
}

bool get_os_boot_status() {
    const uint16_t status = deconstruct_os_status_code(boot_reason_get_raw());
    return status == os_alive;
}
