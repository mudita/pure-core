add_library(rt1051_priv
        boot_reason.c

        fsl/fsl_common.c
        fsl/fsl_snvs_hp.c
        fsl/fsl_snvs_lp.c
        )

add_library(pure-hal::arch::imx::rt1051 ALIAS rt1051_priv)

target_include_directories(rt1051_priv PRIVATE . fsl CMSIS)
target_link_libraries(rt1051_priv PRIVATE pure::hal)

target_compile_definitions(rt1051_priv PRIVATE
        FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1
        __STARTUP_INITIALIZE_NONCACHEDATA
        CPU_MIMXRT1051DVL6B_cm7
        CPU_MIMXRT1051DVL6B
        __FPU_PRESENT
        )