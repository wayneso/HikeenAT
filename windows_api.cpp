/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-27 18:16:53
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-09-30 14:42:58
 * @FilePath: \DDCCI_Protocol\windows_api.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QRegularExpression>
#include <QString>
#include <QVector>

#include "user.h"
#include <LowLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <Windows.h>

// 存储物理显示器的句柄
QVector<PHYSICAL_MONITOR> physicalMonitors;

// 枚举监视器的回调函数
BOOL CALLBACK monitorEnumProcCallback(HMONITOR hMonitor, HDC hDeviceContext, LPRECT rect, LPARAM data)
{
    DWORD numberOfPhysicalMonitors;
    BOOL success = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numberOfPhysicalMonitors);
    if (success)
    {
        int originalSize = physicalMonitors.size();
        physicalMonitors.resize(physicalMonitors.size() + numberOfPhysicalMonitors);
        success =
            GetPhysicalMonitorsFromHMONITOR(hMonitor, numberOfPhysicalMonitors, physicalMonitors.data() + originalSize);
    }
    return true;
}

// 将宽字符转换为UTF-8格式
QString toUtf8(const wchar_t *buffer)
{
    return QString::fromWCharArray(buffer);
}

// 枚举显示器，获取物理监视器信息
void enumerateMonitors()
{
    // 枚举显示器
    EnumDisplayMonitors(NULL, NULL, &monitorEnumProcCallback, 0);
}

// 打印物理监视器信息
void detectMonitors()
{
    int i = 0;
    for (const auto &physicalMonitor : physicalMonitors)
    {
        qDebug() << "显示器" << i << ":\t" << toUtf8(physicalMonitor.szPhysicalMonitorDescription);
        i++;
    }
}

// 查询监视器的能力
QString getMonitorCapabilities(size_t displayId)
{
    if (displayId >= physicalMonitors.size())
    {
        qDebug() << "无效的显示 ID";
        return QString();
    }

    auto physicalMonitorHandle = physicalMonitors[displayId].hPhysicalMonitor;
    DWORD capabilitiesStringLengthInCharacters;
    if (!GetCapabilitiesStringLength(physicalMonitorHandle, &capabilitiesStringLengthInCharacters))
    {
        qDebug() << "获取能力字符串长度失败";
        return QString();
    }

    std::unique_ptr<char[]> capabilitiesString(new char[capabilitiesStringLengthInCharacters]);
    if (!CapabilitiesRequestAndCapabilitiesReply(physicalMonitorHandle, capabilitiesString.get(),
                                                 capabilitiesStringLengthInCharacters))
    {
        qDebug() << "获取能力字符串失败";
        return QString();
    }

    return QString::fromUtf8(capabilitiesString.get());
}

// 处理能力字符串
QString filterCapabilities(QString& capabilities, const QString& startKey, const QString& endKey) {
    // 查找起始字符串的位置
    QString extracted;
    int startIndex = capabilities.indexOf(startKey);
    if (startIndex != -1) {
        // 从起始字符串的位置开始
        // 查找结束字符串的位置
        int endIndex = capabilities.indexOf(endKey, startIndex);
        if (endIndex != -1) {
            // 提取起始字符串与结束字符串之间的内容，包括起始字符串
            extracted = capabilities.mid(startIndex, endIndex - startIndex).trimmed();
            // 输出提取的内容
            // qDebug() << "Extracted:" << extracted;
        }
    }
    return extracted;
}

// 打印解析后的结果
void printParsedData(const QMap<QString, QString> &data)
{
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        qDebug() << it.key() << "=>" << it.value();
    }
}

// 获取指定监视器的VCP特性值
QString getVcpFeature(size_t displayId, BYTE vcpCode)
{
    if (displayId >= physicalMonitors.size())
    {
        qDebug() << "无效的显示 ID";
        return QString();
    }

    auto physicalMonitorHandle = physicalMonitors[displayId].hPhysicalMonitor;
    DWORD currentValue;
    if (!GetVCPFeatureAndVCPFeatureReply(physicalMonitorHandle, vcpCode, NULL, &currentValue, NULL))
    {
        qDebug() << "获取VCP代码值失败";
        return QString();
    }

    return QString("命令:0x%1 值:0x%2").arg(QString::number(vcpCode, 16)).arg(QString::number(currentValue, 16));
}

// 设置指定监视器的VCP特性值
bool setVcpFeature(size_t displayId, BYTE vcpCode, DWORD newValue)
{
    if (displayId >= physicalMonitors.size())
    {
        return false;
    }

    if (!SetVCPFeature(physicalMonitors[displayId].hPhysicalMonitor, vcpCode, newValue))
    {
        return false;
    }

    return true;
}

void Win_Api_Query(int display_num, int command)
{
    QString vcpValue = getVcpFeature(display_num, command);
    qDebug() << vcpValue;
}

void Win_Api_SetAndQuery(int display_num, int command, int sent_data)
{
    bool result = setVcpFeature(display_num, command, sent_data);
    if (result)
    {
        qDebug() << "设置成功";
    }
    else
    {
        qDebug() << "设置失败";
    }

    QString vcpValue = getVcpFeature(display_num, command);
    qDebug() << vcpValue;
}

// 清理物理监视器
void cleanupMonitors()
{
    DestroyPhysicalMonitors(physicalMonitors.size(), physicalMonitors.data());
    physicalMonitors.clear();
}
