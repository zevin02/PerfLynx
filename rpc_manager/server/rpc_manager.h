#pragma once

#include <grpcpp/support/status.h>

#include <unordered_map>
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
  //里面生成了服务器的各种接口
class GrpcManagerImpl : public monitor::proto::GrpcManager::Service {
 public:
  GrpcManagerImpl();

  virtual ~GrpcManagerImpl();

  ::grpc::Status SetMonitorInfo(::grpc::ServerContext* context,
                                const ::monitor::proto::MonitorInfo* request,
                                ::google::protobuf::Empty* response);
  ::grpc::Status GetMonitorInfo(::grpc::ServerContext* context,
                                const ::google::protobuf::Empty* request,
                                ::monitor::proto::MonitorInfo* response);

 private:
  monitor::proto::MonitorInfo monitor_infos_;//这个是传输的数据，存储客户端发送过来的数据，这样就能在网络中进行传输
};

}  // namespace monitor
