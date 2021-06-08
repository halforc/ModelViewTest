#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QUdpSocket>
#include <QDataWidgetMapper>
#include "mymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void setTable();
    void setChartView();
    void setMapper();
protected slots:
    void on_pbSend_clicked();
    void on_pbAdd_clicked();
private:
    Ui::Widget *ui;

    QUdpSocket qus;
    std::unique_ptr<MyModel> model;
    std::unique_ptr<QTimer> timer;

    //UDP接收
public slots:
    void receive();
private:
    std::unique_ptr<QUdpSocket> uSocket;

    QDataWidgetMapper* mapper;

};
#endif // WIDGET_H
