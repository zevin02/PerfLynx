#pragma once

#include <string>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//cpu的负载，
/*
  我们查看/proc/loadavg文件，可以看到这样的内容
  0.10（平均1min时间内，只有0.1个进程在运行，电脑完全是空闲可用的） 0.05 0.01 1/102 1234
  这些数字分别代表的是：
  1分钟的平均负载，5分钟的平均负载，15分钟的平均负载，当前运行的进程数/总进程数，最近运行的进程ID
  (3个时间段都要看，)


  平均负载是指单位时间内，系统处于可运行状态和不可中断状态的平均进程数，也就是（平均活跃进程数），它和CPU使用率没有直接关系，可以理解为进程数量
  举例说明：
  比如当平均负载是2时候，在2个cpu的系统上运行，那么意味着所有的cpu都被占用了，1.0
  如果在4个cpu的系统上运行，那么意味着只有50%的cpu被占用了，有一半的cpu空闲，一半的cpu在运行，0.5
  如果在1个cpu的系统上运行，意味着有50%的进程在等待cpu资源

  平均负载，最合理的情况就是CPU的个数,如果平均负载比cpu的个数还大的话，系统就出现了过载
  /proc/cpuinfo文件中的processor的个数就是cpu的个数=16

  平均负载的参考：
  三个不同时间间隔的平均值为我们分析系统负载趋势给了一个非常立体的了解
  如果1分钟，5分钟，15分钟三个时间段的值基本一致，说明系统的负载情况非常稳定，
  如果1分钟的值远远大于5分钟和15分钟的值，说明系统的负载在增加，我们就需要把系统的负载情况进行分析（这个可能是一个临时性的操作），找出系统的瓶颈，考虑是否可以杀掉某一些进程，或者增加硬件资源
  如果1分钟的值远远小于5分钟和15分钟的值，说明系统的负载在减少，而过去15分钟哦内却有很大的负载，我们就需要进行性能分析
  如果单核CPU中，平均负载为1.73,0.60,7.98;说明当前一分钟内有73%的超载，而在过去15分钟内有698%的超载，这个系统的负载是非常严重的，需要进行性能分析
  
  */


//CPU load中继承了MonitorInter的方法

namespace monitor {
class CpuLoadMonitor : public MonitorInter {
 public:
  CpuLoadMonitor() {}
  //都是通过monitorInfo来操作数据
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

 private:
  //当前这个结构体的类成员变量
  float load_avg_1_;//1分钟的平均负载
  float load_avg_3_;//5分钟的平均负载
  float load_avg_15_;//15分钟的平均负载
};

}  // namespace monitor
