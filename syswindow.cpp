#include "syswindow.h"
#include "ui_syswindow.h"
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

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

void syswindow::on_pushButton_clicked()
{
    // 打开文件对话框选择JSON文件
    QString filePath = QFileDialog::getOpenFileName(this, "选择JSON文件", "", "JSON Files (*.json);;All Files (*)");

}

