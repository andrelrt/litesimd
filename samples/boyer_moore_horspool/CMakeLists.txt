project(boyer_moore_horspool)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME}
	${SRC_LIST}
)

target_include_directories(${PROJECT_NAME}
	SYSTEM PUBLIC
    ${Boost_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
