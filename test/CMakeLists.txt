project(litesimd_test)
cmake_minimum_required(VERSION 2.8)

find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
#add_library(libgmock IMPORTED STATIC GLOBAL)
#add_dependencies(libgmock gtest)
#
## Set libgmock properties
#set_target_properties(libgmock PROPERTIES
#    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
#    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#)

cmake_policy(SET CMP0054 NEW)
include(GoogleTest)

# Test project
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME}
	${SRC_LIST}
)

target_include_directories(${PROJECT_NAME}
	SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    "${source_dir}/googletest/include"
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
    libgtest
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

if(CMAKE_VERSION VERSION_LESS "3.10")
    gtest_add_tests(TARGET ${PROJECT_NAME})
else()
    gtest_discover_tests(${PROJECT_NAME} TEST_PREFIX test)
endif()
