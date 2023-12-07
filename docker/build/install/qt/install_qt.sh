#!/usr/bin/env bash

set -e #脚本的开头，用于设置脚本的环境，这里的-e是指当命令执行失败，就会退出，不会继续执行下面的命令

# 指定构建的类型，默认是download
BUILD_TYPE="${1:-download}"
# 当前脚本的目录 &&,就是仅允许在第一个命令执行成功之后才能执行后面的指令，进入到里面的路径后，打印出当前的路径，去掉他的所有符号链接
CURR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

# 指定当前系统的架构
TARGET_ARCH="$(uname -m)"


# 安装qt的版本号，和安装包的名称
# qt version
QT_VERSION_A=5.12
QT_VERSION_B=5.12.9
QT_VERSION_Z=$(echo "$QT_VERSION_B" | tr -d '.')

# 安装qt的路径
# docker/build/install/qt/qt-opensource-linux-x64-5.12.9.run
QT_INSTALLER=qt-opensource-linux-x64-5.12.9.run
# https://download.qt.io/archive/qt/5.12/5.12.9/qt-opensource-linux-x64-5.12.9.run

MY_DEST_DIR="/usr/local/Qt${QT_VERSION_B}"
# cuteci工具执行安装，cuteci工具用来自动化安装的过程，让qt安装更加容易
# 因为这个都是在docker配置镜像中安装的，所以这些都在docker中的/tmp/install中,
cuteci --installer /tmp/install/qt/qt-opensource-linux-x64-5.12.9.run \
    install \
    --destdir="$MY_DEST_DIR" \
    --packages "qt.qt5.${QT_VERSION_Z}.gcc_64" \
    --keep-tools                               


# 设置qt的环境变量，和路径

QT5_PATH="/usr/local/qt5"   # QT的安装路径
# Hide qt5 version from end users
# 把QT8下的gcc_64软连接到QT5_PATH下面
ln -s ${MY_DEST_DIR}/${QT_VERSION_B}/gcc_64 "${QT5_PATH}"

# QT生成的各种库文件路径，写入到qt.conf这个文件中
echo "${QT5_PATH}/lib" > /etc/ld.so.conf.d/qt.conf
ldconfig

# 定义一个__mytext变量，把所有的环境变量都写入到这个变量中

__mytext="""
export QT5_PATH=\"${QT5_PATH}\"
export QT_QPA_PLATFORM_PLUGIN_PATH=\"\${QT5_PATH}/plugins\"
add_to_path \"\${QT5_PATH}/bin\"
"""




# clean up
rm -f ${QT_INSTALLER}
# Keep License files
rm -rf ${MY_DEST_DIR}/{Docs,Examples,Tools,dist} || true
rm -rf ${MY_DEST_DIR}/MaintenanceTool* || true
rm -rf ${MY_DEST_DIR}/{InstallationLog.txt,installer-changelog} || true
rm -rf ${MY_DEST_DIR}/{components,network}.xml || true
