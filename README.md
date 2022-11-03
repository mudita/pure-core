# pure-core:

## How to use it:

The preferred way of including this library is to use CMake's FetchContents API.

Put the code below somewhere in your project's `CMakeLists.txt`
```
include(FetchContent)

FetchContent_Declare(
        pure-core
        GIT_REPOSITORY git@github.com:mudita/pure-core.git
)
FetchContent_MakeAvailable(pure-core)
```
After doing so, the new target `pure-core` will be available.

Alternatively, you can use GIT's submodules in conjunction with `add_subdirectory`

## API

Available headers/modules are placed under `hal` directory. 

```
hal
├── boot_control.h
└── boot_reason.h
```
To use them, simply add `#include <hal/header_of_choice.h>`

You will also need to link `pure-core` in order for the linker to find the correct symbols
`target_link_libraries(target PRIVATE/PUBLIC pure-core)`

> **_NOTE:_**  In order to use `boot_reason.h` API you need to provide implementation of
> two functions:
> ```
> extern uint32_t boot_reason_get_raw();
> extern void boot_reason_set_raw(uint32_t raw);
> ```
> For instance (use RT1051's SNVS general purpose registers):
> ```
> uint32_t boot_reason_get_raw(){
>   return SNVS->LPGPR[0];
> }
> void boot_reason_set_raw(uint32_t raw){
>   SNVS->LPGPR[0] = raw;
> }
> ```
> Additionally, you need to link module that implements these functions to
> the `pure-core` using `target_link_libraries(pure-core PRIVATE <target_that_implements_functions>)`
> 