include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 5df88da16e276f853cc0c45f4b570419be77dd43 # v3.1.1
)
FetchContent_MakeAvailable(catch2)


project(pure-core-tests)
add_executable(pure-core-tests boot_control.cpp)

target_compile_features(pure-core-tests PRIVATE cxx_std_17)
target_link_libraries(pure-core-tests PRIVATE pure-core Catch2::Catch2WithMain)

add_test(pure-core-tests pure-core-tests)

