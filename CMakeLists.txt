cmake_minimum_required(VERSION 3.10.2)
project(test_monitor LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

# sub是最外层的目录，因为是递归的进行调用，所以遇到sub，就会进入递归的调用当前的cmake文件
add_subdirectory(rpc_manager)
add_subdirectory(test_monitor)
add_subdirectory(proto)
add_subdirectory(display_monitor)