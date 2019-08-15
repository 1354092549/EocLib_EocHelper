#pragma once


#ifndef __E_STATIC_LIB
#define LIB_GUID_STR "{808854C5-D149-4F1E-BB08-A53CB84BD12E}" /*GUID串: {059F1362-C1B0-439E-8204-7C5CDF43CA0D}, 必须使用guidgen.exe生成*/
#define LIB_MajorVersion 1 /*库主版本号*/
#define LIB_MinorVersion 0 /*库次版本号*/
#define LIB_BuildNumber 20190815 /*构建版本号*/
#define LIB_SysMajorVer 3 /*系统主版本号*/
#define LIB_SysMinorVer 0 /*系统次版本号*/
#define LIB_KrnlLibMajorVer 3 /*核心库主版本号*/
#define LIB_KrnlLibMinorVer 0 /*核心库次版本号*/
#define LIB_NAME_STR "EocHelper" /*支持库名*/
#define LIB_DESCRIPTION_STR "EocHelper" /*功能描述*/
#define LIB_Author "QIQI" /*作者名称*/
#define LIB_ZipCode "-" /*邮政编码*/
#define LIB_Address "-" /*通信地址*/
#define LIB_Phone "-" /*电话号码*/
#define LIB_Fax "-" /*传真号码*/
#define LIB_Email "1354092549@qq.com" /*电子邮箱*/
#define LIB_HomePage "https://qiqiworld.tk/" /*主页地址*/
#define LIB_Other "EocHelper" /*其它信息*/
#define LIB_TYPE_COUNT 1 /*命令分类数量*/
#define LIB_TYPE_STR "0000基本命令\0""\0" /*命令分类*/
#endif

#define IDM_保存 57603
#define IDM_重新关联名称 32952
#define IDM_编译 32851
#define IDM_静态编译 32912
#define IDM_独立编译 32911
#define IDM_编译为易包 32980
#define IDM_编译生成安装软件 32909
#define IDM_编译为指定类型_Windows窗口程序 32985
#define IDM_编译为指定类型_Windows控制台程序 32986
#define IDM_编译为指定类型_Windows动态链接库 32987
#define IDM_编译为指定类型_Windows易语言模块 32988

#define IDM_EOCBuild 5000 //可自定义，不要和已有的冲突。

int WINAPI E_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI My_GetSaveFileNameA(LPOPENFILENAMEA Arg1);

typedef BOOL(WINAPI* PFN_GetSaveFileNameA) (LPOPENFILENAMEA Arg1);

HWND g_EhWnd = 0; //易语言IDE窗口句柄
int g_ver = 0; //易语言IDE版本
char g_EPath[MAX_PATH] = { 0 };
WNDPROC g_EWndProc = 0;
HWND g_EOutputhWnd = 0;
PFN_GetSaveFileNameA Hook_GetSaveFileNameA = NULL;
BOOL g_EOCBuild = false;
