# PerfLinux

本系统采用C++技术栈，并结合 Docker、gRPC、ProtoBuf等工具和框架的分布式Linux性能分析监控系统。通过模块化的设计，该项目旨在实现对分布式Linux 系统的性能监控和分析。


# 应用技术
C++、Docker、gRPC、protobuf、Cmake、工厂方法。
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

查看镜像id，命名镜像id为linux:monitor

~~~shell
docker images
docker tag xxx linux:monitor
~~~

进入docker容器
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