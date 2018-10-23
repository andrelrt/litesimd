project(litesimd)
cmake_minimum_required(VERSION 3.5)

find_package(Boost 1.56.0 REQUIRED COMPONENTS timer chrono system)

if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>" "$<$<CONFIG:RELEASE>:/Oi>" "/arch:AVX2" "$<IF:$<CONFIG:RELEASE>,/MT,/MTd>")
else()
    add_compile_options("-Wall" "-Wextra" "-Werror" "-march=native" "-mtune=native" "-fno-strict-aliasing")
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

enable_testing()

add_subdirectory(doc)
add_subdirectory(samples)
add_subdirectory(test)

