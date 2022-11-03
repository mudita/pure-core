#include "hal/boot_reason.h"

#include "fsl/fsl_snvs_lp.h"

static const uint16_t os_magic_code = 0xdead;

static uint32_t build_code(const uint16_t code) {
    const uint16_t bootloader_magic_code = 0xbadc;
    return bootloader_magic_code << 16U | code;
}

static enum boot_reason_code_code deconstruct_code(const uint32_t reg) {
    return reg & 0xff;
}

/** Get system boot reason code */
enum boot_reason_code_code boot_reason(void) {
    if (SNVS->LPGPR[0] != 0) {
        return deconstruct_code(SNVS->LPGPR[0]);
    }
    return boot_reason_code_unknown;
}

void set_boot_reason(enum boot_reason_code_code code) {
    build_code(code);
}
