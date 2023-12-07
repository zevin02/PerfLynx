#include "rpc_client.h"

namespace monitor {
RpcClient::RpcClient() {
  //在构造函数的时候，就会创建一个通道，然后通过通道来创建存根，实现端到段的链接，我们可以设置其ip：port
  //构造函数，首先先创建一个通道，
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());//第二个参数，我们设置的是不需要加密认证
  stub_ptr_ = monitor::proto::GrpcManager::NewStub(channel);//创建完一个通道之后，新建立一个存根对象，与服务段进行交互
}
RpcClient::~RpcClient() {}
//客户端发送一些信息给服务器
void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo& monito_info) {
  //存根已经存在了，就需要可以进行远程链接
  ::grpc::ClientContext context;//grpc的上下文信息，（可以设置一些超时时间）
  ::google::protobuf::Empty response;//相应的数据，因为我们不需要任何的相应的数据，所以这里我们使用的是空的数据
  ::grpc::Status status =
      stub_ptr_->SetMonitorInfo(&context, monito_info, &response);//底层实际是用这个来发送给服务端，通过存根来进行发送数据
  if (status.ok()) {//服务器接收到了客户端发送给来的数据，服务器就返回当前的状态
  } else {
    std::cout << status.error_details() << std::endl;
    std::cout << "falied to connect !!!" << std::endl;
  }
}

//客户端读取服务器中的数据，这个接口的请求参数是空的，只需要获得一个相应的数据
void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo* monito_info) {
  ::grpc::ClientContext context;
  ::google::protobuf::Empty request;
  ::grpc::Status status =
      stub_ptr_->GetMonitorInfo(&context, request, monito_info);//客户端把服务器的相应数据读取出来，赋值给monito_info,并传出去
  if (status.ok()) {
  } else {//没有读取到数据，就链接失败了
    std::cout << status.error_details() << std::endl;
    std::cout << "falied to connect !!!" << std::endl;
  }
}
}  // namespace monitor
