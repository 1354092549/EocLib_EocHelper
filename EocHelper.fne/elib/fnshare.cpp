/*
    ��Ȩ������
    ���ļ���ȨΪ�����������������У�����Ȩ����������������������֧�ֿ⣬��ֹ���������κγ��ϡ�
*/
#include "StdAfx.h"
#include "fnshare.h"
#include <assert.h>
#include "IATHook.h"

INT g_nLastNotifyResult;
PFN_NOTIFY_SYS g_fnNotifySys = NULL;

//add by liigo
PFN_ON_SYS_NOTIFY g_fn_OnSysNotify = NULL;
/*
�û���ʹ�����´��뼴�ɱ�֤���޸ı��ļ������ﵽ��ʱ����ϵͳͨѶ��Ŀ�ģ�

void My_OnSysNotify (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
}

extern PFN_ON_SYS_NOTIFY g_fn_OnSysNotify;
g_fn_OnSysNotify = (PFN_ON_SYS_NOTIFY) My_OnSysNotify;

*/
//end add


typedef BOOL(WINAPI* PFN_GetSaveFileNameA) (LPOPENFILENAMEA Arg1);
HWND g_EhWnd = 0; //������IDE���ھ��
int g_ver = 0; //������IDE�汾
char g_EPath[MAX_PATH] = { 0 };
WNDPROC g_EWndProc = 0;
HWND g_EOutputhWnd = 0;
PFN_GetSaveFileNameA Hook_GetSaveFileNameA = NULL;

void GetEOutputWnd()
{
	HWND TempWnd;
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

INT WINAPI ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	INT nRet = NR_OK;
	switch (nMsg)
	{
	case NL_SYS_NOTIFY_FUNCTION:
		g_fnNotifySys = (PFN_NOTIFY_SYS)dwParam1;

		g_EhWnd = (HWND)NotifySys(NES_GET_MAIN_HWND, NULL, NULL);
		g_ver = NotifySys(NAS_GET_VER, NULL, NULL);
		NotifySys(NAS_GET_PATH, 1, (int)&g_EPath);
		GetEOutputWnd();
		Hook_GetSaveFileNameA = (PFN_GetSaveFileNameA)IATHook(GetModuleHandle(NULL), "comdlg32.dll", "GetSaveFileNameA", (PROC)My_GetSaveFileNameA);

		g_EWndProc = (WNDPROC)SetWindowLong(g_EhWnd, GWL_WNDPROC, (int)&E_WndProc);
		EOutput("\r\n��� EOC������� ���\r\n");
		break;
	case NL_FREE_LIB_DATA:
		break;
	case NL_UNLOAD_FROM_IDE:
		SetWindowLong(g_EhWnd, GWL_WNDPROC, (int)g_EWndProc);
		IATHook(GetModuleHandle(NULL), "comdlg32.dll", "GetSaveFileNameA", (PROC)Hook_GetSaveFileNameA);
		EOutput("\r\n��� EOC����ر� ���\r\n");
		return NR_DELAY_FREE;
	default:
		nRet = NR_ERR;
		break;
	}

	//�����û�����
	if(g_fn_OnSysNotify) 
		nRet = g_fn_OnSysNotify(nMsg,dwParam1,dwParam2);

	return nRet;
}


INT WINAPI NotifySys (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	assert (g_fnNotifySys != NULL);
	if (g_fnNotifySys != NULL)
		return g_nLastNotifyResult = g_fnNotifySys (nMsg, dwParam1, dwParam2);
	else
		return g_nLastNotifyResult = 0;
}

// ʹ��ָ���ı����ݽ����׳�����ʹ�õ��ı����ݡ�
char* CloneTextData (char* ps)
{
    if (ps == NULL || *ps == '\0')
        return NULL;

    INT nTextLen = strlen (ps);
    char* pd = (char*)NotifySys (NRS_MALLOC, (DWORD)(nTextLen + 1), 0);
    memcpy (pd, ps, nTextLen);
    pd [nTextLen] = '\0';
    return pd;
}

// ʹ��ָ���ı����ݽ����׳�����ʹ�õ��ı����ݡ�
//   nTextLen����ָ���ı����ֵĳ��ȣ������������㣩��
// ���Ϊ-1����ȡps��ȫ�����ȡ�
char* CloneTextData (char* ps, INT nTextLen)
{
    if (nTextLen <= 0)
        return NULL;

    char* pd = (char*)NotifySys (NRS_MALLOC, (DWORD)(nTextLen + 1), 0);
    memcpy (pd, ps, nTextLen);
    pd [nTextLen] = '\0';
    return pd;
}

// ʹ��ָ�����ݽ����׳�����ʹ�õ��ֽڼ����ݡ�
LPBYTE CloneBinData (LPBYTE pData, INT nDataSize)
{
    if (nDataSize == 0)
        return NULL;

    LPBYTE pd = (LPBYTE)NotifySys (NRS_MALLOC, (DWORD)(sizeof (INT) * 2 + nDataSize), 0);
    *(LPINT)pd = 1;
    *(LPINT)(pd + sizeof (INT)) = nDataSize;
    memcpy (pd + sizeof (INT) * 2, pData, nDataSize);
    return pd;
}

// ��������ʱ����
void GReportError (char* szErrText)
{
    NotifySys (NRS_RUNTIME_ERR, (DWORD)szErrText, 0);
}

void* MMalloc (INT nSize)
{
	return (void*)NotifySys (NRS_MALLOC, (DWORD)nSize, 0);
}

void MFree (void* p)
{
	NotifySys (NRS_MFREE, (DWORD)p, 0);
}

// ������������ݲ����׵�ַ����Ա��Ŀ��
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount)
{
	LPINT pnData = (LPINT)pAryData;
	INT nArys = *pnData++;  // ȡ��ά����
	// �����Ա��Ŀ��
	INT nElementCount = 1;
	while (nArys > 0)
	{
		nElementCount *= *pnData++;
		nArys--;
	}

	if (pnElementCount != NULL)
		*pnElementCount = nElementCount;
	return (LPBYTE)pnData;
}

#define DTT_IS_NULL_DATA_TYPE   0
#define DTT_IS_SYS_DATA_TYPE    1
#define DTT_IS_USER_DATA_TYPE   2
#define DTT_IS_LIB_DATA_TYPE    3
// ȡ���������͵����
INT GetDataTypeType (DATA_TYPE dtDataType)
{
	if (dtDataType == _SDT_NULL)
		return DTT_IS_NULL_DATA_TYPE;

	DWORD dw = dtDataType & 0xC0000000;
	return dw == DTM_SYS_DATA_TYPE_MASK ? DTT_IS_SYS_DATA_TYPE :
			dw == DTM_USER_DATA_TYPE_MASK ? DTT_IS_USER_DATA_TYPE :
			DTT_IS_LIB_DATA_TYPE;
}


//������liigo����


/*
//!!! ���º�����������������֮��Աȫ��ΪINT����(��ͬ��ռ��4���ֽ�)
//index: 0..*/


_private void* _GetPointerByIndex(void *pBase, int index)
{
	return (void*) ((INT)pBase + index * sizeof(INT));
}

_private INT _GetIntByIndex(void *pBase, INT index)
{
	return *((INT*)_GetPointerByIndex(pBase,index));
}

_private void _SetIntByIndex(void *pBase, INT index, INT value)
{
	*((INT*)_GetPointerByIndex(pBase,index)) = value;
}


INT GetIntByIndex(PMDATA_INF pArgInf, INT index)
{
	return _GetIntByIndex(pArgInf->m_pCompoundData,index);
}

void SetIntByIndex(PMDATA_INF pArgInf, INT index, INT value)
{
	_SetIntByIndex(pArgInf->m_pCompoundData,index,value);
}

void* GetPointerByIndex(PMDATA_INF pArgInf, INT index)
{
	return _GetPointerByIndex(pArgInf->m_pCompoundData,index);
}

void SetLogfont_CompileLanVer(LPLOGFONT pLogfont)
{
	::GetObject (::GetStockObject (DEFAULT_GUI_FONT), sizeof (LOGFONT), pLogfont);
    #ifndef __COMPILE_LANG_VER
        #error *** lang.h must be imported!
    #endif
    #if (__COMPILE_LANG_VER == __BIG5_LANG_VER)
        /* �ڷ����Windows��DEFAULT_GUI_FONT�е��������ǡ��¼����w����
           ���������Ǳ���ģ����Ա���ǿ�Ƹ�Ϊ�������w����
           ����Ϊ�����wBIG5���롣Ϊ�˱��ⱻת�������滻�����������ָ�ʽд�������С� */
        _tcscpy (pLogfont->lfFaceName, _T("\xB2\xD3\xA9\xFA\xC5\xE9"));
    #elif (__COMPILE_LANG_VER == __SJIS_JP_LANG_VER)
        /* �����İ�Windows��DEFAULT_GUI_FONT�е��������ǡ�MS UI Gothic����
           ���������Ǳ���ģ����Ա���ǿ�Ƹ�Ϊ��MS Gothic����*/
        pLogfont->lfCharSet = SHIFTJIS_CHARSET;
        _tcscpy (pLogfont->lfFaceName, "MS Gothic");
    #elif (__COMPILE_LANG_VER == __ENGLISH_LANG_VER)
        _tcscpy (pLogfont->lfFaceName, _T("Courier New"));
    #endif
}