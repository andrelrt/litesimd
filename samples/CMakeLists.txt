
# Boost is requirement for samples
find_package(Boost 1.56.0 COMPONENTS timer chrono system)

if(Boost_FOUND)
    add_subdirectory(binary_search)
    add_subdirectory(boyer_moore_horspool)
    add_subdirectory(bubble_sort)
    add_subdirectory(greater)
    add_subdirectory(nway_tree)
    add_subdirectory(to_lower)
endif()
