#pragma once

#include <string>
#include <unordered_map>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class MemMonitor : public MonitorInter {
  struct MenInfo {
    int64_t total;//总内存大小，包括
    int64_t free;//完全没有用到的物理内存空间，完全没有用到的空间
    int64_t avail;//可以被应用程序使用的内存空间，avail包含了free和一些内核缓存，avail>free
    int64_t buffers;//buffer是对数据的缓存，
    int64_t cached;//cache是对文件的缓存，对于文件系统而言的
    int64_t swap_cached;
    int64_t active;
    int64_t in_active;
    int64_t active_anon;
    int64_t inactive_anon;
    int64_t active_file;
    int64_t inactive_file;
    int64_t dirty;
    int64_t writeback;
    int64_t anon_pages;
    int64_t mapped;
    int64_t kReclaimable;
    int64_t sReclaimable;
    int64_t sUnreclaim;
  };

 public:
  MemMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}
};

}  // namespace monitor