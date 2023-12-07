#pragma once

#include <QApplication>
#include <thread>
#include "client/rpc_client.h"
#include "monitor_widget.h"

int main(int argc, char* argv[]) {
  //使用argc，argv来初始化qt的应用程序
  QApplication app(argc, argv);//对于qt的应用程序，有且只能有一个，这个就是应用程序的对象
  monitor::MonitorWidget moitor_widget;//
  monitor::RpcClient rpc_client;//display的客户端
  monitor::proto::MonitorInfo monitor_info;//这个是读取到的数据，这个是从服务器中读取到的数据

  //get board name
  rpc_client.GetMonitorInfo(&monitor_info);//display调用get方法，获得内核模块中的数据
  std::string name = monitor_info.name();

  //QWidget类是所有窗口的父类，意味着所有窗口类对象只要指定了父对象，都可以实现内存资源的回收
  QWidget* widget = moitor_widget.ShowAllMonitorWidget(name);//设置他的名字，同时获得总的窗口的布局
  widget->show();//进行展示

  std::unique_ptr<std::thread> thread_;
  thread_ = std::make_unique<std::thread>([&]() {
    while (true) {
      monitor_info.Clear();
      rpc_client.GetMonitorInfo(&monitor_info);//groc的客户端，调用grpc的get方法就能调用到

      moitor_widget.UpdateData(monitor_info);//给数据模型进行更新数据
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  });
  thread_->detach();//使用线程分离，如果阻塞等待的话，后面的exec就不能执行了，os会自己回收的

  return app.exec();//进入主循环，知道exit（）被调用
}