# 设置要编译的可执行文件，g++在编译文件中需要哪些文件
set(SOURCES
   main.cpp
   monitor/cpu_softirq_monitor.cpp
   monitor/cpu_load_monitor.cpp
   monitor/cpu_stat_monitor.cpp
   monitor/mem_monitor.cpp
   monitor/net_monitor.cpp
   utils/read_file.cpp
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
# 把这些文件，生成一个monitor的可执行程序
add_executable(monitor ${SOURCES})

# target_include_directories(monitor PUBLIC
#   ../include/..
# )
# 添加的各种需要的头文件，指定当前的各种包括文件，和一个rpc使用的各种的头文件，这样之后我们就能直接包含各种的头文件的相对路径了
target_include_directories(monitor PUBLIC
  ${PROJECT_SOURCE_DIR}/test_monitor/include
  ${PROJECT_SOURCE_DIR}/rpc_manager
)

# 当前需要哪些的库，这里需要的是rpc_manager的库，和boost的库，同时还有我们上面编译好的proto的库
# 同时这里我们使用了rpc的client的库，我们就可以直接使用rpc的客户端程序代码
target_link_libraries(monitor
    PUBLIC
    monitor_proto
    client
    boost_system
    boost_chrono
)