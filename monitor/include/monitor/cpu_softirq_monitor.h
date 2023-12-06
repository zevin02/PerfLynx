#pragma once

#include <string>
#include <unordered_map>

#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//这个地方是专门分析软中断的情况的
/*
  /proc/softirqs:记录自从开机以来的软中断累积次数
  HI：表示高优先级的软中断（高优先级主要用于处理网络和其他高优先级的任务，这个机制允许内核延迟一些任务的执行，而不是立即在硬中断上下文中执行）
  {
  }
  
  TIMER:定时中断
  NET_TX:网络发送中断
  NET_RX:网络接收中断
  SCHED：内核调度
  RCU：RCU所中，网络接收变化最快(read copy update)
  /proc/interrupts:记录自从开机以来的中断累积次数

  中断就是一个异步处理机制,可以提高系统并发处理的能力
  中断处理程序在响应中断的时候，会临时关闭中断响应，所以在上一次中断完成之前，无法接收其他中断（中断丢失）
  Linux于是就把一个中断分成了两个部分，上半部分和下半部分
  上半部分：中断处理程序，尽可能的快速的完成中断处理，然后尽快的退出，这样就可以尽快的响应其他中断（处理和硬件相关的的东西），并发送一个软中断（快速执行）
  下半部分：由于是内核触发的，软中断；每个CPU都有一个内核线程，来处理软中断
  网卡发送中断：网卡把数据读取到内存中，并更新硬件寄存器的状态，并发送一个软中断，通知内核数据已经准备好了，可以进行下一步的处理了



*/

//每个都继承了抽象类的方法，工厂的代码

namespace monitor {
class CpuSoftIrqMonitor : public MonitorInter {
  //这个地方我们存放的都是各个数据的速率
  struct SoftIrq {
    std::string cpu_name;
    int64_t hi;//
    int64_t timer;
    int64_t net_tx;
    int64_t net_rx;
    int64_t block;
    int64_t irq_poll;
    int64_t tasklet;
    int64_t sched;
    int64_t hrtimer;
    int64_t rcu;
    boost::chrono::steady_clock::time_point timepoint;//时间点的成员变量，每次读取的时间点
  };

 public:
  CpuSoftIrqMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

 private:
  std::unordered_map<std::string, struct SoftIrq> cpu_softirqs_;//同样记录上一次的数据，然后和当前的数据进行比对，计算软中断的速率
};

}  // namespace monitor
