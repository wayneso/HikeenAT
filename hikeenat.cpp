#include "hikeenat.h"
#include "./ui_hikeenat.h"

#include "qt_func.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QVector>
#include <QClipboard>

Data_Count Sent_Data_Count = {0};
Data_Count Query_Data_Count = {0};

uint API = NONE_API;
DeviceContext Intel_API_Display = {0};
ADLPROCS adlprocs = {0, 0, 0, 0};

HikeenAT::HikeenAT(QWidget *parent) : QMainWindow(parent), ui(new Ui::HikeenAT)
{
    ui->setupUi(this);
    ui->start_pushButton->setEnabled(false);
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
        ui->start_pushButton->setEnabled(true);
        Sent_Data_Count = {0};
        Query_Data_Count = {0};
        ui->link_pushButton->setText("断开连接");
        Intel_API_Display = {0};
        adlprocs = {0, 0, 0, 0};
        NvAPI_Status nvapiStatus = NVAPI_ERROR;

        if (Intel_Api_Init())
        {
            ui->api_comboBox->addItem("Intel_API");
            ctl_result_t Result = CTL_RESULT_SUCCESS;

            if (EnumerateDevicesAndDisplays(&Intel_API_Display))
            {
                ui->displaynum_comboBox->clear();
                for (uint32_t AdapterIndex = 0; AdapterIndex < Intel_API_Display.AdapterCount; AdapterIndex++)
                {
                    for (uint32_t DisplayIndex = 0; DisplayIndex < Intel_API_Display.DisplayCount; DisplayIndex++)
                    {
                        ctl_display_properties_t DisplayProperties = {0};
                        DisplayProperties.Size = sizeof(ctl_display_properties_t);
                        Result =
                            ctlGetDisplayProperties(Intel_API_Display.hDisplayOutput[DisplayIndex], &DisplayProperties);
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
        else if (InitADL(adlprocs))
        {
            ui->api_comboBox->addItem("AMD_API");

            if(int iNumberAdapters = iInitDisplayNames(adlprocs, iAdapterIndex, iDisplayIndex) != 0)
            {
                ui->displaynum_comboBox->addItem(QString("%1").arg(iDisplayIndex));
            }
            else
            {
                ui->link_pushButton->setChecked(false);
                QMessageBox::critical(this, "错误", "枚举设备失败", QMessageBox::Ok);
            }
        }
        else if ((nvapiStatus = NvAPI_Initialize()) == NVAPI_OK)
        {
            ui->api_comboBox->addItem("Nvidia_API");
            if(Nvidia_Get_Display(hGpu, outputID))
            {
                ui->displaynum_comboBox->addItem(QString("%1").arg(outputID));
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
            QMessageBox::critical(this, "错误", "初始化显卡API失败", QMessageBox::Ok);
        }
    }
    else // 如果按键被释放
    {
        ui->link_pushButton->setText("连接平台");
        ui->displaynum_comboBox->clear();
        ui->api_comboBox->clear();
        QString query_count_label_text = "查询次数：0|0|0";
        ui->query_count_label->setText(query_count_label_text);
        QString sent_count_label_text = "发送次数：0|0|0";
        ui->sent_count_label->setText(sent_count_label_text);
        ui->start_pushButton->setEnabled(false);
        CleanupDeviceContext(&Intel_API_Display);
        DeInitADL(adlprocs);
    }
}

void HikeenAT::on_start_pushButton_clicked(bool checked)
{
    if (ui->link_pushButton->isChecked())
    {
        ui->link_pushButton->setEnabled(false);
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
    if(!checked)
    {
        ui->link_pushButton->setEnabled(true);
    }
}

void HikeenAT::Sent_Data_Timer_CallBack()
{
    appendLog(ui->informatio_textEdit,"");
    QVector<int> data = {0x6E, 0X51, 0X84, 0X03, 0X10, 0X00, 0X32, 0X9A};
    QVector<int> query_data = {0x6E, 0X51, 0X82, 0X01, 0X10, 0XAC};
    QVector<int> rece_data_array;
    rece_data_len = 11;
    Sent_Data_Count.total++;
    QString currentValue = ui->api_comboBox->currentText();
    if (API == Intel_API)
    {
        if (CTL_RESULT_SUCCESS == I_I2C_Write(&Intel_API_Display, data))
        {
            Sent_Data_Count.success++;
            appendLog(ui->informatio_textEdit,"Intel API Write success");

        }
        else
            Sent_Data_Count.fail++;
        Sleep(100);
        if (CTL_RESULT_SUCCESS == I_I2C_Query(&Intel_API_Display, query_data))
        {
            Query_Data_Count.total++;
            appendLog(ui->informatio_textEdit,"Intel API Query success");
        }
    }

    else if (API == Nvidia_API)
    {
        if (N_I2C_Write(hGpu, outputID, data))
        {
            Sent_Data_Count.success++;
            appendLog(ui->informatio_textEdit,"Nvidia API Write success");
        }
        else
            Sent_Data_Count.fail++;
        Sleep(100);
        if (N_I2C_Query(hGpu, outputID, query_data))
        {
            Query_Data_Count.total++;
            appendLog(ui->informatio_textEdit,"Nvidia API Query success");
        }
    }

    else if (API == AMD_API)
    {
        if (A_I2C_Write(adlprocs, iAdapterIndex, iDisplayIndex, data))
        {
            Sent_Data_Count.success++;
            appendLog(ui->informatio_textEdit,"AMD API Write success");
        }
        else
            Sent_Data_Count.fail++;
        Sleep(100);
        if (A_I2C_Query(adlprocs, iAdapterIndex, iDisplayIndex, query_data))
        {
            Query_Data_Count.total++;
            appendLog(ui->informatio_textEdit,"AMD API Query success");
        }
    }

    for (int i = 0; i < rece_data_len; i++)
    {
        rece_data_array.append(rece_data_arr[i]);
    }
    if(Rece_Data_Check(rece_data_arr,rece_data_len))
        Query_Data_Count.success++;
    else
        Query_Data_Count.fail++;

    QString query_count_label_text = "查询次数：" + QString::number(Query_Data_Count.total) + "|" + QString::number(Query_Data_Count.success) + "|" + QString::number(Query_Data_Count.fail);
    ui->query_count_label->setText(query_count_label_text);
    QString sent_count_label_text = "发送次数：" + QString::number(Sent_Data_Count.total) + "|" + QString::number(Sent_Data_Count.success) + "|" + QString::number(Sent_Data_Count.fail);
    ui->sent_count_label->setText(sent_count_label_text);

    appendLog(ui->informatio_textEdit,"",data);
    appendLog(ui->informatio_textEdit,"",query_data);
    appendLog(ui->informatio_textEdit,"",rece_data_array);
}

void HikeenAT::on_api_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case Intel_API:
        API = Intel_API;
        appendLog(ui->informatio_textEdit,"Intel API is ready");
        break;
    case AMD_API:
        API = AMD_API;
        appendLog(ui->informatio_textEdit,"AMD API is ready");
        break;
    case Nvidia_API:
        API = Nvidia_API;
        appendLog(ui->informatio_textEdit,"Nvidia API is ready");
        break;
    default:
        break;
    }
}


void HikeenAT::on_clear_pushButton_clicked()
{
    ui->informatio_textEdit->clear();
}


void HikeenAT::on_copy_pushButton_clicked()
{
    QString text = ui->informatio_textEdit->toPlainText();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

