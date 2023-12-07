# PerfLinux

本系统采用C++技术栈，并结合 Docker、gRPC、ProtoBuf等工具和框架的分布式Linux性能分析监控系统。通过模块化的设计，该项目旨在实现对分布式Linux 系统的性能监控和分析。


# 应用技术
C++、Docker、gRPC、protobuf、Cmake、工厂方法。
# 特点

- `Docker`模块:使用dockerfile指定相应的`CMake`，`gRPC`，`protobuf`等源码和依赖，构建整个项目环境，方便在多个服务器上部署环境，并编写容器操作项目所依赖的环境
- `Monitor`模块：使用工厂模式，构造 Monitor 抽象类定义接口，后实现相应的CPU状态，系统负载，软中断，内存，网络监控，方便为之后扩展更多的系统监控，并为了模拟出真实的性能问题，使用`stress`工具进行模拟压测，分析相应时刻服务器的CPU状态和中断状态
- 通过`gRPC`框架，构建出相应的server，client。server在所要监控的服务器部署，client生成库供Monitor模块和display模块使用，并考虑为了降低耦合性，项目中的每个模块相互独立，可拆解，只通过调用grpc进行远程连接，分布式的核心实现
- 使用`protobuf`序列化协议，构建出整个项目的数据结构
- `Display`模块中使用qt，每3s刷新一次
- 相比top命令的优势是信息更加全面(包括CPU中的每个核的信息)，能够更好的发现问题，并进行性能分析
- 多处使用`智能指针`，避免内存泄漏，以及`条件变量`和`锁`保证并发安全
- 分布式：可以在当前机器上同时监控多个不同主机的系统状况



## 环境配置
**安装Docker**
~~~shell
sudo apt install curl
curl -fsSL https://test.docker.com -o test-docker.sh
sudo sh test-docker.sh
~~~

**docker加入用户组**
~~~shell
sudo groupadd docker
sudo usermod -aG docker ${USER}
sudo systemctl restart docker
newgrp docker
docker ps
~~~
https://download.gt.io/archive/gt/5.12/5.12.9/gt-opensource-linux-x64-5.12.9.run
将下载好的qt文件拷贝到`docker/build/install/qt`路径下

**构建项目镜像**
~~~shell
cd /docker/build
docker build --network host -f base.dockerfile .
~~~

**查看镜像id，命名镜像id为linux:monitor**

~~~shell
docker images
docker tag xxx linux:monitor
~~~

**进入docker容器**
~~~shell
cd docker/scripts
# 启动容器
./monitor_docker_run.sh
# 进入容器
./monitor_docker_into.sh
~~~


**编译代码**
~~~sh
cd work/cmake
cmake ..
make
~~~


## 启动客户端和服务端
**进入容器**
~~~sh
# 启动容器
./monitor_docker_run.sh
# 进入容器
./monitor_docker_into.sh
~~~

启动服务端
~~~sh
cd work/cmake/rpc_manager/server
./server
cd work/cmake/test_monitor/src
./monitor
~~~
启动客户端

~~~sh
cd work/cmake/display_monitor
./display
~~~