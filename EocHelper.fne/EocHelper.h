#pragma once


#ifndef __E_STATIC_LIB
#define LIB_GUID_STR "{808854C5-D149-4F1E-BB08-A53CB84BD12E}" /*GUID��: {059F1362-C1B0-439E-8204-7C5CDF43CA0D}, ����ʹ��guidgen.exe����*/
#define LIB_MajorVersion 1 /*�����汾��*/
#define LIB_MinorVersion 0 /*��ΰ汾��*/
#define LIB_BuildNumber 20190815 /*�����汾��*/
#define LIB_SysMajorVer 3 /*ϵͳ���汾��*/
#define LIB_SysMinorVer 0 /*ϵͳ�ΰ汾��*/
#define LIB_KrnlLibMajorVer 3 /*���Ŀ����汾��*/
#define LIB_KrnlLibMinorVer 0 /*���Ŀ�ΰ汾��*/
#define LIB_NAME_STR "EocHelper" /*֧�ֿ���*/
#define LIB_DESCRIPTION_STR "EocHelper" /*��������*/
#define LIB_Author "QIQI" /*��������*/
#define LIB_ZipCode "-" /*��������*/
#define LIB_Address "-" /*ͨ�ŵ�ַ*/
#define LIB_Phone "-" /*�绰����*/
#define LIB_Fax "-" /*�������*/
#define LIB_Email "1354092549@qq.com" /*��������*/
#define LIB_HomePage "https://qiqiworld.tk/" /*��ҳ��ַ*/
#define LIB_Other "EocHelper" /*������Ϣ*/
#define LIB_TYPE_COUNT 1 /*�����������*/
#define LIB_TYPE_STR "0000��������\0""\0" /*�������*/
#endif

#define IDM_���� 57603
#define IDM_���¹������� 32952
#define IDM_���� 32851
#define IDM_��̬���� 32912
#define IDM_�������� 32911
#define IDM_����Ϊ�װ� 32980
#define IDM_�������ɰ�װ��� 32909
#define IDM_����Ϊָ������_Windows���ڳ��� 32985
#define IDM_����Ϊָ������_Windows����̨���� 32986
#define IDM_����Ϊָ������_Windows��̬���ӿ� 32987
#define IDM_����Ϊָ������_Windows������ģ�� 32988

#define IDM_EOCBuild 5000 //���Զ��壬��Ҫ�����еĳ�ͻ��

int WINAPI E_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI My_GetSaveFileNameA(LPOPENFILENAMEA Arg1);

typedef BOOL(WINAPI* PFN_GetSaveFileNameA) (LPOPENFILENAMEA Arg1);

HWND g_EhWnd = 0; //������IDE���ھ��
int g_ver = 0; //������IDE�汾
char g_EPath[MAX_PATH] = { 0 };
WNDPROC g_EWndProc = 0;
HWND g_EOutputhWnd = 0;
PFN_GetSaveFileNameA Hook_GetSaveFileNameA = NULL;
BOOL g_EOCBuild = false;
