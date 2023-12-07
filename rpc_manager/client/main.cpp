#pragma once

#include <string>
#include "rpc_client.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

int main(int argc, char* argv[]) {
  // Setup request
  //创建了两个soft_irq对象，
  monitor::proto::MonitorInfo monitor_info;//gouza
  auto soft_irq = monitor_info.add_soft_irq();//获得一个soft——irq的一个对象
  soft_irq->set_cpu("cpu1");//对这个对象中的成员变量进行赋值,设置一个cpu的值
  auto soft_irq2 = monitor_info.add_soft_irq();
  soft_irq2->set_cpu("cpu2");
  //客户端设置完数据之后，发送给rpc客户端
  monitor::RpcClient rpc_client;
  rpc_client.SetMonitorInfo(monitor_info);//把客户端传输的一些类的数据，发送给服务段

  return 0;
}