#include "syswindow.h"
#include "ui_syswindow.h"
#include <QCloseEvent>
#include <QDebug>

syswindow::syswindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::syswindow)
{
    ui->setupUi(this);
}

syswindow::~syswindow()
{
    delete ui;
}

void syswindow::closeEvent(QCloseEvent *event)
{
    emit windowClosed(); // 发送信号
    event->accept(); // 处理关闭事件
}
