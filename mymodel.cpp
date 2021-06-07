#include "mymodel.h"
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
    header << "x" << "y" << "z" << "type" << "dis";
    para.push_back(BombPara(0.1, 10.1, 5.4, 0, 0.2));
    para.push_back(BombPara(0.2, 11.1, 6.4, 1, 0.3));
    para.push_back(BombPara(0.3, 12.1, 7.4, 0, 0.4));
    para.push_back(BombPara(0.4, 13.1, 8.4, 1, 0.6));
    para.push_back(BombPara(0.5, 14.1, 9.4, 1, 0.8));
    para.push_back(BombPara(0.6, 19.1, 11.4, 0, 1.0));
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
    if(role == Qt::DisplayRole){
        int row = index.row();
        int col = index.column();
        switch (col) {
        case 0:
            return QString("%1").arg(para.at(row).x, 4, 'g', 4);
        case 1:
            return QString("%1").arg(para.at(row).y, 4, 'g', 4);
        case 2:
            return QString("%1").arg(para.at(row).z, 4, 'g', 4);
        case 3:
            return QString("%1").arg(para.at(row).type);
        case 4:
            return QString("%1").arg(para.at(row).distentPercent, 4, 'g', 4);
        }
    }
    return QVariant();
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable; // FIXME: Implement me!
}
