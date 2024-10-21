/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-28 17:12:46
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-09-30 10:24:59
 * @FilePath: \DDCCI_Protocol\Nvidia_api.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <QByteArray>
#include <QStringList>
#include <QVector>

#include "Graphics_api.h"
#include "ddcci_protocol.h"
#include "qt_func.h"
#include "user.h"

/* Nvidia 函数 */
NvU32 outputID = 0;
NvPhysicalGpuHandle hGpu = NULL;

void copyData(int* rece_data_arr, const BYTE* readBytes, int rece_data_len) {
    for (int i = 0; i < rece_data_len; i++) {
        rece_data_arr[i] = static_cast<int>(readBytes[i]); // 假设需要转换为 int
    }
}

#define INIT_I2CINFO(i2cInfo, i2cVersion, displayId, isDDCPort, i2cDevAddr, regAddr, regSize, dataBuf, bufSize, speed) \
do                                                                                                                 \
    {                                                                                                                  \
            i2cInfo.version = i2cVersion;                                                                                  \
            i2cInfo.displayMask = displayId;                                                                               \
            i2cInfo.bIsDDCPort = isDDCPort;                                                                                \
            i2cInfo.i2cDevAddress = i2cDevAddr;                                                                            \
            i2cInfo.pbI2cRegAddress = (BYTE *)&regAddr;                                                                    \
            i2cInfo.regAddrSize = regSize;                                                                                 \
            i2cInfo.pbData = (BYTE *)&dataBuf;                                                                             \
            i2cInfo.cbSize = bufSize;                                                                                      \
            i2cInfo.i2cSpeed = speed;                                                                                      \
    } while (0)

    BOOL N_I2C_Write(NvPhysicalGpuHandle& hPhysicalGpu, NvU32& displayId, QVector<int> Array)
{
    NvAPI_Status nvapiStatus = NVAPI_ERROR;
    NvU8 i2cWriteDeviceAddr = master_addr;
    BYTE i2cReadDeviceAddr[1] = {0x00};
    i2cReadDeviceAddr[0] = slaver_addr;
    NV_I2C_INFO i2cInfo = {0};
    // BYTE sent_data[MAX_VALUES];
    // 从 QVector<int> 中删除前 2 个元素
    Array.remove(0, 2);
    BYTE* sent_data = new BYTE[Array.size()];
    for (size_t i = 0; i < Array.size(); i++)
    {
        sent_data[i] = Array[i];
    }
    INIT_I2CINFO(i2cInfo, NV_I2C_INFO_VER, displayId, TRUE, i2cWriteDeviceAddr, i2cReadDeviceAddr,
                 sizeof(i2cReadDeviceAddr), *sent_data, Array.size(), 27);

    nvapiStatus = NvAPI_I2CWrite(hPhysicalGpu, &i2cInfo);
    delete[] sent_data;
    if (nvapiStatus != NVAPI_OK)
    {
        return FALSE;
    }
    return true;
}

BOOL N_I2C_Query(NvPhysicalGpuHandle hPhysicalGpu, NvU32 displayId, QVector<int> Array)
{
    NvAPI_Status nvapiStatus = NVAPI_ERROR;
    NvU8 i2cWriteDeviceAddr = master_addr;
    BYTE i2cReadDeviceAddr[1] = {0};
    i2cReadDeviceAddr[0] = slaver_addr;
    BYTE i2cRegAddress[1] = {0};
    i2cRegAddress[0] = master_addr;
    NV_I2C_INFO i2cInfo = {0};
    // //删除 6E 51
    Array.remove(0, 2);
    BYTE* sent_data = new BYTE[Array.size()];
    for (size_t i = 0; i < Array.size(); i++)
    {
        sent_data[i] = Array[i];
    }
    INIT_I2CINFO(i2cInfo, NV_I2C_INFO_VER, displayId, TRUE, i2cWriteDeviceAddr, i2cReadDeviceAddr,
                 sizeof(i2cReadDeviceAddr), *sent_data, Array.size(), 27);
    nvapiStatus = NvAPI_I2CWrite(hPhysicalGpu, &i2cInfo);
    BYTE* rece_data = new BYTE[rece_data_len];
    Sleep( 50 );
    INIT_I2CINFO(i2cInfo, NV_I2C_INFO_VER, displayId, TRUE, 0X6F, i2cRegAddress,
                 sizeof(i2cRegAddress), *rece_data, rece_data_len, 27);

    nvapiStatus = NvAPI_I2CRead(hPhysicalGpu, &i2cInfo);

    for (int i = 0; i < rece_data_len; i++) {
        rece_data_arr[i] = static_cast<int>(rece_data[i]); // 假设需要转换为 int
    }

    delete[] sent_data;
    delete[] rece_data;
    if (nvapiStatus != NVAPI_OK)
    {
        qDebug() << "NvAPI_I2CRead failed with status :" << nvapiStatus;
        return FALSE;
    }
    return TRUE;
}

int Nvidia_Get_Display(NvPhysicalGpuHandle& hPhysicalGpu, NvU32& displayId)
{
    NvAPI_Status nvapiStatus = NVAPI_OK;
    NvDisplayHandle hDisplay_a[NVAPI_MAX_PHYSICAL_GPUS * NVAPI_MAX_DISPLAY_HEADS] = {0};
    NvU32 nvDisplayCount = 0;
    for (unsigned int i = 0; nvapiStatus == NVAPI_OK; i++)
    {
        nvapiStatus = NvAPI_EnumNvidiaDisplayHandle(i, &hDisplay_a[i]);

        if (nvapiStatus == NVAPI_OK)
        {
            nvDisplayCount++;
        }
        else if (nvapiStatus != NVAPI_END_ENUMERATION)
        {
            return false;
        }
    }
    NvPhysicalGpuHandle hGPU_a[NVAPI_MAX_PHYSICAL_GPUS] = {0}; // handle to GPUs
    NvU32 gpuCount = 0;
    nvapiStatus = NvAPI_EnumPhysicalGPUs(hGPU_a, &gpuCount);
    if (nvapiStatus != NVAPI_OK)
    {
        return false;
    }
    BOOL isTestPassed = TRUE;
    for (unsigned int i = 0; i < nvDisplayCount; i++)
    {
        // Get GPU id assiciated with display ID

        NvU32 pGpuCount = 0;
        nvapiStatus = NvAPI_GetPhysicalGPUsFromDisplay(hDisplay_a[i], &hPhysicalGpu, &pGpuCount);
        if (nvapiStatus != NVAPI_OK)
        {
            isTestPassed = FALSE;
            break;
        }
        nvapiStatus = NvAPI_GetAssociatedDisplayOutputId(hDisplay_a[i], &displayId);
        if (nvapiStatus != NVAPI_OK)
        {
            isTestPassed = FALSE;
            break;
        }
        // qDebug() << "GPU handle: " << hPhysicalGpu << "Output ID:" << displayId;
    }
    return true;
}
