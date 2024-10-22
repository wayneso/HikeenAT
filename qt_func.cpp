/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-27 20:27:28
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-22 11:00:33
 * @FilePath: \HikeenAT\qt_func.cpp
 * @Description: 各种与Qt相关的功能实现
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QTextEdit>


#include "Graphics_api.h"
#include "user.h"


// 将日志消息附加到文本编辑器中
void appendLog(QTextEdit *textEdit, const QString &message, const QVector<int> &dataArray = QVector<int>())
{
    // 获取当前时间并格式化
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 创建带有时间戳的日志条目
    QString logEntry = QString("[%1] %2").arg(currentTime, message);

    // 如果有数据，附加数据日志到消息之后
    if (!dataArray.isEmpty())
    {
        QStringList stringList;
        for (int value : dataArray)
        {
            // 转换为两位十六进制格式
            stringList.append(QString::number(value, 16).toUpper().rightJustified(2, '0'));
        }
        // 将 QStringList 拼接成一个以空格分隔的字符串
        QString hexData = stringList.join(" ");
        logEntry += QString(" %2").arg(hexData);
    }

    // 使用 append 而不是 setText 以避免覆盖之前的内容
    textEdit->append(logEntry);
}

// 将字符串转换为十六进制整数并填充到 QVector 中
void QStrToQVec(QString str, QVector<int> &data)
{
    QStringList hexStrings = str.split(" ");
    QStringList filteredHexStrings;
    for (const QString &hexString : hexStrings)
    {
        if (!hexString.isEmpty())
        {
            filteredHexStrings.append(hexString);
        }
    }
    // 遍历字符串列表，将每个字符串转换为十六进制整数
    bool ok;
    for (const QString &hexString : filteredHexStrings)
    {
        int value = hexString.toInt(&ok, 16);
        if (ok)
        {
            data.append(value); // 将转换的值添加到 QVector
        }
        else
        {
            qDebug() << "Invalid hex value:" << hexString; // 无效的十六进制值
        }
    }
}

// 处理发送的 EDID 数据
void Sent_Data_Handle_EDID(QString sent_data_str, QVector<int> &sent_data_array)
{
    // 按空格分割字符串，手动过滤空字符串
    bool ok;
    int checksum = 0x00;
    QStringList hexStrings = sent_data_str.split(" ");
    QStringList filteredHexStrings;
    for (const QString &hexString : hexStrings)
    {
        if (!hexString.isEmpty())
        {
            filteredHexStrings.append(hexString);
        }
    }
    // 遍历字符串列表，将每个字符串转换为十六进制整数
    for (const QString &hexString : filteredHexStrings)
    {
        int value = hexString.toInt(&ok, 16);
        if (ok)
        {
            sent_data_array.append(value); // 添加到发送数据数组
        }
        else
        {
            qDebug() << "Invalid hex value:" << hexString; // 无效的十六进制值
        }
    }
}

// 处理发送的数据
void Sent_Data_Handle(QString sent_data_str, QVector<int> &sent_data_array)
{
    // 按空格分割字符串，手动过滤空字符串
    bool ok;
    int checksum = 0x00;
    QStringList hexStrings = sent_data_str.split(" ");
    QStringList filteredHexStrings;
    for (const QString &hexString : hexStrings)
    {
        if (!hexString.isEmpty())
        {
            filteredHexStrings.append(hexString);
        }
    }
    // 遍历字符串列表，将每个字符串转换为十六进制整数
    for (const QString &hexString : filteredHexStrings)
    {
        int value = hexString.toInt(&ok, 16);
        if (ok)
        {
            sent_data_array.append(value); // 添加到发送数据数组
        }
        else
        {
            qDebug() << "Invalid hex value:" << hexString; // 无效的十六进制值
        }
    }
    int dataLength = sent_data_array.size(); // 获取当前数据长度
    int all_dataLength = dataLength + 3; // 计算总数据长度
    dataLength ^= DDCCI_cmd_status_flag; // 进行数据长度操作
    sent_data_array.insert(0, master_addr); // 插入主设备地址
    sent_data_array.insert(1, slaver_addr); // 插入从设备地址
    sent_data_array.insert(2, (dataLength)); // 插入处理后的数据长度
    checksum = CalculateChecksum(&sent_data_array, all_dataLength, checktype_xor); // 计算校验和
    sent_data_array.append(checksum); // 添加校验和到数据数组
}
