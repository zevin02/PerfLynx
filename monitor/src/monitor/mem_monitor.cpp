#include "monitor/mem_monitor.h"
#include "utils/read_file.h"

//内存的分析
namespace monitor {
static constexpr float KBToGB = 1000 * 1000;

void MemMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile mem_file("/proc/meminfo");//我们通过读取/proc/meminfo文件来获取内存的信息，再进行一个切割
  struct MenInfo mem_info;
  std::vector<std::string> mem_datas;
  while (mem_file.ReadLine(&mem_datas)) {//读取一行的数据
  //对第一个字段进行如下的判断，然后进行赋值
    if (mem_datas[0] == "MemTotal:") {
      mem_info.total = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "MemFree:") {
      mem_info.free = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "MemAvailable:") {
      mem_info.avail = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Buffers:") {
      mem_info.buffers = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Cached:") {
      mem_info.cached = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "SwapCached:") {
      mem_info.swap_cached = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Active:") {
      mem_info.active = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Inactive:") {
      mem_info.in_active = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Active(anon):") {
      mem_info.active_anon = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Inactive(anon):") {
      mem_info.inactive_anon = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Active(file):") {
      mem_info.active_file = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Inactive(file):") {
      mem_info.inactive_file = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Dirty:") {
      mem_info.dirty = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Writeback:") {
      mem_info.writeback = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "AnonPages:") {
      mem_info.anon_pages = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "Mapped:") {
      mem_info.mapped = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "KReclaimable:") {
      mem_info.kReclaimable = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "SReclaimable:") {
      mem_info.sReclaimable = std::stoll(mem_datas[1]);
    } else if (mem_datas[0] == "SUnreclaim:") {
      mem_info.sUnreclaim = std::stoll(mem_datas[1]);
    }
    mem_datas.clear();
  }
  //前面都把数据放到一个结构体中，后面就需要根据当前的结构体来计算相应的数据
  auto mem_detail = monitor_info->mutable_mem_info();//获得一个mem_info的对象
  //计算内存的使用率
  mem_detail->set_used_percent((mem_info.total - mem_info.avail) * 1.0 /
                               mem_info.total * 100.0);
  mem_detail->set_total(mem_info.total / KBToGB);//总共的内存大小
  mem_detail->set_free(mem_info.free / KBToGB);
  mem_detail->set_avail(mem_info.avail / KBToGB);
  mem_detail->set_buffers(mem_info.buffers / KBToGB);
  mem_detail->set_cached(mem_info.cached / KBToGB);
  mem_detail->set_swap_cached(mem_info.swap_cached / KBToGB);
  mem_detail->set_active(mem_info.active / KBToGB);
  mem_detail->set_inactive(mem_info.in_active / KBToGB);
  mem_detail->set_active_anon(mem_info.active_anon / KBToGB);
  mem_detail->set_inactive_anon(mem_info.inactive_anon / KBToGB);
  mem_detail->set_active_file(mem_info.active_file / KBToGB);
  mem_detail->set_inactive_file(mem_info.inactive_file / KBToGB);
  mem_detail->set_dirty(mem_info.dirty / KBToGB);
  mem_detail->set_writeback(mem_info.writeback / KBToGB);
  mem_detail->set_anon_pages(mem_info.anon_pages / KBToGB);
  mem_detail->set_mapped(mem_info.mapped / KBToGB);
  mem_detail->set_kreclaimable(mem_info.kReclaimable / KBToGB);
  mem_detail->set_sreclaimable(mem_info.sReclaimable / KBToGB);
  mem_detail->set_sunreclaim(mem_info.sUnreclaim / KBToGB);

  return;
}
}  // namespace monitor
