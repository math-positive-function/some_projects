#include <iostream>
#include <cstring>
#include <thread> // 添加头文件以使用 sleep_for
using namespace std;
#include "HCNetSDK.h"

static const char sdkPath[] = "./";              // SDK路径
static const char logPath[] = "./SdkLog_Cpp/";   // 日志路径
static const char certificatePath[] = "./cert/"; // 证书路径

// 模拟 NET_DVR_SetLogToFile 方法的调用
void SetLogToFile(const char *logPath, int logToFile)
{
    NET_DVR_LOCAL_SDK_PATH sdkPathStruct;
    strcpy(sdkPathStruct.sPath, "/home/daxian/Desktop/test");

    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_SDK_PATH, &sdkPathStruct);
    
    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_LIBEAY_PATH, (void *)"/home/daxian/Desktop/test/libcrypto.so.1.1");
    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_SSLEAY_PATH, (void *)"/home/daxian/Desktop/test/libssl.so.1.1");

    NET_DVR_SetLogToFile(4, const_cast<char *>(logPath), logToFile);
}

// 获取视场角的函数
void GetGisInfo(LONG lHandle)
{
    NET_DVR_STD_CONFIG std_config = {0};
    DWORD channel = 1; // 你可能需要根据实际情况更改通道号

    NET_DVR_GIS_INFO gis_info_tmp = {0};
    std_config.lpStatusBuffer = NULL;
    std_config.dwStatusSize = 0;
    std_config.lpOutBuffer = &gis_info_tmp;
    std_config.dwOutSize = sizeof(NET_DVR_GIS_INFO);
    std_config.byDataType = 0;
    std_config.lpCondBuffer = &channel;
    std_config.dwCondSize = sizeof(channel);
    if (!NET_DVR_GetSTDConfig(lHandle, NET_DVR_GET_GISINFO, &std_config))
    {
        cerr << "Failed to get GIS information" << endl;
        cerr << NET_DVR_GetErrorMsg() << endl;
        // 这里可以添加错误处理逻辑
        return;
    }

    cout << "fHorizontalValue: " << gis_info_tmp.fHorizontalValue << endl;
    cout << "fVerticalValue: " << gis_info_tmp.fVerticalValue << endl;
    cout << "fVisibleRadius: " << gis_info_tmp.fVisibleRadius << endl;
    cout << "fMaxViewRadius: " << gis_info_tmp.fMaxViewRadius << endl;
    cout << "H: " << gis_info_tmp.fHorizontalValue << endl;
    cout << "V: " << gis_info_tmp.fVerticalValue << endl;
    cout<<"----------------------------------------------------------------"<<endl;
}

int main()
{
    NET_DVR_Init();

    // 设置日志输出到指定目录
    SetLogToFile(logPath, false);

    NET_DVR_USER_LOGIN_INFO struLoginInfo;
    NET_DVR_DEVICEINFO_V40 struDeviceInfo;

    strncpy(struLoginInfo.sDeviceAddress, "36.153.142.114", strlen("36.153.142.114") + 1);
    strncpy(struLoginInfo.sUserName, "admin", strlen("admin") + 1);
    strncpy(struLoginInfo.sPassword, "njcm123456", strlen("njcm123456") + 1);
    struLoginInfo.bUseAsynLogin = 0;
    struLoginInfo.byUseTransport = 0;
    struLoginInfo.wPort = 18800;

    // 打印设备地址等信息
    cout << "Device Address: " << struLoginInfo.sDeviceAddress << endl;
    cout << "Device Port: " << struLoginInfo.wPort << endl;

    LONG lHandle = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if (lHandle < 0)
    {
        cerr << NET_DVR_GetErrorMsg() << endl;
        return -1;
    }

    cout << "Login success. Handle: " << lHandle << endl;

    // 主循环
    while (true)
    {
        // 获取视场角信息
        GetGisInfo(lHandle);

        // 休眠2秒
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // 注意：这里没有调用 NET_DVR_Logout 和 NET_DVR_Cleanup，你可能需要在程序退出前添加这些调用。

    return 0;
}
