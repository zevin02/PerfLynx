#!/usr/bin/env bash

# grpc是一个现代开源高性能的远程调用框架，可以在任何地方运行，grpc可以使得客户端和服务端之间的通信变得更加简单

set -e

cd "$(dirname "${BASH_SOURCE[0]}")"
# THREAD_NUM=$(grep -c ^processor /proc/cpuinfo) # 这个是获取cpu的核数
THREAD_NUM=$(nproc) # nproc是获取cpu的核数

# https://github.com/grpc/grpc/archive/refs/tags/v1.30.0.tar.gz
# Install grpc.
VERSION="1.30.0"    # 
PKG_NAME="grpc-${VERSION}.tar.gz"

tar xzf "${PKG_NAME}"
# usr/local目录中是用来安装第三方软件的目录，第三方软件区别于系统自带的软件，/usr-由系统安装的软件，/usr/local-由用户安装的软件，把文件放在这里之后，系统就可以自动找到这些文件了
# 我们可以看到，这下面安装的，全部都是在/usr/local目录下面，这个目录下面的文件，都是用户自己安装的，而不是系统自带的
pushd grpc-${VERSION}
    # mkdir build && cd build
    if [ ! -d build ]; then
    mkdir build
    fi
    cd build
    cmake ..    -DgRPC_INSTALL=ON                \
                -DBUILD_SHARED_LIBS=ON           \
                -DCMAKE_BUILD_TYPE=Release       \
                -DgRPC_ABSL_PROVIDER=package     \
                -DgRPC_CARES_PROVIDER=package    \
                -DgRPC_PROTOBUF_PROVIDER=package \
                -DgRPC_RE2_PROVIDER=package      \
                -DgRPC_SSL_PROVIDER=package      \
                -DgRPC_ZLIB_PROVIDER=package     \
                -DCMAKE_INSTALL_PREFIX=/usr/local/
    make -j${THREAD_NUM}    # -J是指定线程数,核数越多，编译速度越快
    make install
popd

ldconfig

# cleanup，安装完之后所有的源码库都没有用了，完全可以被删除掉了
rm -rf $PKG_NAME grpc-$VERSION