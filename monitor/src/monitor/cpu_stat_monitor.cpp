#include "monitor/cpu_stat_monitor.h"

#include "utils/read_file.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//cpu的统计数据
namespace monitor {
void CpuStatMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile cpu_stat_file(std::string("/proc/stat"));//读取的实际上也是/proc/stat下内核运行时的各种数据
  std::vector<std::string> cpu_stat_list;//CPU一行中的各种统计信息
  while (cpu_stat_file.ReadLine(&cpu_stat_list)) {//读取多行的数据，我们需要读取全部的数据
    //我们只需要以cpu开头的行
    if (cpu_stat_list[0].find("cpu") != std::string::npos) {
      //因为文件中有多个cpu，所以monitor中是使用repeated来管理该对象

      // std::cout << cpu_stat_list[0] << cpu_stat_list[1] << std::endl;
      struct CpuStat cpu_stat;//构造一个cpu_stat的对象，cpu的一个对象信息
      cpu_stat.cpu_name = cpu_stat_list[0];
      cpu_stat.user = std::stof(cpu_stat_list[1]);
      cpu_stat.nice = std::stof(cpu_stat_list[2]);
      cpu_stat.system = std::stof(cpu_stat_list[3]);
      cpu_stat.idle = std::stof(cpu_stat_list[4]);
      cpu_stat.io_wait = std::stof(cpu_stat_list[5]);
      cpu_stat.irq = std::stof(cpu_stat_list[6]);
      cpu_stat.soft_irq = std::stof(cpu_stat_list[7]);
      cpu_stat.steal = std::stof(cpu_stat_list[8]);
      cpu_stat.guest = std::stof(cpu_stat_list[9]);
      cpu_stat.guest_nice = std::stof(cpu_stat_list[10]);
      //当前的是最新的一次操作
      auto it = cpu_stat_map_.find(cpu_stat.cpu_name);//在map里面查找是否有当前的cpu的名字
      if (it != cpu_stat_map_.end()) {
        //
        // std::cout << cpu_stat.cpu_name << std::endl;
        // 因为我们是每隔3s取一次值，所以我们需要计算出来CPU的使用率，所以这里取了两个值，然后进行做差来计算CPU的使用率

        struct CpuStat old = it->second;//获得旧的CPU的状态
        auto cpu_stat_msg = monitor_info->add_cpu_stat();//在monitor中构建一个CPU stat的一个对象
        //计算一个cpu的总共的运行时间
        float new_cpu_total_time = cpu_stat.user + cpu_stat.system +
                                   cpu_stat.idle + cpu_stat.nice +
                                   cpu_stat.io_wait + cpu_stat.irq +
                                   cpu_stat.soft_irq + cpu_stat.steal;

        float old_cpu_total_time = old.user + old.system + old.idle + old.nice +
                                   old.io_wait + old.irq + old.soft_irq +
                                   old.steal;
        
        float new_cpu_busy_time = cpu_stat.user + cpu_stat.system +
                                  cpu_stat.nice + cpu_stat.irq +
                                  cpu_stat.soft_irq + cpu_stat.steal;//新的CPU的忙碌时间，不包含io等待时间,和cpu的空闲时间
        float old_cpu_busy_time = old.user + old.system + old.nice + old.irq +
                                  old.soft_irq + old.steal;

        //CPU的使用率公式=忙碌时间/总时间
        float cpu_percent = (new_cpu_busy_time - old_cpu_busy_time) /
                            (new_cpu_total_time - old_cpu_total_time) * 100.00;//两次采样的差值计算CPU的使用率

        //用户态的CPU的使用率
        float cpu_user_percent = (cpu_stat.user - old.user) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;//
        //内核态的CPU的使用率
        float cpu_system_percent = (cpu_stat.system - old.system) /
                                   (new_cpu_total_time - old_cpu_total_time) *
                                   100.00;

        float cpu_nice_percent = (cpu_stat.nice - old.nice) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;
        float cpu_idle_percent = (cpu_stat.idle - old.idle) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;
        float cpu_io_wait_percent = (cpu_stat.io_wait - old.io_wait) /
                                    (new_cpu_total_time - old_cpu_total_time) *
                                    100.00;
        float cpu_irq_percent = (cpu_stat.irq - old.irq) /
                                (new_cpu_total_time - old_cpu_total_time) *
                                100.00;
        float cpu_soft_irq_percent = (cpu_stat.soft_irq - old.soft_irq) /
                                     (new_cpu_total_time - old_cpu_total_time) *
                                     100.00;

        cpu_stat_msg->set_cpu_name(cpu_stat.cpu_name);//把计算好的数据放到CPU的对象中
        cpu_stat_msg->set_cpu_percent(cpu_percent);
        cpu_stat_msg->set_usr_percent(cpu_user_percent);
        cpu_stat_msg->set_system_percent(cpu_system_percent);
        cpu_stat_msg->set_nice_percent(cpu_nice_percent);
        cpu_stat_msg->set_idle_percent(cpu_idle_percent);
        cpu_stat_msg->set_io_wait_percent(cpu_io_wait_percent);
        cpu_stat_msg->set_irq_percent(cpu_irq_percent);
        cpu_stat_msg->set_soft_irq_percent(cpu_soft_irq_percent);
      }
      
      cpu_stat_map_[cpu_stat.cpu_name] = cpu_stat;//每次处理完当前的数据了，之前的老数据就没用了，当前的值就变成了老数据，供下一次使用
    }
    //每次在读取下一行的数据之前，需要先把之前的数据先进行清空
    
    cpu_stat_list.clear();//用完之后就需要进行清空，留给
  }

  return;
}

}  // namespace monitor