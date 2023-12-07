#include "monitor_widget.h"

namespace monitor {

MonitorWidget::MonitorWidget(QWidget *parent) {}
//把所有的布局全部都融合在一起，并返回一个大的窗口
QWidget *MonitorWidget::ShowAllMonitorWidget(const std::string& name) {
  QWidget *widget = new QWidget();//这个是我们的要的大的窗口的布局
  //在这个地方对我们的成员变量进行初始化，一次只能看到一个的布局
  stack_menu_ = new QStackedLayout();//一次只能查看到一个页面的信息
  stack_menu_->addWidget(InitCpuMonitorWidget());//对这个大的窗口进行构造，放入一个小的表单窗口
  stack_menu_->addWidget(InitSoftIrqMonitorWidget());
  stack_menu_->addWidget(InitMemMonitorWidget());
  stack_menu_->addWidget(InitNetMonitorWidget());

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(InitButtonMenu(name), 1, 0);//把按钮的布局放在第一行，第0列
  layout->addLayout(stack_menu_, 2, 0);//把各种布局放在第二行，第0列
  widget->setLayout(layout);//最后把这个布局给到总的窗口中
  return widget;
}

//这个就是通过按某个按钮，就能切换某个布局
QWidget *MonitorWidget::InitButtonMenu(const std::string& name) {
  // char *name = getenv("USER");
  //创建了几个按钮
  QPushButton *cpu_button = new QPushButton(QString::fromStdString(name) + "_cpu", this);
  QPushButton *soft_irq_button = new QPushButton(QString::fromStdString(name) + "_soft_irq", this);
  QPushButton *mem_button = new QPushButton(QString::fromStdString(name) + "_mem", this);
  QPushButton *net_button = new QPushButton(QString::fromStdString(name) + "_net", this);
  QFont *font = new QFont("Microsoft YaHei", 15, 40);//并且设置了字体的格式
  //这几个的按钮的文本都设置成了同样的格式
  cpu_button->setFont(*font);
  soft_irq_button->setFont(*font);
  mem_button->setFont(*font);
  net_button->setFont(*font);
  //将按钮设置成了水平的布局
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(cpu_button);
  layout->addWidget(soft_irq_button);
  layout->addWidget(mem_button);
  layout->addWidget(net_button);
  //
  QWidget *widget = new QWidget();
  widget->setLayout(layout);
  //这里我们使用了信号和槽的机制，当我们点击某个按钮的时候，就会触发某个槽函数,会把相应的索引，进行设置
  connect(cpu_button, SIGNAL(clicked()), this, SLOT(ClickCpuButton()));
  connect(soft_irq_button, SIGNAL(clicked()), this, SLOT(ClickSoftIrqButton()));
  connect(mem_button, SIGNAL(clicked()), this, SLOT(ClickMemButton()));
  connect(net_button, SIGNAL(clicked()), this, SLOT(ClickNetButton()));

  return widget;
}

//CPU的监控模块，既可以是cpu的负载，也可以是cpu的各种统计信息使用率
QWidget *MonitorWidget::InitCpuMonitorWidget() {
  QWidget *widget = new QWidget();

  QLabel *cpu_load_label = new QLabel(this);//这个是cpu的负载信息
  cpu_load_label->setText(tr("Monitor CpuLoad:"));//添加当前窗口的文本内容
  cpu_load_label->setFont(QFont("Microsoft YaHei", 10, 40));//

  cpu_load_monitor_view_ = new QTableView;
  cpu_load_model_ = new CpuLoadModel;
  cpu_load_monitor_view_->setModel(cpu_load_model_);
  cpu_load_monitor_view_->show();

  QLabel *cpu_stat_label = new QLabel(this);//cpu的统计信息
  cpu_stat_label->setText(tr("Monitor CpuStat:"));
  cpu_stat_label->setFont(QFont("Microsoft YaHei", 10, 40));

  cpu_stat_monitor_view_ = new QTableView;
  cpu_stat_model_ = new CpuStatModel;//构造一个数据模型
  cpu_stat_monitor_view_->setModel(cpu_stat_model_);//把当前数据模型中的数据插入到当前的表格中
  cpu_stat_monitor_view_->show();

  QGridLayout *layout = new QGridLayout();

  layout->addWidget(cpu_load_label, 3, 0);//把负载文本放在第三行，第0列
  layout->addWidget(cpu_load_monitor_view_, 4, 0, 2, 2);//把负载的数据放在第四行，第0列，占用两行两列

  layout->addWidget(cpu_stat_label, 1, 0, 1, 1);//把统计数据的文本放在第一行，第0列，占用一行一列
  layout->addWidget(cpu_stat_monitor_view_, 2, 0, 1, 2);//把cpu的统计数据放在第二行，第0列，占用一行两列

  widget->setLayout(layout);
  return widget;
}

QWidget *MonitorWidget::InitSoftIrqMonitorWidget() {
  QWidget *widget = new QWidget();
  QLabel *monitor_label = new QLabel(this);//构造一个文本
  monitor_label->setText(tr("Monitor softirq:"));//给当前的文本添加一些内容
  monitor_label->setFont(QFont("Microsoft YaHei", 10, 40));//设置当前文本的子体格式

  monitor_view_ = new QTableView;//创建一个表单
  monitor_model_ = new MonitorBaseModel;//创建一个数据模型，需要往里面填充数据
  QSortFilterProxyModel *sort_proxy = new QSortFilterProxyModel(this);//这个可以设置排序的功能，把这个数据模型设置成功可排序的数据模型
  sort_proxy->setSourceModel(monitor_model_);
  monitor_view_->setModel(sort_proxy);
  monitor_view_->setSortingEnabled(true);
  monitor_view_->show();

  QGridLayout *layout = new QGridLayout();//创建一个布局的对象
  layout->addWidget(monitor_label, 1, 0);//把表单放入到这个表中的第一行，第0列
  layout->addWidget(monitor_view_, 2, 0, 1, 2);//把表单放入到这个表中的第二行，第0列，占用一行两列

  widget->setLayout(layout);
  return widget;
}

QWidget *MonitorWidget::InitMemMonitorWidget() {
  QWidget *widget = new QWidget();//初始化一个窗口，

  QLabel *mem_label = new QLabel(this);
  mem_label->setText(tr("Monitor mem:"));//添加当前窗口的文本内容
  mem_label->setFont(QFont("Microsoft YaHei", 10, 40));//设置当前文本的大小

//生成一个表格
  mem_monitor_view_ = new QTableView;
  mem_model_ = new MemModel;
  mem_monitor_view_->setModel(mem_model_);//把数据填充到表格中
  mem_monitor_view_->show();//展示

  QGridLayout *layout = new QGridLayout();//进行对数据的布局

  layout->addWidget(mem_label, 1, 0);//把文本内容放到第一行，第0列
  layout->addWidget(mem_monitor_view_, 2, 0, 1, 1);//把数据的内容放到，第二行，第0列，占用一行一列
  //再把数据放到显示的窗口函数中
  widget->setLayout(layout);
  return widget;
}

QWidget *MonitorWidget::InitNetMonitorWidget() {
  QWidget *widget = new QWidget();

  QLabel *net_label = new QLabel(this);
  net_label->setText(tr("Monitor net:"));//再把树就放到表格中
  net_label->setFont(QFont("Microsoft YaHei", 10, 40));

  net_monitor_view_ = new QTableView;
  net_model_ = new NetModel;
  net_monitor_view_->setModel(net_model_);
  net_monitor_view_->show();

  QGridLayout *layout = new QGridLayout();

  layout->addWidget(net_label, 1, 0);
  layout->addWidget(net_monitor_view_, 2, 0, 1, 1);

  widget->setLayout(layout);
  return widget;
}
//刷新数据
void MonitorWidget::UpdateData(
    const monitor::proto::MonitorInfo &monitor_info) {
      //一次去调用底层的各种的类型，更新模型的数据
  monitor_model_->UpdateMonitorInfo(monitor_info);
  cpu_load_model_->UpdateMonitorInfo(monitor_info);
  cpu_stat_model_->UpdateMonitorInfo(monitor_info);
  mem_model_->UpdateMonitorInfo(monitor_info);
  net_model_->UpdateMonitorInfo(monitor_info);
}

//触发按钮，就切换到了某个具体的小窗口
void MonitorWidget::ClickCpuButton() { stack_menu_->setCurrentIndex(0); }
void MonitorWidget::ClickSoftIrqButton() { stack_menu_->setCurrentIndex(1); }
void MonitorWidget::ClickMemButton() { stack_menu_->setCurrentIndex(2); }
void MonitorWidget::ClickNetButton() { stack_menu_->setCurrentIndex(3); }
}  // namespace monitor
