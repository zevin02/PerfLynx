#!/usr/bin/env bash

# xhost是x服务器的控制工具,并且忽略所有的命令输出信息,将标准错误流里面的信息也全部放到1中，全部都重定向到/dev/null中
xhost +local:root 1>/dev/null 2>&1

# 进入到容器中，以root身份进入，-it是交互式的进入，/bin/bash是进入到容器中的bash中

docker exec \
    -u root \
    -it linux_monitor \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1