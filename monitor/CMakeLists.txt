#
# Dependencies
#
# find_package(Threads)

#
# Sources
#

# cmake_minimum_required(VERSION 3.10)
# project(monitor)
# 因为上面的cmake就是最顶层的cmake，所以在这个地方需要有一个sub，遇到后就需要进行进入递归的调用当前的文件
add_subdirectory(src)