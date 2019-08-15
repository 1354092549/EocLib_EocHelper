// myelib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "EocHelper.h"
#include "IATHook.h"
#include "elib/lib2.h"
#include "elib/lang.h"
#include "elib/fnshare.h"
#include "elib/fnshare.cpp"
#include <stdlib.h>

//------------------------�ⳣ��������������Ͷ�����-----------------------------


#ifndef __E_STATIC_LIB
LIB_CONST_INFO Consts[] =
{
	/* { ��������, Ӣ������, ��������, �����ȼ�(LVL_), ��������(CT_), �ı�����, ��ֵ���� } */
	{ _WT("�Ƿ�ΪEOCģʽ"), _WT("IsEocMode"), NULL, LVL_HIGH, CT_BOOL, NULL, 0 },//��ֵ����,
	{ _WT("�Ƿ�Ϊ64λģʽ"), _WT("Is64bitMode"), NULL, LVL_HIGH, CT_BOOL, NULL, 0 }//��ֵ����
};
#endif


/*
   �����ȼ���: LVL_SIMPLE          1        // ���� 
	LVL_SECONDARY       2        // �м� 
	LVL_HIGH            3        // �߼� 

	���������� : CT_NUM             1    // sample: 3.1415926 
	CT_BOOL            2    // sample: 1 
	CT_TEXT            3    // sample: "abc" 

	�ı�������CT_TEXT�ã���ֵ������CT_NUM��CT_BOOL�á�
*/


//�����Զ�����������
#ifndef __E_STATICLIB

static LIB_DATA_TYPE_INFO s_DataType[] =
{
	/*m_szName*/                _T("ƽ̨����"),
	/*m_szEgName*/              _T("intptr"),
	/*m_szExplain*/             NULL,
	/*m_nCmdCount*/             0,      // û�г�Ա����
	/*m_pnCmdsIndex*/           NULL,
	/*m_dwState*/               LDT_ENUM,
	/*m_dwUnitBmpID*/           0,
	/*m_nEventCount*/           0,      // ����Ϊ 0 ��
	/*m_pEventBegin*/           NULL,
	/*m_nPropertyCount*/        0,      // ����Ϊ 0 ��
	/*m_pPropertyBegin*/        NULL,
	/*m_pfnGetInterface*/       NULL,   // ����Ϊ NULL ��
	/*m_nElementCount*/         0,
	/*m_pElementBegin*/         0,
};




//INT DatatypeCommandIndexs[] =
//{
//	2
//};
//static LIB_DATA_TYPE_INFO DataTypes[] =
//{
//	/* { ��������, Ӣ������, ��������, ��������, ��������, ����״̬, ͼ������, �¼�����, �¼�ָ��, ��������, ����ָ��, ����ָ��, Ԫ������, Ԫ��ָ�� } */
//	{ _WT("������������"), _WT("DatatypeCommand"), _WT("����������������"), sizeof(DatatypeCommandIndexs) / sizeof(DatatypeCommandIndexs[0]), DatatypeCommandIndexs, NULL, 0, 0, NULL, 0, NULL, NULL, 0, NULL },
//	{ _WT("����"), _WT("ziti"), _WT("��������"), sizeof(DatatypeCommandIndexs) / sizeof(DatatypeCommandIndexs[0]), DatatypeCommandIndexs, NULL, 0, 0, NULL, 0, NULL, NULL, 0, NULL }
//};
#endif

// ����LIB_DATA_TYPE_INFO�Ľ��� �Լ� ���������ݵĶ��� ���ɲμ������ĵ��� "m_pDataType��Ա˵��"��"��������˵��"��

//��������

EXTERN_C void EocHelper_ToIntPtr(PMDATA_INF pRetData, INT iArgCount, PMDATA_INF pArgInf)
{
	switch (pArgInf[0].m_dtDataType)
	{
	case SDT_BYTE:
		pRetData->m_int = pArgInf[0].m_byte;
		break;
	case SDT_SHORT:
		pRetData->m_int = pArgInf[0].m_short;
		break;
	case SDT_INT:
		pRetData->m_int = pArgInf[0].m_int;
		break;
	case SDT_INT64:
		pRetData->m_int = pArgInf[0].m_int64;
		break;
	case SDT_FLOAT:
		pRetData->m_int = pArgInf[0].m_float;
		break;
	case SDT_DOUBLE:
		pRetData->m_int = pArgInf[0].m_double;
		break;
	case SDT_BOOL:
		pRetData->m_int = pArgInf[0].m_bool;
		break;
	case SDT_SUB_PTR:
		pRetData->m_int = (int)pArgInf[0].m_pdwSubCodeAdr;
		break;
	case SDT_TEXT:
		pRetData->m_int = atol(pArgInf[0].m_pText);
		break;
	default:
		pRetData->m_int = 0;
		break;
	}
};


/*
������ʵ�ֶ���Ҫ�����ں�������Ա㾲̬�Ͷ�̬�ⶼ��ʹ�ã���ExecuteCommand��Commands��ֻ�趨���ں�����湩��̬��ʹ�á�
pRetData �������ָ�롣����ӦCMD_INFO��m_dtRetTypeΪ_SDT_NULL���������޷���ֵ��ʱ��pRetData��Ч��
iArgCount ������������
pArgInf ��������ָ��
*/



#ifndef __E_STATIC_LIB
PFN_EXECUTE_CMD ExecuteCommand[] =
{
	EocHelper_ToIntPtr // ������Ҫ���е��õĺ�������������ö��Ÿ���
};
static const char* const CommandNames[] =
{
	"EocHelper_ToIntPtr" // ������Ҫ���е��õĺ�������д������ö��Ÿ���
};
ARG_INFO CommandArgs[] =
{
	/* { ��������, ��������, ͼ������, ͼ������, ��������(�μ�SDT_), Ĭ����ֵ, �������(�μ�AS_) } */
	{ _WT("����"), _WT("��ת��������"), 0, 0, _SDT_ALL, NULL, NULL } //�����������鶨��д�����ÿ��{}Ϊһ�������ı������ö��Ÿ���
};
static CMD_INFO Commands[] =
{
	/* { ��������, Ӣ������, ��������, �������(-1���������͵ķ���), ����״̬(CT_), ��������(SDT_), ��ֵ����, ����ȼ�(LVL_), ͼ������, ͼ������, ��������, ������Ϣ } */
	{ _WT("��ƽ̨����"), _WT("ToIntPtr"), _WT("���� ������/��������"), 1, NULL, 1, 0, LVL_HIGH, 0, 0, 1, CommandArgs },//��������
};
#endif

//--------------------------------------------------------------------
#ifndef __E_STATIC_LIB
void GetEOutputWnd()
{
	HWND TempWnd = 0;
	HWND StatusWnd = 0;
	HWND TabWnd = 0;
	if (!g_EhWnd)
	{
		return;
	}
	//�����Ե�״̬�п��Թ̶��ĺ������
	do //�Ȳ��ҹ̶���״̬��
	{
		TempWnd = FindWindowEx(g_EhWnd, TempWnd, "AfxControlBar42s", 0); //���ڱ����ǡ�״̬�С�������Ӣ�İ�
		if (TempWnd == 0) //״̬�е��Ӵ��ڲ�����˵��״̬�д��ڷ����ˣ������Իᴴ��һ���������ڣ�Ȼ����Ӵ��ڷ����ȥ
		{
			//�Ȳ������ж�������
			do
			{
				TempWnd = FindWindowEx(0, TempWnd, "Afx:400000:8:10003:0:0", 0); //���ڱ����ǡ�״̬�С��������������NULL
				if (TempWnd == 0)
				{
					return; //û���ҵ�
				}
				if (GetParent(TempWnd) == g_EhWnd) //�����ڱ����������Ե�IDE����
				{
					StatusWnd = GetDlgItem(TempWnd, 59423);
					if (StatusWnd)
					{
						StatusWnd = GetDlgItem(StatusWnd, 130); //�����Ҫ�ҵ��Ӵ���
					}
				}
			} while (StatusWnd == 0);
		}
		else
		{
			StatusWnd = GetDlgItem(TempWnd, 130); //��״̬������ֻ��һ���Ӵ���
		}

	} while (StatusWnd == 0); //�������Ӵ��ڲ����ڣ���ͼ���ѭ�����ҵ�Ϊֹ
	TabWnd = GetDlgItem(GetDlgItem(StatusWnd, 0), 1000);
	g_EOutputhWnd = GetDlgItem(TabWnd, 1011);
}

BOOL EOutput(LPCSTR txt)
{
	SendMessage(g_EOutputhWnd, EM_SETSEL, -2, -1);  //�ƶ���굽ĩβ
	return SendMessage(g_EOutputhWnd, EM_REPLACESEL, 0, (int)txt) != 0;
}


int WINAPI E_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int ret = CallWindowProc(g_EWndProc, hwnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
	case WM_INITMENUPOPUP:
	{
		HMENU hMenu = (HMENU)wParam;
		int nCount = GetMenuItemCount(hMenu);
		int uId, uState;
		int i;

		for (i = 0; nCount > i; i++)
		{
			uId = GetMenuItemID(hMenu, i);
			if (uId == IDM_����)
			{
				i++;
				uId = GetMenuItemID(hMenu, i);
				if (uId != IDM_EOCBuild)
				{
					InsertMenu(hMenu, i, MF_BYPOSITION, IDM_EOCBuild, "&EOC����");
				}
				//����˵�״̬
				uState = GetMenuState(hMenu, IDM_����, MF_BYCOMMAND);
				EnableMenuItem(hMenu, IDM_EOCBuild, MF_BYCOMMAND | uState);
				g_EOCBuild = false;
				break;
			}
		}
		break;
	}
	case WM_COMMAND:
	{
		int MenuID = wParam;
		if (MenuID == IDM_EOCBuild)
		{
			g_EOCBuild = true;
			SendMessage(hwnd, WM_COMMAND, IDM_����Ϊָ������_Windows������ģ��, 0);
			g_EOCBuild = false;
		}
		break;
	}
	}
	return ret;
}


BOOL WINAPI My_GetSaveFileNameA(LPOPENFILENAMEA Arg1)
{
	BOOL ret = Hook_GetSaveFileNameA(Arg1);
	if (ret && g_EOCBuild)
	{
		if (Arg1->lpstrFile)
		{
			MessageBox(g_EhWnd, Arg1->lpstrFile, "���浽", MB_OK);
		}

	}

	return ret;
}
#endif

//--------------------------------------------------------------------

EXTERN_C INT WINAPI EocHelper_ProcessNotifyLib(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
#ifndef __E_STATIC_LIB
	if (nMsg == NL_GET_CMD_FUNC_NAMES) // ������������ʵ�ֺ����ĵĺ�����������(char*[]), ֧�־�̬����Ķ�̬����봦��
		return (INT)CommandNames;
	else if (nMsg == NL_GET_NOTIFY_LIB_FUNC_NAME) // ���ش���ϵͳ֪ͨ�ĺ�������(PFN_NOTIFY_LIB��������), ֧�־�̬����Ķ�̬����봦��
		return (INT)"EocHelper_ProcessNotifyLib";
	else if (nMsg == NL_GET_DEPENDENT_LIBS) return (INT)NULL;
	// ���ؾ�̬����������������̬���ļ����б�(��ʽΪ\0�ָ����ı�,��β����\0), ֧�־�̬����Ķ�̬����봦��
	// kernel32.lib user32.lib gdi32.lib �ȳ��õ�ϵͳ�ⲻ��Ҫ���ڴ��б���
	// ����NULL��NR_ERR��ʾ��ָ�������ļ�  
	else if (nMsg == NL_SYS_NOTIFY_FUNCTION)
	{
		if (g_fnNotifySys)
		{
			return NR_OK; //��ֹ�ظ�����
		}
		int nRet = ProcessNotifyLib(nMsg, dwParam1, dwParam2); //��������ȡg_fnNotifySys

		g_EhWnd = (HWND)NotifySys(NES_GET_MAIN_HWND, NULL, NULL);
		g_ver = NotifySys(NAS_GET_VER, NULL, NULL);
		NotifySys(NAS_GET_PATH, 1, (int)& g_EPath);
		GetEOutputWnd();
		Hook_GetSaveFileNameA = (PFN_GetSaveFileNameA)IATHook(GetModuleHandle(NULL), "comdlg32.dll", "GetSaveFileNameA", (PROC)My_GetSaveFileNameA);

		g_EWndProc = (WNDPROC)SetWindowLong(g_EhWnd, GWL_WNDPROC, (LONG)&E_WndProc);
		EOutput("\r\n��� EOC������� ���\r\n");
		
		return nRet;
	}
	else if (nMsg == NL_UNLOAD_FROM_IDE)
	{
		SetWindowLong(g_EhWnd, GWL_WNDPROC, (int)g_EWndProc);
		IATHook(GetModuleHandle(NULL), "comdlg32.dll", "GetSaveFileNameA", (PROC)Hook_GetSaveFileNameA);
		EOutput("\r\n��� EOC����ر� ���\r\n");
		return NR_DELAY_FREE;
	}

#endif
	return ProcessNotifyLib(nMsg, dwParam1, dwParam2);
};


#ifndef __E_STATIC_LIB
static LIB_INFO LibInfo =
{
	/* { ���ʽ��, GUID����, ���汾��, �ΰ汾��, �����汾��, ϵͳ���汾��, ϵͳ�ΰ汾��, ���Ŀ����汾��, ���Ŀ�ΰ汾��,
	֧�ֿ���, ֧�ֿ�����, ֧�ֿ�����, ֧�ֿ�״̬,
	��������, ��������, ͨ�ŵ�ַ, �绰����, �������, ��������, ��ҳ��ַ, ������Ϣ,
	��������, ����ָ��, �������, �������, ��������, ����ָ��, �������,
	���ӹ���, ��������, ��Ϣָ��, ����ģ��, ģ������,
	��������, ����ָ��, �ⲿ�ļ�} */
	LIB_FORMAT_VER,
	_T(LIB_GUID_STR),
	LIB_MajorVersion,
	LIB_MinorVersion,
	LIB_BuildNumber,
	LIB_SysMajorVer,
	LIB_SysMinorVer,
	LIB_KrnlLibMajorVer,
	LIB_KrnlLibMinorVer,
	_T(LIB_NAME_STR),
	__GBK_LANG_VER,
	_WT(LIB_DESCRIPTION_STR),
	_LIB_OS(__OS_WIN),
	_WT(LIB_Author),
	_WT(LIB_ZipCode),
	_WT(LIB_Address),
	_WT(LIB_Phone),
	_WT(LIB_Fax),
	_WT(LIB_Email),
	_WT(LIB_HomePage),
	_WT(LIB_Other),
	sizeof(s_DataType) / sizeof(s_DataType[0]),
	s_DataType,
	LIB_TYPE_COUNT,
	_WT(LIB_TYPE_STR),
	sizeof(Commands) / sizeof(Commands[0]),
	Commands,
	ExecuteCommand,
	NULL,
	NULL,
	EocHelper_ProcessNotifyLib,
	NULL,
	NULL,
	sizeof(Consts) / sizeof(Consts[0]),
	Consts,
	NULL
};
PLIB_INFO WINAPI GetNewInf()
{
	return (&LibInfo);
};

#endif
