/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-10-21 18:04:11
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-21 18:04:35
 * @FilePath: \HikeenAT\user.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef USER_H
#define USER_H

#include <PhysicalMonitorEnumerationAPI.h>
#include <QByteArray>

extern int master_addr;
extern int slaver_addr;
extern int rece_data_arr[];
extern int rece_data_len;
extern int query_data_arr[];
extern int query_data_len;

extern QVector<PHYSICAL_MONITOR> physicalMonitors;

enum
{
    checktype_add,
    checktype_xor
};

typedef struct
{
    int *array;
    int length;
} data_array;

bool Rece_Data_Check(volatile int rece_data_arr[], int rece_data_len);
int CalculateChecksum(QVector<int> dataArray[], int length, int checksum_type);

#endif // USER_H
