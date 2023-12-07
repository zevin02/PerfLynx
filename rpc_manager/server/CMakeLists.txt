#
# Dependencies
#
# find_package(Threads)

#
# Sources
#
set(SOURCES
    main.cpp
    rpc_manager.cpp
)

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# target_include_directories(server PUBLIC
#   ${PROJECT_SOURCE_DIR}/..
# )
#
# Target
#

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_executable(server ${SOURCES})
target_link_libraries(server
    PUBLIC
    monitor_proto
)
