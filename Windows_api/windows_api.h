/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-27 18:17:09
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-14 15:22:45
 * @FilePath: \DDCCI_Protocol\Windows_api\windows_api.h
 * @Description
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef windows_api_H
#define windows_api_H

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>
#include <Windows.h>
#include <string>
#include <vector>

/**
 * @brief 枚举系统中的所有物理监视器
 *
 * 调用此函数会在内部更新 physicalMonitors 容器，存储所有物理监视器的句柄信息。
 */
void enumerateMonitors();

/**
 * @brief 检测并打印当前物理监视器的信息
 *
 * 调用此函数会输出所有检测到的物理监视器的描述信息。
 */
void detectMonitors();

/**
 * @brief 获取指定显示器的能力字符串
 *
 * @param displayId 物理显示器的编号，从0开始
 * @return QString 能力字符串，若失败返回空字符串
 */
QString getMonitorCapabilities(size_t displayId);

/**
 * @brief 获取指定显示器的VCP特性值
 *
 * @param displayId 物理显示器的编号，从0开始
 * @param vcpCode 需要查询的VCP代码
 * @return QString VCP特性值的字符串表示，若失败返回空字符串
 */
QString getVcpFeature(size_t displayId, BYTE vcpCode);

/**
 * @brief 设置指定显示器的VCP特性值
 *
 * @param displayId 物理显示器的编号，从0开始
 * @param vcpCode 要设置的VCP代码
 * @param newValue 要设置的新值
 * @return bool 返回true表示成功，false表示失败
 */
bool setVcpFeature(size_t displayId, BYTE vcpCode, DWORD newValue);

/**
 * @brief 清理物理监视器资源
 *
 * 调用此函数会释放物理监视器的资源。
 */
void cleanupMonitors();

/**
 * @brief 解析命令
 *
 */
QString filterCapabilities(QString& input, const QString& startKey, const QString& endKey);

/**
 * @brief 查询
 *
 * 调用Windows api查询
 */
void Win_Api_Query(int display_num, int command);

/**
 * @brief 设置和查询
 *
 * 调用Windows api设置值然后查询
 */
void Win_Api_SetAndQuery(int display_num, int command, int sent_data);

/**
 * @brief 打印解析后的结果
 */
void printParsedData(const QMap<QString, QString> &data);

/**
 * @brief 获取显示器EDID
 */

#endif // windows_api_H
