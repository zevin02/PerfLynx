#pragma once

#include <string>
#include <unordered_map>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//我们的时间设置的是3s更新一次
/*
  平均负载代表的是活跃的进程数，平均负载提高不代表CPU使用率高，平均负载是指单位时间内，系统处于可运行状态和不可中断状态的平均进程数，也就是（平均活跃进程数），它和CPU使用率没有直接关系，可以理解为进程数量
  CPU使用率是，是指单位时间内CPU繁忙情况的统计，和平均负载并一定很高
  CPU密集型进程，使用大量CPU会导致平均负载提高
  io密集型进程：等待IO也会导致平均负载提高，但cpu使用率也不会高（io等待不计算进入到CPU的使用情况,这个时间段CPU处在一个空闲的时间）
  大量等待CPU的进程调度也会导致平均负载升高，此时CPU的使用率也比较高
  平均负载高不一定代表CPU使用率高，还有可能是IO更加繁忙了


  /proc/stat提供的就是系统的CPU和任务统计信息,
  如果我们发现了IO的等待时间很长，说明当前CPU正在等待IO完成，这就肯呢该是网络，网络，或其他设备的瓶颈导致，而不是CPU的瓶颈导致的
  我们就可以通过IO操作 增加缓存等方式来减少IO等待时间就 提高系统性能
  
  隔3s取一次值，分析CPU的使用率
  CPU的使用率=cpu的使用时间/CPU总时间，在等待的时候，CPU是空闲出来的（CPU的忙碌情况）

  高CPU使用率，低平均负载：CPU正在处理计算密集型的任务，而没有太多需要等待的进程
  低CPU使用率，高平均负载：CPU正在处理IO密集型的任务，而没有太多需要计算的进程
  高CPU使用率，高平均负载：表示当前有很多进程正在运行，同时CPU资源已经饱和了
  低CPU使用率，低平均负载：表示当前系统比较空闲，没有什么任务需要处理，有足够资源来处理任务
*/


//当前是统计cpu的使用情况

//由于是继承了抽象类的方法，所以每个实现类中，都有抽象类的实现方法
namespace monitor {
class CpuStatMonitor : public MonitorInter {
  struct CpuStat {
    std::string cpu_name;//当前的CPU的名字
    float user;//(ui)CPU处与用户态的时间
    float system;//(sy)CPU处与内核态的时间
    float idle;//(id)cpu的空闲时间，不包括IO的等待时间
    float nice;//(ni)低优先级用户的CPU使用时间，数值越大，优先级越低
    float io_wait;//(wa)cpu的io等待使用率，不占用cpu的使用率的
    float irq;//(hi)cpu的硬中断时间
    float soft_irq;//cpu的软中断时间
    float steal;//(st)系统在虚拟机中的时候，被其他虚拟机占用的时间，如果当前没有虚拟环境，CPU就是完全被当前系统占用的；
    float guest;//
    float guest_nice;
  };

 public:
  CpuStatMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}//重写父类的方法，这个方法是空的

 private:
  std::unordered_map<std::string, struct CpuStat> cpu_stat_map_;//这个里面存储的是上一次的老的数据，新的数据和之前的数据进行比对来计算CPU的使用率
};

}  // namespace monitor
