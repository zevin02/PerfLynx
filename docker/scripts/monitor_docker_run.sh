#!/usr/bin/env bash

# BASH_SOURCE[0]是指当前脚本的路径:/home/zevin/githubmanage/private-node/docker/scripts/monitor_docker_run.sh
# dirname就是文件所在的目录，这里回退两次就是到:/home/zevin/githubmanage/private-node


# 这一整条指令就是把MONITOR_HOME_DIR=/home/zevin/githubmanage/private-node
MONITOR_HOME_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"

display=""
if [ -z ${DISPLAY} ];then
    display=":1"
else
    display="${DISPLAY}"
fi

# 主机名
local_host="$(hostname)"
user="${USER}"
uid="$(id -u)"
group="$(id -g -n)"
gid="$(id -g)"


echo "stop and rm docker" 
# 把上一个容器体内停止,,
docker stop linux_monitor > /dev/null
# docker rm 是删除一个容器，-v是删除容器挂载的数据卷，-f 是强制删除
docker rm -v -f linux_monitor > /dev/null

echo "start docker"

# docker run -v就是进行数据卷的挂载，-e 就是 这个就是创建容器,同时进行映射到/work目录下面
# -v，把当前private-node在docker中就是命名成了work目录
# --net host使用host模式（主机器）
# docker内外的ip是一样的
# -e就是环境变量的意思,docker内外的信息保持一致，把他复给docker里面的文件信息
# 我们把当前的项目目录挂载到docker下的work目录，所以在docker内外对这个目录进行修改，修改的操作都会持久化保留下来



docker run -it -d \
--name linux_monitor \
-e DISPLAY=$display \
-e DOCKER_USER="${user}" \
-e USER="${user}" \
-e DOCKER_USER_ID="${uid}" \
-e DOCKER_GRP="${group}" \
-e DOCKER_GRP_ID="${gid}" \
-e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
-v ${MONITOR_HOME_DIR}:/work \
-v ${XDG_RUNTIME_DIR}:${XDG_RUNTIME_DIR} \
--net host \
linux:monitor