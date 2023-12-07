#
# Dependencies
# 查找一些包
find_package(Qt5 
  REQUIRED COMPONENTS
  Core
  Widgets
)
# 把需要的原文件全部都放进去
set(SOURCES
    main.cpp
    monitor_inter.cpp
    cpu_softirq_model.cpp
    cpu_load_model.cpp
    monitor_widget.cpp
    cpu_stat_model.cpp
    mem_model.cpp
    net_model.cpp
)

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

#
# Target
#
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
add_executable(display ${SOURCES})

#设置一些目标的属性，automoc所以就无法使用信号
set_target_properties(display PROPERTIES AUTOMOC TRUE)

# 设置一些需要的头文件
target_include_directories(display PUBLIC
  ${PROJECT_SOURCE_DIR}/rpc_manager
)
# 设置需要的一些库
target_link_libraries(display
    PUBLIC
    client
    monitor_proto
    Qt5::Widgets
    Qt5::Core
    # qwt-qt5
)


# INSTALL(TARGETS client
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )
