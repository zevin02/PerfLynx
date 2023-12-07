#pragma once

#include <QStandardItemModel>
#include <QtWidgets>
#include <thread>
#include <string>

#include "cpu_load_model.h"
#include "cpu_softirq_model.h"
#include "cpu_stat_model.h"
#include "mem_model.h"
#include "net_model.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//窗口
namespace monitor {
  //必须要继承窗口的父类
class MonitorWidget : public QWidget {
  Q_OBJECT//加入这个，才能使用qt的信号机制
 public:
  explicit MonitorWidget(QWidget* parent = nullptr);
  ~MonitorWidget() {
    // stop_ = true;
    // if (thread_->joinable()) {
    //   thread_->join();
    // }
  }
  //自定义了一些窗口，像cpu，mmem，cpuload，cpu usage，net，softirq等需要监控的数据
  QWidget* ShowAllMonitorWidget(const std::string& name);
  QWidget* InitCpuMonitorWidget();
  QWidget* InitSoftIrqMonitorWidget();
  QWidget* InitMemMonitorWidget();
  QWidget* InitNetMonitorWidget();
  QWidget* InitButtonMenu(const std::string& name);
  // QWidget* InitMemMonitorWidget();

  void UpdateData(const monitor::proto::MonitorInfo& monitor_info);//使用这一部分的数据

 private slots:
  void ClickCpuButton();
  void ClickSoftIrqButton();
  void ClickMemButton();
  void ClickNetButton();

 private:
  QTableView* monitor_view_ = nullptr;
  QTableView* cpu_load_monitor_view_ = nullptr;
  QTableView* cpu_stat_monitor_view_ = nullptr;
  QTableView* mem_monitor_view_ = nullptr;
  QTableView* net_monitor_view_ = nullptr;

  MonitorBaseModel* monitor_model_ = nullptr;
  CpuLoadModel* cpu_load_model_ = nullptr;
  CpuStatModel* cpu_stat_model_ = nullptr;
  MemModel* mem_model_ = nullptr;
  NetModel* net_model_ = nullptr;

  QStackedLayout* stack_menu_ = nullptr;//这个是用来存储数据的，水平的窗口

  // std::unique_ptr<std::thread> thread_;
  // bool stop = false;
};
}  // namespace monitor
