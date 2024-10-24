/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-28 17:12:42
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-21 20:05:54
 * @FilePath: \HikeenAT\Intel_api.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "Graphics_api.h"
#include "user.h"
#include <QByteArray>
#include <QStringList>
#include <QVector>

ctl_init_args_t CtlInitArgs;
ctl_api_handle_t hAPIHandle;

/**
 * @description: Intel显卡API初始化
 * @return {*}
 */
bool Intel_Api_Init()
{
    ctl_result_t Result = CTL_RESULT_SUCCESS;

    ZeroMemory(&CtlInitArgs, sizeof(ctl_init_args_t));
    CtlInitArgs.AppVersion = CTL_MAKE_VERSION(CTL_IMPL_MAJOR_VERSION, CTL_IMPL_MINOR_VERSION);
    CtlInitArgs.flags = 0;
    CtlInitArgs.Size = sizeof(CtlInitArgs);
    CtlInitArgs.Version = 0;
    Result = ctlInit(&CtlInitArgs, &hAPIHandle);
    if (Result == CTL_RESULT_SUCCESS)
    {
        return true;
    }
    else
        return false;
}
/**
 * @description: 枚举系统中的设备适配器及其显示输出设备
 * @param {DeviceContext*} context 设备上下文结构体，用于保存枚举结果
 * @return {bool} 返回true表示枚举成功，false表示失败
 */
bool EnumerateDevicesAndDisplays(DeviceContext *context)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS; // 初始化返回结果变量

    // 枚举设备并获取适配器数量
    Result = ctlEnumerateDevices(hAPIHandle, &context->AdapterCount, context->hDevices);
    if (Result != CTL_RESULT_SUCCESS)
        return false; // 若失败，返回false

    // 为适配器句柄分配内存
    context->hDevices =
        (ctl_device_adapter_handle_t *)malloc(sizeof(ctl_device_adapter_handle_t) * context->AdapterCount);

    // 再次枚举设备，获取实际的适配器句柄
    Result = ctlEnumerateDevices(hAPIHandle, &context->AdapterCount, context->hDevices);
    if (Result != CTL_RESULT_SUCCESS)
        return false; // 若失败，返回false

    for (uint32_t AdapterIndex = 0; AdapterIndex < context->AdapterCount; AdapterIndex++)
    {
        context->DisplayCount = 0;

        // 枚举当前适配器的显示输出设备并获取显示设备数量
        Result = ctlEnumerateDisplayOutputs(context->hDevices[AdapterIndex], &context->DisplayCount,
                                            context->hDisplayOutput);
        if (Result != CTL_RESULT_SUCCESS)
            return false; // 若失败，返回false

        // 为显示输出句柄分配内存
        context->hDisplayOutput =
            (ctl_display_output_handle_t *)malloc(sizeof(ctl_display_output_handle_t) * context->DisplayCount);

        // 再次枚举显示输出设备，获取实际的显示输出句柄
        Result = ctlEnumerateDisplayOutputs(context->hDevices[AdapterIndex], &context->DisplayCount,
                                            context->hDisplayOutput);
        if (Result != CTL_RESULT_SUCCESS)
            return false; // 若失败，返回false
    }

    return true;
}
/**
 * @description: I_I2C_Write
 * @param {DeviceContext*} context 设备上下文结构体，用于保存枚举结果
 * @param {QVector<int>} Array 需要写入的数据
 * @return ctl_result_t 返回I2C传输的结果
 */
ctl_result_t I_I2C_Write(DeviceContext *context, QVector<int> Array)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS; // 初始化返回结果变量

    for (uint32_t AdapterIndex = 0; AdapterIndex < context->AdapterCount; AdapterIndex++)
    {
        for (uint32_t DisplayIndex = 0; DisplayIndex < context->DisplayCount; DisplayIndex++)
        {
            ctl_display_properties_t DisplayProperties = {0};
            DisplayProperties.Size = sizeof(ctl_display_properties_t);

            // 获取当前显示设备的属性
            Result = ctlGetDisplayProperties(context->hDisplayOutput[DisplayIndex], &DisplayProperties);
            if (Result != CTL_RESULT_SUCCESS)
                return Result;

            // 检查显示设备是否已连接
            bool IsDisplayAttached = (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));

            if (IsDisplayAttached) // 如果显示设备已连接
            {
                ctl_i2c_access_args_t I2CArgs = {0};
                int len = Array.at(2); // 获取长度信息
                Array.remove(0, 2);    // 移除数组中不需要的元素
                len ^= DDCCI_CMD_Status_Flag; // 对长度进行操作
                I2CArgs.Size = sizeof(ctl_i2c_access_args_t);
                I2CArgs.OpType = CTL_OPERATION_TYPE_WRITE; // 设定为写操作
                I2CArgs.Address = DDCCI_Master_Addr;       // 主设备地址
                I2CArgs.Offset = DDCCI_Slaver_Addr;        // 从设备地址
                I2CArgs.DataSize = len + 2;                // 数据大小

                // 填充I2C数据
                for (int i = 0; i <= len; i++)
                {
                    I2CArgs.Data[i] = Array[i];
                }

                // 执行I2C写操作
                Result = ctlI2CAccess(context->hDisplayOutput[DisplayIndex], &I2CArgs);

                if (Result != CTL_RESULT_SUCCESS)
                {
                    printf("ctlI2CAccess for I2C write returned failure code: 0x%X\n", Result);
                    return Result;
                }

                ZeroMemory(&I2CArgs, sizeof(I2CArgs)); // 清零I2C参数
            }
        }
    }

    return Result;
}

/**
 * @description: I_I2C_Query
 * @param {ctl_display_output_handle_t} hDisplayOutput
 * @param {QVector<int>} Array
 * @return {*}
 */
ctl_result_t I_I2C_Query(DeviceContext *context, QVector<int> Array)
{
    ctl_result_t Result = CTL_RESULT_SUCCESS; // 初始化返回结果变量

    for (uint32_t AdapterIndex = 0; AdapterIndex < context->AdapterCount; AdapterIndex++)
    {
        for (uint32_t DisplayIndex = 0; DisplayIndex < context->DisplayCount; DisplayIndex++)
        {
            ctl_display_properties_t DisplayProperties = {0};
            DisplayProperties.Size = sizeof(ctl_display_properties_t);

            // 获取当前显示设备的属性
            Result = ctlGetDisplayProperties(context->hDisplayOutput[DisplayIndex], &DisplayProperties);
            if (Result != CTL_RESULT_SUCCESS)
                return Result;

            // 检查显示设备是否已连接
            bool IsDisplayAttached =
                (0 != (DisplayProperties.DisplayConfigFlags & CTL_DISPLAY_CONFIG_FLAG_DISPLAY_ATTACHED));

            if (IsDisplayAttached) // 如果显示设备已连接
            {
                ctl_i2c_access_args_t I2CArgs = {0};
                int len;
                len = Array.at(2);
                // 从 QVector<int> 中删除前 3 个元素
                Array.remove(0, 2);

                len ^= DDCCI_CMD_Status_Flag;
                I2CArgs.Size = sizeof(ctl_i2c_access_args_t);
                I2CArgs.OpType = CTL_OPERATION_TYPE_WRITE; // 发送
                I2CArgs.Address = DDCCI_Master_Addr;
                I2CArgs.Offset = DDCCI_Slaver_Addr;
                I2CArgs.DataSize = len + 2;

                for (size_t i = 0; i <= len; i++)
                {
                    I2CArgs.Data[i] = Array[i];
                }

                Result = ctlI2CAccess(context->hDisplayOutput[DisplayIndex], &I2CArgs);

                if (CTL_RESULT_SUCCESS != Result)
                {
                    printf("ctlI2CAccess for I2C write returned failure code: 0x%X\n", Result);
                }
                ZeroMemory(&I2CArgs, sizeof(I2CArgs));

                // 接收回复报文
                I2CArgs.Size = sizeof(ctl_i2c_access_args_t);
                I2CArgs.OpType = CTL_OPERATION_TYPE_READ;
                I2CArgs.Address = DDCCI_Master_Addr;
                I2CArgs.Offset = DDCCI_Slaver_Addr;
                I2CArgs.DataSize = 11;
                // I2CArgs.Flags = CTL_I2C_FLAG_ATOMICI2C; // Need to set this to do Atomic I2C call
                Result = ctlI2CAccess(context->hDisplayOutput[DisplayIndex], &I2CArgs);

                for (uint32_t i = 0; i < rece_data_len; i++)
                {
                    rece_data_arr[i] = I2CArgs.Data[i];
                }
                ZeroMemory(&I2CArgs, sizeof(I2CArgs));

                if (Result != CTL_RESULT_SUCCESS)
                    return Result; // 若传输失败，返回false
            }
        }
    }

    return Result;
}



/**
 * @description: 清理设备上下文中的资源
 * @param {DeviceContext*} context 设备上下文结构体
 */
void CleanupDeviceContext(DeviceContext *context)
{
    CTL_FREE_MEM(context->hDisplayOutput); // 释放显示输出句柄的内存
    CTL_FREE_MEM(context->hDevices);       // 释放设备句柄的内存
}
