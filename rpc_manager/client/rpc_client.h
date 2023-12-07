#pragma once

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
  //客户端的实现类
class RpcClient {
 public:
  RpcClient();
  ~RpcClient();
  void SetMonitorInfo(const monitor::proto::MonitorInfo& monito_info);
  void GetMonitorInfo(monitor::proto::MonitorInfo* monito_info);

 private:
 //只有通过这个存根才能和服务段链接，
 //同时这里使用只能指针
  std::unique_ptr<monitor::proto::GrpcManager::Stub> stub_ptr_;//客户端存根的stub，这个就是为了与服务器进行链接
};
}  // namespace monitor
