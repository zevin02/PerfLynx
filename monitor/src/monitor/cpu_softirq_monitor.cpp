#include "monitor/cpu_softirq_monitor.h"

#include "utils/read_file.h"
#include "utils/utils.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
//cpu的软中断
namespace monitor {
void CpuSoftIrqMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile softirqs_file(std::string("/proc/softirqs"));
  std::vector<std::string> one_softirq;//一行的中断数据
  //由于在/proc/softirqs中的数据都是纵向放的，所以我们需要
  std::vector<std::vector<std::string>> softirq;//
  while (softirqs_file.ReadLine(&one_softirq)) {
    //读取以行一行的数据
    softirq.push_back(one_softirq);//放入到二维数组中
    one_softirq.clear();//把当前的一行数据清空，给下一次进行使用
  }
  //把数据放到后面的代码中进行分析
  //遍历二维数组,由于这里的CPU是空了一个位置来进行保存的
  for (int i = 0; i < softirq[0].size() - 1; i++) {
    std::string name = softirq[0][i];//获得每一行的数据
    struct SoftIrq info;//一个软中断的数据结构，并把得到的数据存放到当前的结构体中
    info.cpu_name = name;
    //因为我们需要跳过第一行的数据
    info.hi = std::stoll(softirq[1][i + 1]);//从第一个开始，因为第一个是cpu的名字，所以我们需要跳过第一个
    info.timer = std::stoll(softirq[2][i + 1]);
    info.net_tx = std::stoll(softirq[3][i + 1]);
    info.net_rx = std::stoll(softirq[4][i + 1]);
    info.block = std::stoll(softirq[5][i + 1]);
    info.irq_poll = std::stoll(softirq[6][i + 1]);
    info.tasklet = std::stoll(softirq[7][i + 1]);
    info.sched = std::stoll(softirq[8][i + 1]);
    info.hrtimer = std::stoll(softirq[9][i + 1]);
    info.rcu = std::stoll(softirq[10][i + 1]);
    info.timepoint = boost::chrono::steady_clock::now();//获得当前的时间点，用作下一次的计算时间间隔

    auto iter = cpu_softirqs_.find(name);//在map中查找之前存放的数据
    if (iter != cpu_softirqs_.end()) {
      struct SoftIrq& old = (*iter).second;//获得之前的数据的结构体
      double period = Utils::SteadyTimeSecond(info.timepoint, old.timepoint);//计算两个时间点之间的时间间隔
      auto one_softirq_msg = monitor_info->add_soft_irq();//
      //计算单位时间内中断卷的次数，我们都是取两个时间段的差值，然后除以时间间隔，就是单位时间内的中断次数，计算的就是各种中断的速率(我们需要判断，是不是一直在递增的，我们就需要去查看是不是有问题)
      one_softirq_msg->set_cpu(info.cpu_name);//
      //把两次数据的差值除以时间间隔，就是单位时间内的中断次数，计算的就是各种中断的速率
      one_softirq_msg->set_hi((info.hi - old.hi) / period);
      one_softirq_msg->set_timer((info.timer - old.timer) / period);
      one_softirq_msg->set_net_tx((info.net_tx - old.net_tx) / period);
      one_softirq_msg->set_net_rx((info.net_rx - old.net_rx) / period);
      one_softirq_msg->set_block((info.block - old.block) / period);
      one_softirq_msg->set_irq_poll((info.irq_poll - old.irq_poll) / period);
      one_softirq_msg->set_tasklet((info.tasklet - old.tasklet) / period);
      one_softirq_msg->set_sched((info.sched - old.sched) / period);
      one_softirq_msg->set_hrtimer((info.hrtimer - old.hrtimer) / period);
      one_softirq_msg->set_rcu((info.rcu - old.rcu) / period);
    }
    cpu_softirqs_[name] = info;//把新的数据放到map中，下次来继续使用
  }
  return;
}

}  // namespace monitor