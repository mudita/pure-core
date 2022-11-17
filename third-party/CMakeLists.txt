include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(ENABLE_CJSON_UNINSTALL OFF CACHE INTERNAL "")
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "")
FetchContent_Declare(
        cJSON
        GIT_REPOSITORY git@github.com:mudita/cJSON.git
)
FetchContent_MakeAvailable(cJSON)
FetchContent_GetProperties(cJSON)
target_include_directories(cjson PUBLIC $<BUILD_INTERFACE:${cJSON_SOURCE_DIR}>)
