
# 引入一些需要的包，使用的是配置文件方式引入
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
# 这个是grpc所需要的库，所以我们也需要找到
find_package(c-ares CONFIG)
# find_package(Threads)

#
# Protobuf/Grpc source files
# 把各种.proto文件放入到PROTO_FILES中
set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
)

#
# Add Library target with protobuf sources
#
# 将proto生成的文件全部都编译成静态库，monitor_proto就是静态库的名称,PROTO_FILES是需要编译的proto源文件的名称集合
add_library(monitor_proto ${PROTO_FILES})
# 将monitor_proto库链接到protobuf和grpc库，链接上这两个库
target_link_libraries(monitor_proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
# 将这个库的头文件目录添加到编译器可以搜索的放为内
target_include_directories(monitor_proto PUBLIC
${PROTOBUF_INCLUDE_DIRS} 
${CMAKE_CURRENT_BINARY_DIR})

#
# Compile protobuf and grpc files in mointor proto target to cpp
#
# 搜索grpc插件的路径，放到 grpc_cpp_plugin_location
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

# 在cmake中集成了protoc的指令到protobuf_generate中
# 第一个指令用于生成c++的protobuf代码，，他会自动查找所有的proto文件，生成对应的.pb.cc,.pb.h文件，
# LANGUAGE cpp表示生成c++的代码
# TARGET 生成的文件将作为哪个target的一部分，这里是monitor_proto
protobuf_generate(TARGET monitor_proto LANGUAGE cpp)
# PLUGIN中指定是grpc插件，
# GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc表示生成的文件的后缀名
# 通过grpc的插件，就会生成在客户端和服务器生成不同的东西
protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
