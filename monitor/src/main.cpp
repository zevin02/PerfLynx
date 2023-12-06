#pragma once
#include <memory>
#include <thread>
#include <vector>

#include "client/rpc_client.h"
#include "monitor/cpu_load_monitor.h"
#include "monitor/cpu_softirq_monitor.h"
#include "monitor/cpu_stat_monitor.h"
#include "monitor/mem_monitor.h"
#include "monitor/monitor_inter.h"
#include "monitor/net_monitor.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

/*
  在工作开发中，服务器开发，后台开发，在编写代码，会遇到一些性能问题，进行性能优化的操作，从哪里获得性能相关的操作部分，
  
*/
int main() {
  //这里我们创建了一个vector，里面存储的是MonitorInter的智能指针，我们需要一个工厂来生产这些对象的数据
  //这个就是父类的指针指向子类的引用
  //使用智能指针，来管理这些对象的生命周期
  std::vector<std::shared_ptr<monitor::MonitorInter>> runners_;//监控对象的创建，多态的基类指针指向子类的对象
  runners_.emplace_back(new monitor::CpuSoftIrqMonitor());//构造子类的对象，赋值给基类管理
  runners_.emplace_back(new monitor::CpuLoadMonitor());
  runners_.emplace_back(new monitor::CpuStatMonitor());
  runners_.emplace_back(new monitor::MemMonitor());
  runners_.emplace_back(new monitor::NetMonitor());
  //创造一些子类的对象

  //这个地方就是rpc的客户端程序代码，使用这个客户端的接口来访问服务器
  monitor::RpcClient rpc_client_;//创建一个客户端对象，来发送数据给grpc的服务端
  char *name = getenv("USER");//获得当前用户的用户名
  std::unique_ptr<std::thread> thread_ = nullptr;//创建了一个线程的对象
  //使用lambda表达式来初始化当前线程，这里我们使用unique_ptr来管理这个线程的生命周期,在生命周期结束的时候，会自动完成数据的
  thread_ = std::make_unique<std::thread>([&]() {
    //监控数据的更新
    while (true) {
      monitor::proto::MonitorInfo monitor_info;//这个里面存储的就是全部的数据,最外层的一个数据结构
      monitor_info.set_name(std::string(name));//先设置当前对象的名字
      //在runners的每个子类中调用UpdataOnce来更新数据
      for (auto& runner : runners_) {
        //通过工厂方法调用各种子类的方法
        //遍历子类对象，调用相应的实现方法
        runner->UpdateOnce(&monitor_info);//在这个地方进行更新数据，
      }
      //先把数据填充好再发送给grpc
      rpc_client_.SetMonitorInfo(monitor_info);//这里我们把数据传给grpc的服务，让grpc进行次数据的处理
      std::this_thread::sleep_for(std::chrono::seconds(3));//这个线程每3s去更新一次数据
    }
  });

  thread_->join();//阻塞等待线程结束
  return 0;
}
