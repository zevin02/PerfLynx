#include "rpc_manager.h"
#include <iostream>

namespace monitor {
GrpcManagerImpl::GrpcManagerImpl() {}
//
GrpcManagerImpl::~GrpcManagerImpl() {}
//客户端调用当前set接口的时候，服务段的set函数，就能受到客户端的数据
::grpc::Status GrpcManagerImpl::SetMonitorInfo(
    ::grpc::ServerContext* context,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response) {
  monitor_infos_.Clear();//把之前存储在这里的数据进行清空
  monitor_infos_ = *request;//把客户端传输的数据，赋值给服务端,进行解析，并恢复给客户端，因为是指针，所以这里我们需要解指针
  // std::cout << "jinru" << request->soft_irq_size() << std::endl;
  //   for (int i = 0; i < request->soft_irq_size(); i++) {
  //     std::cout << request->soft_irq(i).cpu() << " " <<
  //     request->soft_irq(i).hi()
  //               << " " << request->soft_irq(i).timer() << " "
  //               << request->soft_irq(i).net_tx() << " "
  //               << request->soft_irq(i).net_rx() << " "
  //               << request->soft_irq(i).block() << " "
  //               << request->soft_irq(i).irq_poll() << std::endl;
  //   }
  return grpc::Status::OK;
}
::grpc::Status GrpcManagerImpl::GetMonitorInfo(
    ::grpc::ServerContext* context, const ::google::protobuf::Empty* request,
    ::monitor::proto::MonitorInfo* response) {
  //   monitor::proto::MonitorInfo monitor_info;
  //   auto soft_irq = monitor_info.add_soft_irq();
  //   soft_irq->set_cpu("cpu1");
  //   soft_irq->set_hi(100);
  //   soft_irq->set_timer(100);
  //   soft_irq->set_net_tx(100);
  //   soft_irq->set_net_rx(100);
  //   auto soft_irq2 = monitor_info.add_soft_irq();
  //   soft_irq2->set_cpu("cpu2");

  //   for (int i = 0; i < monitor_infos_.soft_irq_size(); i++) {
  //     std::cout << monitor_infos_.soft_irq(i).cpu() << " "
  //               << monitor_infos_.soft_irq(i).hi() << " "
  //               << monitor_infos_.soft_irq(i).timer() << " "
  //               << monitor_infos_.soft_irq(i).net_tx() << " "
  //               << monitor_infos_.soft_irq(i).net_rx() << " "
  //               << monitor_infos_.soft_irq(i).block() << " "
  //               << monitor_infos_.soft_irq(i).irq_poll() << std::endl;
  //   }
  *response = monitor_infos_;//只需要把存储在服务器中的数据，给到这个传出参数即可
  return grpc::Status::OK;
}

}  // namespace monitor
