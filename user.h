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
#include <QVector>

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

#define MAX_VALUES 256 // 允许的最大数据个数

/*协议地址及标志位*/
#define DDCCI_Master_Addr 0x6E
#define DDCCI_Slaver_Addr 0X51
#define DDCCI_CMD_Status_Flag 0X80

/* 主机命令标志位 */
#define DDCCI_CMD_Query_Data_Flag 0x01
#define DDCCI_CMD_Set_Data_Flag 0x03

/* 从机命令标志位 */
#define DDCCI_CMD_Query_Qata_Replay_Flag 0x02 // 从机回复命令标志位

/* 命令类型 */
#define DDCCI_CMD_Type_Brightness 0x10
#define DDCCI_CMD_Type_Contrast 0x12

typedef struct {
    uint8_t masterAddr;
    uint8_t slaverAddr;
    uint8_t datalen;
    uint8_t commandtype;
    uint8_t command;
    QVector<int> data;
    uint8_t checksum;
} DDCCI_Frame;

bool Rece_Data_Check(int rece_data_arr[], int rece_data_len);
int CalculateChecksum(QVector<int> dataArray[], int length, int checksum_type);

uint8_t calculate_checksum(const DDCCI_Frame &frame);
QVector<int> SetData_Build_Frame(DDCCI_Frame &frame, uint8_t command, QVector<int> data = QVector<int>());
QVector<int> QueryData_Build_Frame(DDCCI_Frame &frame, uint8_t command, QVector<int> data = QVector<int>());

#endif // USER_H
