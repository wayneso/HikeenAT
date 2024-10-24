/*
 * @Author: Wayne 2546850503@qq.com
 * @Date: 2024-09-29 09:16:02
 * @LastEditors: Wayne 2546850503@qq.com
 * @LastEditTime: 2024-10-21 20:08:46
 * @FilePath: \HikeenAT\AMD_api.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include <QByteArray>
#include <QVector>
#include <QStringList>
#include "user.h"
#include "Graphics_api.h"

int iAdapterIndex = 0;
int iDisplayIndex = 0;

/* AMD API */
void *__stdcall ADL_Main_Memory_Alloc(int iSize)
{
    void *lpBuffer = malloc(iSize);
    return lpBuffer;
}

void __stdcall ADL_Main_Memory_Free(void **lpBuffer)
{
    if (NULL != *lpBuffer)
    {
        free(*lpBuffer);
        *lpBuffer = NULL;
    }
}
/**
 * @description: amd api初始化
 * @return {*}
 */
bool InitADL(ADLPROCS& adlprocs)
{
    int ADL_Err = ADL_ERR;
    if (!adlprocs.hModule)
    {
        adlprocs.hModule = LoadLibrary(TEXT("atiadlxx.dll"));

        if (!adlprocs.hModule)
        {
            adlprocs.hModule = LoadLibrary(TEXT("atiadlxy.dll"));
        }
        if (adlprocs.hModule)
        {

            adlprocs.ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(adlprocs.hModule, "ADL_Main_Control_Create");
            adlprocs.ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(adlprocs.hModule, "ADL_Main_Control_Destroy");
            adlprocs.ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(adlprocs.hModule, "ADL_Adapter_NumberOfAdapters_Get");
            adlprocs.ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)GetProcAddress(adlprocs.hModule, "ADL_Adapter_AdapterInfo_Get");
            adlprocs.ADL_Display_DisplayInfo_Get = (ADL_DISPLAY_DISPLAYINFO_GET)GetProcAddress(adlprocs.hModule, "ADL_Display_DisplayInfo_Get");
            adlprocs.ADL_Display_DDCBlockAccess_Get = (ADL_DISPLAY_DDCBLOCKACCESSGET)GetProcAddress(adlprocs.hModule, "ADL_Display_DDCBlockAccess_Get");
            std::cerr << "success to load ADL library." << std::endl;
        }
        if (adlprocs.hModule == NULL ||
            adlprocs.ADL_Main_Control_Create == NULL ||
            adlprocs.ADL_Main_Control_Destroy == NULL ||
            adlprocs.ADL_Adapter_NumberOfAdapters_Get == NULL ||
            adlprocs.ADL_Adapter_AdapterInfo_Get == NULL ||
            adlprocs.ADL_Display_DisplayInfo_Get == NULL ||
            adlprocs.ADL_Display_DDCBlockAccess_Get == NULL)
        {
            return false;
        }
        ADL_Err = adlprocs.ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1);
    }
    return (ADL_OK == ADL_Err) ? true : false;
}

void DeInitADL(ADLPROCS& adlprocs)
{
    if (adlprocs.ADL_Main_Control_Destroy)
    {
        adlprocs.ADL_Main_Control_Destroy();
    }
    FreeLibrary(adlprocs.hModule);
}

int iInitDisplayNames(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex)
{
    int ADL_Err = ADL_ERR;
    LPAdapterInfo lpAdapterInfo = NULL;
    int iNumberAdapters = 0;
    int iNumDisplays;
    LPADLDisplayInfo lpAdlDisplayInfo = NULL;
    UINT aAllConnectedDisplays[MAX_NUM_DISPLAY_DEVICES];
    adlprocs.ADL_Adapter_NumberOfAdapters_Get(&iNumberAdapters);
    if (0 >= iNumberAdapters) // No adapters found!!!
        return 0;
    lpAdapterInfo = (LPAdapterInfo)malloc(sizeof(AdapterInfo) * iNumberAdapters);
    memset(lpAdapterInfo, '\0', sizeof(AdapterInfo) * iNumberAdapters);
    adlprocs.ADL_Adapter_AdapterInfo_Get(lpAdapterInfo, sizeof(AdapterInfo) * iNumberAdapters);
    for (int i = 0; i < iNumberAdapters; i++)
    {
        iAdapterIndex = lpAdapterInfo[i].iAdapterIndex;
        ADL_Main_Memory_Free((void **)&lpAdlDisplayInfo);
        ADL_Err = adlprocs.ADL_Display_DisplayInfo_Get(lpAdapterInfo[i].iAdapterIndex, &iNumDisplays, &lpAdlDisplayInfo, 0);
        for (int j = 0; j < iNumDisplays; j++)
        {
            if ((ADL_DISPLAY_DISPLAYINFO_DISPLAYCONNECTED | ADL_DISPLAY_DISPLAYINFO_DISPLAYMAPPED) !=
                (ADL_DISPLAY_DISPLAYINFO_DISPLAYCONNECTED | ADL_DISPLAY_DISPLAYINFO_DISPLAYMAPPED & lpAdlDisplayInfo[j].iDisplayInfoValue))
                continue;
            iDisplayIndex = lpAdlDisplayInfo[j].displayID.iDisplayLogicalIndex;
            aAllConnectedDisplays[iAdapterIndex] |= 1 << iDisplayIndex;
        }
        return iNumberAdapters;
    }
    return iNumberAdapters;
}

int vWriteI2c(ADLPROCS& adlprocs, char *lpucSendMsgBuf, int iSendMsgLen, int iAdapterIndex, int iDisplayIndex)
{
    int iRev = 0;
    return adlprocs.ADL_Display_DDCBlockAccess_Get(iAdapterIndex, iDisplayIndex, NULL, NULL, iSendMsgLen, lpucSendMsgBuf, &iRev, NULL);
}

int vWriteAndReadI2c(ADLPROCS& adlprocs, char *lpucSendMsgBuf, int iSendMsgLen, char *lpucRecvMsgBuf, int iRecvMsgLen, int iAdapterIndex, int iDisplayIndex)
{
    return adlprocs.ADL_Display_DDCBlockAccess_Get(iAdapterIndex, iDisplayIndex, NULL, NULL,
                                                   iSendMsgLen, lpucSendMsgBuf, &iRecvMsgLen, lpucRecvMsgBuf);
}

bool A_I2C_Write(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex, QVector<int> Array)
{
    int ADL_Err = ADL_ERR;
    char* sent_data = new char[Array.size()];
    for (size_t i = 0; i < Array.size(); i++)
    {
        sent_data[i] = Array[i];
    }
    ADL_Err = vWriteI2c(adlprocs, (char *)&sent_data[0], Array.size(), iAdapterIndex, iDisplayIndex);
    delete[] sent_data;
    if (ADL_Err == ADL_OK)
        return true;
    else
        return false;
}

bool A_I2C_Query(ADLPROCS& adlprocs, int& iAdapterIndex, int& iDisplayIndex, QVector<int> Array)
{
    int ADL_Err = ADL_ERR;
    UCHAR ucGetCommandReplyWrite[1]	={0x6F};
    char* query_data = new char[Array.size()];
    char* rece_data = new char[rece_data_len];
    for (size_t i = 0; i < Array.size(); i++)
    {
        query_data[i] = Array[i];
    }
    ADL_Err = vWriteI2c(adlprocs, (char *)&query_data[0], Array.size(), iAdapterIndex, iDisplayIndex);
    Sleep(10);
    ADL_Err = vWriteAndReadI2c(adlprocs, (char *)&ucGetCommandReplyWrite[0], 1, (char *)&rece_data[0], rece_data_len, iAdapterIndex, iDisplayIndex);
    for (int i = 0; i < rece_data_len; i++)
    {
        rece_data_arr[i] = rece_data[i];
        if (rece_data_arr[i] < 0) {
            rece_data_arr[i] += 256; // 还原为原来的值
        }
    }
    delete[] query_data;
    delete[] rece_data;
    if (ADL_Err == ADL_OK)
    {
        return true;
    }
    else
        return false;
}
