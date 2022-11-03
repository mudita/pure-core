// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <catch2/catch_test_macros.hpp>
#include <hal/boot_control.h>

#include <cstring>

void create_boot_json() {
    constexpr auto json_contents = "{\n"
                                   "  \"boot\": {\n"
                                   "    \"slots\": {\n"
                                   "      \"a\": {\n"
                                   "        \"bootable\": true,\n"
                                   "        \"successful\": true,\n"
                                   "        \"boot_attempts_left\": 1,\n"
                                   "        \"prefix\" : \"/system_a\"\n"
                                   "      },\n"
                                   "      \"b\": {\n"
                                   "        \"bootable\": true,\n"
                                   "        \"successful\": true,\n"
                                   "        \"boot_attempts_left\": 10,\n"
                                   "        \"prefix\" : \"/system_b\"\n"
                                   "      },\n"
                                   "      \"active\": \"a\",\n"
                                   "      \"boot_attempts_max\": 10\n"
                                   "    },\n"
                                   "    \"os\": \"boot.bin\",\n"
                                   "    \"recovery\": \"recovery.bin\",\n"
                                   "    \"bin_dir\": \"bin\"\n"
                                   "  }\n"
                                   "}";

    const auto fd = fopen("boot.json", "w");
    fwrite(json_contents, 1, strlen(json_contents), fd);
    fclose(fd);
}

TEST_CASE("Boot control module") {
    create_boot_json();
    REQUIRE(boot_control_init("boot.json") == 0);

    REQUIRE(std::string{get_os()} == "boot.bin");
    REQUIRE(std::string{get_recovery()} == "recovery.bin");
    REQUIRE(std::string{get_binary_dir()} == "bin");
    REQUIRE(std::string{get_suffix(Slot_A)} == "/system_a");
    REQUIRE(std::string{get_suffix(Slot_B)} == "/system_b");
    REQUIRE(get_boot_attempts_left(Slot_A) == 1);
    REQUIRE(get_boot_attempts_left(Slot_B) == 10);
    REQUIRE(get_active() == Slot_B);
    REQUIRE(get_current_slot() == Slot_A);
    REQUIRE(get_slots_number() == 2);
    REQUIRE(is_bootable(Slot_A));
    REQUIRE(is_bootable(Slot_B));
    REQUIRE(is_successful(Slot_A));
    REQUIRE(is_successful(Slot_B));

    SECTION("modify") {
        REQUIRE(mark_as_unbootable(Slot_A) == 0);
        REQUIRE_FALSE(is_bootable(Slot_A));
        REQUIRE_FALSE(is_successful(Slot_A));

        REQUIRE(mark_as_active(Slot_B) == 0);
        REQUIRE(get_current_slot() == Slot_B);
        REQUIRE(get_boot_attempts_left(Slot_B) == 10);

        REQUIRE(mark_as_successful() == 0);
        REQUIRE(is_successful(Slot_B));
    }

    SECTION("boot attempts"){
        /// slot A
        REQUIRE(decrease_boot_attempt() == 0);
        REQUIRE(get_boot_attempts_left(Slot_A) == 0);
        REQUIRE(decrease_boot_attempt() == 0);
        REQUIRE(get_boot_attempts_left(Slot_A) == 0);

        /// slot B
        REQUIRE(mark_as_active(Slot_B) == 0);
        REQUIRE(decrease_boot_attempt() == 0);
        REQUIRE(get_boot_attempts_left(Slot_B) == 9);
    }
}