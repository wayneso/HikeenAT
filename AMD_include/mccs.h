/*
 * @Author: DESKTOP-LF49CEE\Wayne 2546850503@qq.com
 * @Date: 2024-09-02 16:08:49
 * @LastEditors: DESKTOP-LF49CEE\Wayne 2546850503@qq.com
 * @LastEditTime: 2024-09-03 17:25:59
 * @FilePath: \Auto_ddcci\HK_DDCCI_Protocol\AMD_include\mccs.h
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
// 定义一系列函数指针类型，用于调用不同的 ADL (AMD Display Library) 函数
#include <Windows.h>

typedef int (*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (*ADL_MAIN_CONTROL_DESTROY)();
typedef int (*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int *);
typedef int (*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);
typedef int (*ADL_DISPLAY_DISPLAYINFO_GET)(int, int *, ADLDisplayInfo **, int);
typedef int (*ADL_DISPLAY_DDCBLOCKACCESSGET)(int iAdapterIndex, int iDisplayIndex, int iOption, int iCommandIndex, int iSendMsgLen, char *lpucSendMsgBuf, int *lpulRecvMsgLen, char *lpucRecvMsgBuf);

// 定义一个结构体，用于存储各种 ADL 函数的函数指针以及模块句柄
typedef struct _ADLPROCS
{
    HMODULE hModule;                                     // 模块句柄
    ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create;     // 创建控制函数
    ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy;   // 销毁控制函数
    ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get;  // 获取适配器数量函数
    ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get;            // 获取适配器信息函数
    ADL_DISPLAY_DDCBLOCKACCESSGET ADL_Display_DDCBlockAccess_Get;       // 获取DDC块访问函数
    ADL_DISPLAY_DISPLAYINFO_GET ADL_Display_DisplayInfo_Get;            // 获取显示信息函数
} ADLPROCS;



// 定义一系列宏，表示各种数据传输或响应的大小、偏移量等

#define MAX_NUM_DISPLAY_DEVICES             16


#define SETWRITESIZE 8                         // 设置写入数据大小
#define GETRQWRITESIZE 6                       // 获取请求写入数据大小
#define GETCAPWRITESIZE 7                      // 获取能力写入数据大小
#define GETREPLYWRITESIZE 1                    // 获取回复写入数据大小
#define GETREPLYREADSIZE 11                    // 获取回复读取数据大小
#define GETREPLYCAPSIZEFIXED 38                // 获取固定长度能力回复数据大小
#define GETREPLYCAPSIZEVARIABLE 6              // 获取可变长度能力回复数据大小
#define MAXREADSIZE 131                        // 最大读取数据大小

#define SET_VCPCODE_OFFSET 4                   // 设置 VCP 代码的偏移量
#define SET_HIGH_OFFSET 5                      // 设置高字节偏移量
#define SET_LOW_OFFSET 6                       // 设置低字节偏移量
#define SET_CHK_OFFSET 7                       // 设置校验码偏移量

#define GETRQ_VCPCODE_OFFSET 4                 // 获取请求 VCP 代码的偏移量
#define GETRQ_CHK_OFFSET 5                     // 获取请求校验码的偏移量

#define GETRP_LENGHTH_OFFSET 1                 // 获取回复数据长度的偏移量
#define GETRP_MAXHIGH_OFFSET 6                 // 获取回复数据最大值高字节的偏移量
#define GETRP_MAXLOW_OFFSET 7                  // 获取回复数据最大值低字节的偏移量
#define GETRP_CURHIGH_OFFSET 8                 // 获取回复数据当前值高字节的偏移量
#define GETRP_CURLOW_OFFSET 9                  // 获取回复数据当前值低字节的偏移量

#define CAP_HIGH_OFFSET 4                      // 能力数据高字节偏移量
#define CAP_LOW_OFFSET 5                       // 能力数据低字节偏移量
#define CAP_CHK_OFFSET 6                       // 能力数据校验码偏移量



// 定义一些常用的 VCP 代码，用于 DDC/CI 通信协议
#define VCP_CODE_BRIGHTNESS 0x10               // 亮度 VCP 代码
#define VCP_CODE_CONTRAST 0x12                 // 对比度 VCP 代码
#define VCP_CODE_COLORTEMP 0x14                // 色温 VCP 代码
#define VCP_CODE_CAPABILITIES 0xF3             // 能力 VCP 代码
#define VCP_CODE_CAPABILITIES_NEW 0xF4         // 新的能力 VCP 代码

