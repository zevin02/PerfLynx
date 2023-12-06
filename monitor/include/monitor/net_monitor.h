#pragma once

#include <string>
#include <unordered_map>
#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class NetMonitor : public MonitorInter {
  struct NetInfo {
    std::string name;
    int64_t rcv_bytes;//接收的字节数
    int64_t rcv_packets;//接收的包数
    int64_t err_in;//设备驱动检测到的接收错误
    int64_t drop_in;//设备驱动丢失的数据包数
    int64_t snd_bytes;
    int64_t snd_packets;
    int64_t err_out;
    int64_t drop_out;
    boost::chrono::steady_clock::time_point timepoint;
  };

 public:
  NetMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

 private:
  std::unordered_map<std::string, struct NetInfo> net_info_;//同样也是需要有两个数据，一个是老的数据，一个是新的数据，然后进行比对
};

}  // namespace monitor