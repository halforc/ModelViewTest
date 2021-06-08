#include "widget.h"
#include "ui_widget.h"
#include <memory>
#include <QTableView>
#include <QDebug>
#include <QTimer>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>
namespace{
const int PORT = 8844;
const int msec_period = 1000;
}

static float t = 0.7;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , model(std::make_unique<MyModel>(this))
    , timer(std::make_unique<QTimer>(this))
    , uSocket(std::make_unique<QUdpSocket>(this))
{
    ui->setupUi(this);

    uSocket->bind(QHostAddress("127.0.0.1"), PORT);
    connect(uSocket.get(), &QUdpSocket::readyRead, this, &Widget::receive);

    setTable();
    setMapper();
    setChartView();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setTable()
{
    ui->tableView->setModel(model.get());
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
}

void Widget::setChartView()
{
    //chart
    //! [3]
    QChart *chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    //! [3]

    // series 1
    //! [4]
    QLineSeries *series = new QLineSeries;
    series->setName("x");
    QVXYModelMapper *mapper = new QVXYModelMapper(this);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setSeries(series);
    mapper->setModel(model.get());
    chart->addSeries(series);
    //! [4]

    //! [5]
    // for storing color hex from the series
    QString seriesColorHex = "#000000";

    // get the color of the series and use it for showing the mapped area
    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));
    //! [5]

    // series 2
    //! [6]
    series = new QLineSeries;
    series->setName("y");

    mapper = new QVXYModelMapper(this);
    mapper->setXColumn(0);
    mapper->setYColumn(2);
    mapper->setSeries(series);
    mapper->setModel(model.get());
    chart->addSeries(series);
    //! [6]

    //! [7]
    // get the color of the series and use it for showing the mapped area
    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    model->addMapping(seriesColorHex, QRect(2, 0, 2, model->rowCount()));
    //! [7]

    //! [8]
    chart->createDefaultAxes();
    //![3]
    chart->axes(Qt::Horizontal).first()->setRange(0, 10);
    chart->axes(Qt::Vertical).first()->setRange(0, 10);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void Widget::setMapper()
{
    mapper = new QDataWidgetMapper(this);
    mapper->setOrientation(Qt::Horizontal);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //! [Set up the mapper]

    mapper->setModel(model.get());
    mapper->addMapping(ui->elValX, 0);
    mapper->addMapping(ui->elValY, 1);
    mapper->addMapping(ui->elValZ, 2);
    mapper->addMapping(ui->comboBox, 3);
    //! [Set up the mapper]
    connect(ui->tableView->selectionModel(),&QItemSelectionModel::currentRowChanged,
                    mapper,&QDataWidgetMapper::setCurrentModelIndex);
    connect(ui->previousButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->nextButton, &QPushButton::clicked,
            mapper, &QDataWidgetMapper::toNext);

    mapper->toFirst();
}

void Widget::on_pbSend_clicked()
{
    static bool bRunning = false;
    if(!bRunning){
        QObject::connect(timer.get(), &QTimer::timeout, [&](){
            BombPara para;
            para.t = t;
            t += 0.1;
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
    BombPara para;
    para.t = t;
    t += 0.1;
    model->add(para);
#ifndef REALTIME_UPDATE
    if(++nCount % 5 == 0){
        model->reflashModel();
    }
#endif
}

void Widget::receive()
{
    static int count = 0;
    while(uSocket->hasPendingDatagrams())
    {
        BombPara para;
        uSocket->readDatagram((char*)&para, sizeof(para));
        model->add(para);
        //ui->tableView->scrollToBottom();
        int rowSum = ui->tableView->model()->rowCount();//获取tableView总行数
        ui->tableView->selectRow(rowSum-1);//选中最后一行
#ifndef REALTIME_UPDATE
        if(++count % 5 == 0){
            model->reflashModel();
        }
#endif
        qDebug() << "--- receive ---";
    }
}

