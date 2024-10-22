/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-28 17:04:16
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-21 19:56:37
 * @FilePath: \HikeenAT\Graphics_api.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef _Graphics_api_h
#define _Graphics_api_h

/* Intel API */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CTL_APIEXPORT // caller of control API DLL shall define this before
                      // including igcl_api.h
#include "GenericIGCLApp.h"
#include "igcl_api.h"

/* Nvidia API */
#include "nvapi.h"
#include "targetver.h"
#include <tchar.h>

/* AMD API */
#include "adl_defines.h"
#include "adl_sdk.h"
#include "mccs.h"
#include "user.h"
#include <cstring>
#include <iostream>
#include <windows.h>

#include <QByteArray>

enum
{
    NONE_API,
    Intel_API,
    Nvidia_API,
    AMD_API
};

#define MAX_VALUES 256 // 允许的最大数据个数

/*协议地址及标志位*/
#define DDCCI_master_addr 0x6E // 主机地址
#define DDCCI_slaver_addr 0X51 // 从机地址
#define DDCCI_cmd_status_flag 0X80

/* 主机命令标志位 */
#define DDCCI_CMD_query_data_flag 0x01
#define DDCCI_CMD_set_data_flag 0x04

/* 从机命令标志位 */
#define DDCCI_CMD_query_data_replay_flag 0x02 // 从机回复命令标志位

/* 命令类型 */
#define DDCCI_CMD_type_brightness 0x10
#define DDCCI_CMD_type_contrast 0x12

/* Intel API */
extern ctl_init_args_t CtlInitArgs;
extern ctl_api_handle_t hAPIHandle;

// 定义一个结构体来保存设备和显示器的上下文信息
typedef struct {
    ctl_device_adapter_handle_t *hDevices;      // 设备适配器句柄
    ctl_display_output_handle_t *hDisplayOutput; // 显示输出句柄
    uint32_t AdapterCount;                       // 适配器数量
    uint32_t DisplayCount;                       // 显示器数量
} DeviceContext;

bool Intel_Api_Init();
ctl_result_t I_I2C_Write(DeviceContext *context, QVector<int> Array);
ctl_result_t I_I2C_Query(DeviceContext *context, QVector<int> Array);
bool EnumerateDevicesAndDisplays(DeviceContext *context);
void CleanupDeviceContext(DeviceContext *context);

/* Nvidia API*/
extern NvU32 outputID;
extern NvPhysicalGpuHandle hGpu;
BOOL N_I2C_Write(NvPhysicalGpuHandle& hPhysicalGpu, NvU32& displayId, QVector<int> Array);
BOOL N_I2C_Query(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayId, QVector<int> Array);
int Nvidia_Get_Display(NvPhysicalGpuHandle& hPhysicalGpu, NvU32& displayId);

/* AMD API*/
extern int iAdapterIndex;
extern int iDisplayIndex;
extern ADLPROCS adlprocs;
bool InitADL(ADLPROCS& adlprocs);
void DeInitADL(ADLPROCS& adlprocs);
int iInitDisplayNames(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex);
int vWriteI2c(ADLPROCS& adlprocs, char *lpucSendMsgBuf, int iSendMsgLen, int iAdapterIndex, int iDisplayIndex);
int vWriteAndReadI2c(ADLPROCS& adlprocs, char *lpucSendMsgBuf, int iSendMsgLen, char *lpucRecvMsgBuf, int iRecvMsgLen, int iAdapterIndex,
                     int iDisplayIndex);
bool A_I2C_Write(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex, QVector<int> Array);
bool A_I2C_Query(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex, QVector<int> Array);

void __stdcall ADL_Main_Memory_Free(void **lpBuffer);
void *__stdcall ADL_Main_Memory_Alloc(int iSize);

#endif
