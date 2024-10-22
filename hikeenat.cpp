#include "hikeenat.h"
#include "./ui_hikeenat.h"

#include "qt_func.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QVector>

DeviceContext Intel_API_Display = {0};

HikeenAT::HikeenAT(QWidget *parent) : QMainWindow(parent), ui(new Ui::HikeenAT)
{
    ui->setupUi(this);
}

HikeenAT::~HikeenAT()
{
    CleanupDeviceContext(&Intel_API_Display);
    delete ui;
}

void HikeenAT::on_link_pushButton_clicked(bool checked)
{
    if (checked) // 如果按键被按下
    {
        ui->link_pushButton->setText("断开连接"); // 修改按键文本
        Intel_API_Display = {0};                  // 重置Intel API上下文
        // 1. 调用Intel显卡API初始化
        if (Intel_Api_Init())
        {
            ctl_result_t Result = CTL_RESULT_SUCCESS; // 初始化返回结果变量
            qDebug() << "Intel API is ready";
            // 2. 枚举设备和显示输出
            if (EnumerateDevicesAndDisplays(&Intel_API_Display))
            {
                qDebug() << "EnumerateDevicesAndDisplays success";
                // 3. 更新ComboBox，显示适配器和显示设备数量
                ui->displaynum_comboBox->clear(); // 清空之前的内容
                for (uint32_t AdapterIndex = 0; AdapterIndex < Intel_API_Display.AdapterCount; AdapterIndex++)
                {
                    for (uint32_t DisplayIndex = 0; DisplayIndex < Intel_API_Display.DisplayCount; DisplayIndex++)
                    {
                        ctl_display_properties_t DisplayProperties = {0};
                        DisplayProperties.Size = sizeof(ctl_display_properties_t);
                        // 获取当前显示设备的属性
                        Result =
                            ctlGetDisplayProperties(Intel_API_Display.hDisplayOutput[DisplayIndex], &DisplayProperties);
                        // 检查显示设备是否已连接
                        bool IsDisplayAttached =
                            (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));
                        if (IsDisplayAttached) // 如果显示设备已连接
                        {
                            // 添加适配器和显示设备信息到ComboBox中
                            ui->displaynum_comboBox->addItem(QString("%1").arg(DisplayIndex));
                        }
                    }
                }
            }
            else
            {
                ui->link_pushButton->setChecked(false);
                QMessageBox::critical(this, "错误", "枚举设备失败", QMessageBox::Ok);
            }
        }
        else
        {
            ui->link_pushButton->setChecked(false);
            QMessageBox::critical(this, "错误", "初始化Intel API失败", QMessageBox::Ok);
        }
    }
    else // 如果按键被释放
    {
        ui->link_pushButton->setText("连接平台"); // 修改按键文本
        ui->displaynum_comboBox->clear();         // 清空ComboBox内容
        // 释放Intel API上下文中的资源
        CleanupDeviceContext(&Intel_API_Display);
    }
}

void HikeenAT::on_start_pushButton_clicked(bool checked)
{
    if (ui->link_pushButton->isChecked())
    {
        static QTimer *Sent_Data_timer = new QTimer(this);
        connect(Sent_Data_timer, &QTimer::timeout, this, &HikeenAT::Sent_Data_Timer_CallBack);

        if (checked)
        {
            ui->start_pushButton->setText("停止");
            Sent_Data_timer->start(500);
        }
        else // 按钮被再次按下
        {
            ui->start_pushButton->setText("开始");
            Sent_Data_timer->stop();
        }
    }
    else
    {
        QMessageBox::critical(this, "错误", "确认是否连接平台", QMessageBox::Ok);
    }
}

void HikeenAT::Sent_Data_Timer_CallBack()
{
    QVector<int> data = {0x6E, 0X51, 0X84, 0X03, 0X10, 0X00, 0X32, 0X9A};
    if (CTL_RESULT_SUCCESS == I_I2C_Write(&Intel_API_Display, data))
    {
        qDebug() << "Intel API Write success";
        appendLog(ui->informatio_textEdit,"");
        appendLog(ui->informatio_textEdit,"Intel API Write success");
        appendLog(ui->informatio_textEdit,"",data);
    }
}
