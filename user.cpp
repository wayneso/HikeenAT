/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-28 17:07:52
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-09-29 14:02:41
 * @FilePath: \DDCCI_Protocol\user.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "Graphics_api.h"
#include <QByteArray>
#include <QVector>
#include "user.h"

int master_addr = 0x00;
int slaver_addr = 0x00;
int rece_data_arr[MAX_VALUES];
int query_data_arr[MAX_VALUES];
int query_data_len = 0;
int rece_data_len = 0;

bool Rece_Data_Check(volatile int rece_data_arr[], int rece_data_len){

    int checksum = 0x00;
    checksum = checksum ^ 0x50;//获取数据异或上0x50
    for (int var = 0; var < rece_data_len - 1; var++) {
        checksum = checksum ^ rece_data_arr[var];
    }
    if (checksum == rece_data_arr[rece_data_len - 1]){
        return true;
    }
    else
        return false;
}

int CalculateChecksum(QVector<int> dataArray[], int length, int checksum_type) {
    int checksum = 0x00;  // 初始化校验和为0x00

    for (int i = 0; i < length; i++) {  // 遍历数组
        switch (checksum_type) {
        case checktype_add:  // 加法校验类型
            checksum += dataArray->at(i);  // 累加每个数组元素
            break;
        case checktype_xor:  // 异或校验类型
            checksum ^= dataArray->at(i);  // 将每个数组元素进行异或操作
            break;
        default:  // 默认情况
            return -1;  // 返回一个错误代码表示无效的校验类型
        }
    }

    // 对于加法校验，处理超过255的情况
    if (checksum_type == checktype_add) {
        if (checksum > 255) {
            checksum = ~checksum + 1;  // 计算补码
        }
    }

    checksum &= 0xFF;  // 确保校验和在 0x00 到 0xFF 之间
    return checksum;  // 返回计算的校验和
}
