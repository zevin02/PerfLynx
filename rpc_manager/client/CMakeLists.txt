#
# Dependencies
#
# find_package(Threads)

#
# Sources
#
set(SOURCES
    # main.cpp
    rpc_client.cpp
)

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
# add_executable(client ${SOURCES})
# 客户端是生成一个库，调用当前的这个可执行程序
add_library(client  ${SOURCES})
target_link_libraries(client
    PUBLIC
    monitor_proto

)


# INSTALL(TARGETS client
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )
