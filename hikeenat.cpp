#include "hikeenat.h"
#include "./ui_hikeenat.h"

#include <QDebug>
HikeenAT::HikeenAT(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HikeenAT)
{
    ui->setupUi(this);
}

HikeenAT::~HikeenAT()
{
    // 释放Intel API上下文中的资源
    CleanupDeviceContext(&Intel_API_Display);
    delete ui;
}

void HikeenAT::on_link_pushButton_clicked()
{
    // 1. 调用Intel显卡API初始化
    if (Intel_Api_Init())
    {
        qDebug() << "Intel API is ready";

        // 2. 枚举设备及显示器
        if (EnumerateDevicesAndDisplays(&Intel_API_Display))
        {
            qDebug() << "EnumerateDevicesAndDisplays success";
            QVector<int> data = {0x6E, 0X51, 0X84, 0X03, 0X10, 0X00, 0X32, 0X9A};
            if (I_I2C_Write(&Intel_API_Display, data))
            {
                qDebug() << "Intel API Write success";
            }


        }
    }
}

