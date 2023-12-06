#pragma once

#include <string>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"

/*
  抽象类：规范实现类的方法

*/

namespace monitor {
class MonitorInter {
 public:
  MonitorInter() {}
  virtual ~MonitorInter() {}
  virtual void UpdateOnce(monitor::proto::MonitorInfo* monitor_info) = 0;//接口3s更新一次
  virtual void Stop() = 0;//
};
}  // namespace monitor