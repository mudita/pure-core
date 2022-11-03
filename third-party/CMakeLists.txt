include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
FetchContent_Declare(
        cJSON
        GIT_REPOSITORY https://github.com/DaveGamble/cJSON
        GIT_TAG d348621ca93571343a56862df7de4ff3bc9b5667 # v1.7.15
)
FetchContent_MakeAvailable(cJSON)
FetchContent_GetProperties(cJSON)
target_include_directories(cjson PUBLIC $<BUILD_INTERFACE:${cJSON_SOURCE_DIR}>)
