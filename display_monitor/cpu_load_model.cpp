#include "cpu_load_model.h"

namespace monitor {
CpuLoadModel::CpuLoadModel(QObject* parent) : MonitorInterModel(parent) {
  header_ << tr("load_1");//设置列表头的数据
  header_ << tr("load_3");
  header_ << tr("load_15");
}

//设置了行列的大小
int CpuLoadModel::rowCount(const QModelIndex& parent) const {
  return monitor_data_.size();
}

int CpuLoadModel::columnCount(const QModelIndex& parent) const {
  return COLUMN_MAX;
}
//写入这个头的名字
QVariant CpuLoadModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    return header_[section];//把列表头的数据写入到表格中，这样相应就能在表格中显示出来
  }

  return MonitorInterModel::headerData(section, orientation, role);
}

//设置表的数据
QVariant CpuLoadModel::data(const QModelIndex& index, int role) const {
  if (index.column() < 0 || index.column() >= COLUMN_MAX) {
    return QVariant();
  }

//实时刷新
  if (role == Qt::DisplayRole) {
    if (index.row() < monitor_data_.size() && index.column() < COLUMN_MAX)
      return monitor_data_[index.row()][index.column()];//获得表格中的数据
  }
  return MonitorInterModel::data(index, role);
}

//这里进行cpuload的刷新
void CpuLoadModel::UpdateMonitorInfo(
    const monitor::proto::MonitorInfo& monitor_info) {
  beginResetModel();//把之前的数据全部清空掉
  monitor_data_.clear();
  //传入的时monitorINFO的数据，我们就需要从里面获得一个cpu_load对象,读取里面的数据
  monitor_data_.push_back(insert_one_cpu_load(monitor_info.cpu_load()));//解析后放到数据的vector中

  // QModelIndex leftTop = createIndex(0, 0);
  // QModelIndex rightBottom = createIndex(monitor_data_.size(), COLUMN_MAX);
  // emit dataChanged(leftTop, rightBottom, {});

  endResetModel();

  return;
} 
//转化成QT的数据类型  
std::vector<QVariant> CpuLoadModel::insert_one_cpu_load(
    const monitor::proto::CpuLoad& cpu_load) {
  std::vector<QVariant> cpu_load_list;//一个vectoe中的数据
  for (int i = CpuLoad::CPU_AVG_1; i < COLUMN_MAX; i++) {
    switch (i) {
      case CpuLoad::CPU_AVG_1:
        cpu_load_list.push_back(QVariant(cpu_load.load_avg_1()));//获得cpu的1分钟的负载
        break;
      case CpuLoad::CPU_AVG_3:
        cpu_load_list.push_back(QVariant(cpu_load.load_avg_3()));//把里面的数据全部都放入的cpuload的list中
        break;
      case CpuLoad::CPU_AVG_15:
        cpu_load_list.push_back(QVariant(cpu_load.load_avg_15()));
        break;
      default:
        break;
    }
  }
  return cpu_load_list;
}
}  // namespace monitor
