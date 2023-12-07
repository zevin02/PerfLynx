#!/usr/bin/env bash

# set -e 命令执行失败，就会退出，不会继续执行下面的命令
set -e

# 因为这些都是在docker中执行的,同时当前项目在docker配置镜像的时候，完全都被复制到了/work中（在docker当前容器中）进入到当前文件的目录中

cd "$(dirname "${BASH_SOURCE[0]}")"

# https://github.com/abseil/abseil-cpp/archive/refs/tags/20200225.2.tar.gz
# Install abseil.
THREAD_NUM=$(nproc)
VERSION="20200225.2"    
PKG_NAME="abseil-cpp-${VERSION}.tar.gz" 

# shell脚本就是把命令行中的各种指令整合在一起,这里的tar是解压缩的命令，x是解压缩的意思，z是指定解压缩的格式，f是指定解压缩的文件 
# 脚本的第一步，先对当前的压缩包进行解压
tar xzf "${PKG_NAME}"

# pushd是相等于一个cd的命令,相当于进入到了abseil-cpp-20200225.2这个目录下面
pushd "abseil-cpp-${VERSION}"
# 进入之后，就创建一个build目录，并且进入到这个目录下面
    # mkdir build && cd build
    # 如果当前目录不存在的话就需要进行创建
    if [ ! -d build ]; then
        mkdir build
    fi
    cd build` 8
    # 运行之前先删除掉cmake缓存
    rm -f CMakeCache.txt
    rm -rf CMakeFiles

    cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_STANDARD=14 -DCMAKE_INSTALL_PREFIX=/usr/local
  
    make -j${THREAD_NUM}                       
    make install                                
    # make install命令会将所有动态库编译后的库文件复制到指定的安装目录中，2.将头文件复制到指定的安装目录中,3.更新系统的动态库缓存，让系统能够快速的正确找到新安装的库文件
# 执行完popd之后，就返回到开始之前的目录了
popd


# ldconfig 这个linux系统命令是用来配置动态链接库的，它会在默认的目录下搜索库文件，并创建链接，以便程序在运行时能够找到所需要的共享库
ldconfig

# shell三剑客
# awk:对文件进行处理，可以处理分隔符分割的文件，也可以处理固定长度的文件，print $1，就可以切割出来第一列的数据
# grep：
# sed

# Clean up，把所有的中间数据全部都删除掉,把数据全部删除掉
rm -rf "abseil-cpp-${VERSION}" "${PKG_NAME}"