cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW) # Enable version number on project command

project(litesimd VERSION 0.1.1)
if(MSVC)
    add_compile_options(
        "/W4"
        "$<$<CONFIG:RELEASE>:/O2>"
        "$<$<CONFIG:RELEASE>:/Oi>"
        "/MD$<$<CONFIG:Debug>:d>"
    )
else()
    add_compile_options(
        "-Wall"
        "-march=native"
        "-mtune=native"
        "-fno-strict-aliasing"
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_library(${PROJECT_NAME} INTERFACE)
target_compile_definitions(${PROJECT_NAME} INTERFACE LIBRARY_HEADER_ONLY)
target_include_directories(${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

install(EXPORT ${PROJECT_NAME}
    DESTINATION lib/litesimd
    NAMESPACE litesimd::
    FILE litesimdConfig.cmake
)

add_subdirectory(doc)
add_subdirectory(samples)
add_subdirectory(test)
