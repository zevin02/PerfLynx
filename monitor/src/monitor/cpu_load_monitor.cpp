#include "monitor/cpu_load_monitor.h"

#include "utils/read_file.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
//cpu的负载情况
namespace monitor {
  //CPU负载中实现更新负载
  //读取文件的内容，将数据转化，并把数据进行赋值，再进行传出
void CpuLoadMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile cpu_load_file(std::string("/proc/loadavg"));//打开一个文件，构造一个readfile的对象，来读取/proc/loadavg文件，这个文件里面存储的就是各种负载
  std::vector<std::string> cpu_load;
  cpu_load_file.ReadLine(&cpu_load);//读取一行的数据，并把数据存入到cpu_load数组中

  load_avg_1_ = std::stof(cpu_load[0]);//把数据转换成float类型
  load_avg_3_ = std::stof(cpu_load[1]);
  load_avg_15_ = std::stof(cpu_load[2]);
  //创建一个对象（因为他不是一个repeated类型）


  auto cpu_load_msg = monitor_info->mutable_cpu_load();//获得一个可变的cpu_load对象
  //往这个里面的成员变量赋值，这几个就是获得的cpu的负载,给这个对象进行添加一些数据
  cpu_load_msg->set_load_avg_1(load_avg_1_);
  cpu_load_msg->set_load_avg_3(load_avg_3_);
  cpu_load_msg->set_load_avg_15(load_avg_15_);
  //参数给定成功
  return;
}

}  // namespace monitor