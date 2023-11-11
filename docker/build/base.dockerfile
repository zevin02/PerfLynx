FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive

ENV TZ=Asia/Shanghai

SHELL ["/bin/bash","-c"]

RUN apt-get clean && \
    apt-get autoclean


COPY apt/sources.list /etc/apt/


RUN apt-get update  && apt-get upgrade -y  && \
    apt-get install -y \
    htop \
    apt-utils \
    curl \
    cmake \
    git \
    openssh-server \
    build-essential \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libboost-all-dev \
    net-tools \
    vim \
    stress \
    gdb


# 设置环境变量支持utf-8
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

RUN apt-get install -y libc-ares-dev  libssl-dev gcc g++ make 

RUN apt-get install -y  \
    libx11-xcb1 \
    libfreetype6 \
    libdbus-1-3 \
    libfontconfig1 \
    libxkbcommon0   \
    libxkbcommon-x11-0


RUN apt-get install -y python-dev \
    python3-dev \
    python-pip \
    python-all-dev 

# 遇到的问题，只想装grpc和abseil库的时候，发现到protobuf还没有装，所以还需要先装protobuf，才能顺利安装grpc的库
# 把我们自己的proto拷贝到docker的/tmp下面，
COPY install/protobuf /tmp/install/protobuf

# 再执行我们自己的脚本
RUN /tmp/install/protobuf/install_protobuf.sh

# 这些都是在当前docker构建的时候创建的脚本，同时在里面脚本中也是写的是docker里面的路径
COPY install/abseil /tmp/install/abseil
RUN /tmp/install/abseil/install_abseil.sh

# 需要先安装protobuf和abseil，才能装grpc，因为grpc依赖于前面的两个库,才能够下载成功
COPY install/grpc /tmp/install/grpc
RUN /tmp/install/grpc/install_grpc.sh

COPY install/cmake /tmp/install/cmake
RUN /tmp/install/cmake/install_cmake.sh

RUN apt-get install -y python3-pip
# 指定pip的源，这里是阿里云的源
RUN pip3 install cuteci -i https://mirrors.aliyun.com/pypi/simple

# 我们需要先安装cuteci，才能安装qt
COPY install/qt /tmp/install/qt
RUN /tmp/install/qt/install_qt.sh