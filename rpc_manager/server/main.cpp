#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "rpc_manager.h"
/*
  客户端在不知道调用细节的情况下，调用存在于远程计算机上的某个对象，就像调用本地应用程序一样
  ：一种通过网络从远程计算机程序上请求服务，而不需要了解底层网络技术的协议（既然rpc客户端认为自己是在调用本地对象，那么传输层使用TCP/UDP还是http协议，都不需要关心了）

  grpc中基于HTTP2协议设计，解决tcp的粘包问题，并进行优化，长链接和多路复用，使用二进制格式进行传输，支持头部压缩，减少了响应头和请求头的信息可以减少数据传输的大小
*/
//constexpr是在编译时候就进行处理了，而如果不加的话，就是在代码运行的时候才进行处理，减少运行期间资源的消耗，
constexpr char kServerPortInfo[] = "0.0.0.0:50051";//这里的ip监听的是所有的ip地址，端口号都是50051
void InitServer() {
  grpc::ServerBuilder builder;
  //添加要监听的ip和端口号，同时设置不需要加密认证
  builder.AddListeningPort(kServerPortInfo, grpc::InsecureServerCredentials());

  monitor::GrpcManagerImpl grpc_server;//这个是我们自定义的服务，就需要注册到grpc中
  builder.RegisterService(&grpc_server);//将当前的方法注册到server的服务中
//启动服务，建立并且启动服务
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();//进行阻塞等待，

  return;
}
//服务器中的mainn函数
int main() {
  InitServer();
  return 0;
}