project(litesimd)
cmake_minimum_required(VERSION 3.5)

find_package(Boost 1.56.0 REQUIRED COMPONENTS timer chrono system)

if(MSVC)
    add_compile_options(
        "/W4"
        "$<$<CONFIG:RELEASE>:/O2>"
        "$<$<CONFIG:RELEASE>:/Oi>"
        "/arch:AVX2"
        "$<IF:$<BOOL:${APPVEYOR}>,/MD,/MT>$<$<CONFIG:Debug>:d>"
    )
else()
    add_compile_options(
        "-Wall"
        "-Wextra"
        "-Werror"
        "-march=native"
        "-mtune=native"
        "-fno-strict-aliasing"
    )
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

enable_testing()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

add_subdirectory(doc)
add_subdirectory(samples)
add_subdirectory(test)

