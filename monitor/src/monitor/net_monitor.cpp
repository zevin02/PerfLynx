#include "monitor/net_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

//网络的分析
namespace monitor {
void NetMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile net_file(std::string("/proc/net/dev"));
  std::vector<std::string> net_datas;
  while (net_file.ReadLine(&net_datas)) {
    std::string name = net_datas[0];
    if (name.find(':') == name.size() - 1 && net_datas.size() >= 10) {
      struct NetInfo net_info;
      name.pop_back();//删除掉最后一个字符
      net_info.name = name;
      net_info.rcv_bytes = std::stoll(net_datas[1]);
      net_info.rcv_packets = std::stoll(net_datas[2]);
      net_info.err_in = std::stoll(net_datas[3]);
      net_info.drop_in = std::stoll(net_datas[4]);
      net_info.snd_bytes = std::stoll(net_datas[9]);
      net_info.snd_packets = std::stoll(net_datas[10]);
      net_info.err_out = std::stoll(net_datas[11]);
      net_info.drop_out = std::stoll(net_datas[12]);
      net_info.timepoint = boost::chrono::steady_clock::now();//当前的时间增量

      auto iter = net_info_.find(name);//在之前的数据中进行查找相关的数据
      if (iter != net_info_.end()) {
        struct NetInfo old = std::move(iter->second);
        double period =
            Utils::SteadyTimeSecond(net_info.timepoint, old.timepoint);//计算一段周期时间
        auto one_net_msg = monitor_info->add_net_info();//获得一个网络的对象
        one_net_msg->set_name(net_info.name);//设置网络的名字
        //计算各种网络的运行速率
        one_net_msg->set_send_rate((net_info.snd_bytes - old.snd_bytes) /
                                   1024.0 / period);//看丢包的速率是不是一直递增的，如果递增的，就有问题，进行分析排查
        one_net_msg->set_rcv_rate((net_info.rcv_bytes - old.rcv_bytes) /
                                  1024.0 / period);
        one_net_msg->set_send_packets_rate(
            (net_info.snd_packets - old.snd_packets) / period);
        one_net_msg->set_rcv_packets_rate(
            (net_info.rcv_packets - old.rcv_packets) / period);
      }
      net_info_[name] = net_info;
    }
    net_datas.clear();
  }
  return;
}
}  // namespace monitor
