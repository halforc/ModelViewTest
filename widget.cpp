#include "widget.h"
#include "ui_widget.h"
#include <memory>

#include <QDebug>
#include <QTimer>

namespace{
const int PORT = 8844;
const int msec_period = 1000;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , model(std::make_unique<MyModel>(this))
    , timer(std::make_unique<QTimer>(this))
    , uSocket(std::make_unique<QUdpSocket>(this))
    , mapper(std::make_unique<QDataWidgetMapper>(this))
{
    ui->setupUi(this);

    uSocket->bind(QHostAddress("127.0.0.1"), PORT);
    connect(uSocket.get(), &QUdpSocket::readyRead, this, &Widget::receive);

    mapper->setModel(model.get());
    mapper->setItemDelegate(ui->tableView->itemDelegate());
    mapper->addMapping(ui->elValX, 1);
    mapper->toFirst();
    //connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper.get(), SLOT(setCurrentModelIndex(QModelIndex)));

    connect(mapper.get(), &QDataWidgetMapper::currentIndexChanged,
            this, &Widget::on_pbAdd_clicked);
    ui->tableView->setModel(model.get());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbSend_clicked()
{
    static bool bRunning = false;
    if(!bRunning){
        QByteArray msg = "Hello world!";
        qDebug() << "--- Sender ---";

        QObject::connect(timer.get(), &QTimer::timeout, [&](){
            BombPara para;
            qus.writeDatagram((char*)&para, sizeof(para), QHostAddress("127.0.0.1"), PORT);
            qDebug() << "--- Sender ---";
        });
        timer->start(msec_period);
        ui->pbSend->setText("stop");
        bRunning = true;
    }else{
        timer->stop();
        ui->pbSend->setText("数据发送模拟");
        bRunning = false;
    }

}

void Widget::on_pbAdd_clicked()
{
    static int nCount = 0;
    model->add(BombPara());
#ifndef REALTIME_UPDATE
    if(++nCount % 5 == 0){
        model->reflashModel();
    }
#endif
}

void Widget::receive()
{
    QByteArray ba;

    static int count = 0;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        BombPara para;
        uSocket->readDatagram((char*)&para, sizeof(para));
        model->add(para);
#ifndef REALTIME_UPDATE
        if(++count % 5 == 0){
            model->reflashModel();
        }
#endif
        qDebug() << "--- receive ---";
    }
}

