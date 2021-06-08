#ifndef MYMODEL_H
#define MYMODEL_H

#define REALTIME_UPDATE

#include <QAbstractTableModel>
#include <QRect>

struct BombPara{
    float t;
    float x;
    float y;
    float z;
    int type;

    BombPara(float _t, float _x, float _y, float _z, int _type)
        : t(_t), x(_x),y(_y),z(_z),type(_type)
    {
    }
    BombPara(){
        x = 3.0 * static_cast <float> (rand()/ static_cast <float> (RAND_MAX));    //0~3.0
        y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));    //0~1.0
        z = 10.0 * static_cast <float> (rand() / static_cast <float> (RAND_MAX));//0~10.0
        type = rand() % 2;  //0或1
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

    bool setData(const QModelIndex &index, const QVariant &value, int role ) override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void addMapping(QString color, QRect area);
    void clearMapping() { m_mapping.clear(); }

private:
    QHash<QString, QRect> m_mapping;

    QStringList header;
    QVector<BombPara> para;
};

#endif // MYMODEL_H
