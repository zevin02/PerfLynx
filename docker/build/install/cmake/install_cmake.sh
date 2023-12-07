#!/usr/bin/env bash

set -e
# BASH_SOURCE是当前脚本文件的路径

cd "$(dirname "${BASH_SOURCE[0]}")"

THREAD_NUM=$(nproc)

# https://github.com/Kitware/CMake/archive/refs/tags/v3.26.4.tar.gz
# Install cmake.
VERSION="3.26.4"
PKG_NAME="CMake-${VERSION}.tar.gz"

tar xzf "${PKG_NAME}"
pushd CMake-${VERSION}
# 指定安装的路径
./configure --prefix=/usr/local/
make -j${THREAD_NUM}
make install
popd

rm -rf PKG_NAME CMake-${VERSION}