project(litesimd)
cmake_minimum_required(VERSION 2.8)

find_package(Boost 1.62.0 REQUIRED COMPONENTS timer chrono system)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -Wall -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -Wall -fno-strict-aliasing")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -Wall -fno-strict-aliasing")


add_subdirectory(samples)
add_subdirectory(test)

