#ifndef MYMODEL_H
#define MYMODEL_H

#define REALTIME_UPDATE

#include <QAbstractTableModel>

struct BombPara{
    float x;
    float y;
    float z;
    int type;
    float distentPercent;

    BombPara(float _x, float _y, float _z, int _type, float dis)
        : x(_x),y(_y),z(_z),type(_type),distentPercent(dis)
    {

    }
    BombPara(){
        x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.0));    //0~3.0
        y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));    //0~1.0
        z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0));//0~10.0
        type = rand() % 2;  //0或1
        distentPercent = rand() / RAND_MAX; //0~1
    }
};

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
//数据操作
public:
void add(const BombPara& val);

void reflashModel();
public:
    explicit MyModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    QStringList header;
    QVector<BombPara> para;
};

#endif // MYMODEL_H
