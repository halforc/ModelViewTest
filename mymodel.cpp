#include "mymodel.h"
#include <QColor>

int nPre = 0;
void MyModel::add(const BombPara &val)
{
     //每条都更新
#ifdef REALTIME_UPDATE
    int n = para.size();
    beginInsertRows(QModelIndex(), n, n);
    para.insert(n, val);
    endInsertRows();
#else
    //累计更新
    para.push_back(val);
#endif
}

void MyModel::reflashModel()
{
    #ifndef REALTIME_UPDATE
    //插入一定数量的数据后统一更新视图，如5条
    int n = para.size();
    beginInsertRows(QModelIndex(), nPre, n-1);
    endInsertRows();
    nPre = n;
    #endif
}

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    header << "t" << "x" << "y" << "z" << "type" << "dis";
    para.push_back(BombPara(0.1, 1.1, 1.4, 0.2, 0));
    para.push_back(BombPara(0.2, 1.5, 3.4, 0.3, 1));
    para.push_back(BombPara(0.3, 2.1, 5.4, 0.4, 1));
    para.push_back(BombPara(0.4, 3.1, 6.4, 0.6, 0));
    para.push_back(BombPara(0.5, 4.1, 3.4, 0.8, 1));
    para.push_back(BombPara(0.6, 9.1, 2.4, 1.0, 0));
    nPre = para.size();
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if(role != Qt::DisplayRole) return QVariant();
    if(orientation == Qt::Horizontal)   return header.at(section);
    return QVariant();
}

bool MyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!

        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int MyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return para.size();
}

int MyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return header.size();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
       // Q_REQUIRED_RESULT QString arg(double a, int fieldWidth = 0, char fmt = 'g', int prec = -1,
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        int row = index.row();
        int col = index.column();
        switch (col) {
        case 0:
            return QString("%1").arg(para.at(row).t, 4, 'g', 4);
        case 1:
            return QString("%1").arg(para.at(row).x, 4, 'g', 4);
        case 2:
            return QString("%1").arg(para.at(row).y, 4, 'g', 4);
        case 3:
            return QString("%1").arg(para.at(row).z, 4, 'g', 4);
        case 4:
            return QString("%1").arg(para.at(row).type);
        }
    }else if (role == Qt::BackgroundRole) {
        for (const QRect &rect : m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole){
            int row = index.row();
            // contacts is a c++ struct
            auto val = para.at(row);
            if (index.column() == 0)
                  val.t = value.toFloat();
            else if (index.column() == 1)
                  val.x = value.toFloat();
            else if (index.column() == 2)
                  val.y = value.toFloat();
            else if (index.column() == 3)
                  val.z = value.toFloat();
            else
                  return false;

            para.replace(row, val);
            emit dataChanged(index,index,{role});
            return true;
        }

        return false;
}


bool MyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool MyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool MyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool MyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}

void MyModel::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}
