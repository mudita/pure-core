// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <catch2/catch_test_macros.hpp>
#include <hal/boot_reason.h>

#include <cstring>

static uint32_t raw_register = 0;

extern "C" {
uint32_t boot_reason_get_raw() {
    return raw_register;
}

void boot_reason_set_raw(uint32_t raw) {
    raw_register = raw;
}
}

TEST_CASE("Boot reason module") {
    REQUIRE_FALSE(get_os_boot_status());
    set_os_boot_status(true);
    REQUIRE(get_os_boot_status());

    REQUIRE(boot_reason() == boot_reason_code_unknown);

    set_boot_reason(boot_reason_code_update);
    REQUIRE(boot_reason() == boot_reason_code_update);
    REQUIRE(get_os_boot_status());

    set_boot_reason(boot_reason_code_os);
    REQUIRE(boot_reason() == boot_reason_code_os);
    REQUIRE(get_os_boot_status());

    set_os_boot_status(false);
    REQUIRE_FALSE(get_os_boot_status());
    REQUIRE(boot_reason() == boot_reason_code_os);
}