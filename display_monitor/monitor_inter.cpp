#include <QColor>
#include <QFont>

#include "monitor_inter.h"

namespace monitor {

//设置当前的表格的列名
QVariant MonitorInterModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (role == Qt::FontRole) {
    return QVariant::fromValue(QFont("Microsoft YaHei", 10, QFont::Bold));//设置当前的字体
  }

  if (role == Qt::BackgroundRole) {
    return QVariant::fromValue(QColor(Qt::lightGray));//设置子体的颜色是深灰
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}
//设置数据是在左边还是右边
QVariant MonitorInterModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::TextAlignmentRole) {
    return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
  }

  if (role == Qt::TextColorRole) {
    return QVariant::fromValue(QColor(Qt::black));
  }

  if (role == Qt::BackgroundRole) {
    return QVariant::fromValue(QColor(Qt::white));
  }

  return QVariant();
}

}  // namespace monitor