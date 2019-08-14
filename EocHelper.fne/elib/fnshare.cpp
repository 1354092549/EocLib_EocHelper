/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
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
用户在使用以下代码即可保证不修改本文件，而达到及时与易系统通讯的目的：

void My_OnSysNotify (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
}

extern PFN_ON_SYS_NOTIFY g_fn_OnSysNotify;
g_fn_OnSysNotify = (PFN_ON_SYS_NOTIFY) My_OnSysNotify;

*/
//end add


typedef BOOL(WINAPI* PFN_GetSaveFileNameA) (LPOPENFILENAMEA Arg1);
HWND g_EhWnd = 0; //易语言IDE窗口句柄
int g_ver = 0; //易语言IDE版本
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
	//易语言的状态夹可以固定的和脱离的
	do //先查找固定的状态夹
	{
		TempWnd = FindWindowEx(g_EhWnd, TempWnd, "AfxControlBar42s", 0); //窗口标题是“状态夹”，但有英文版
		if (TempWnd == 0) //状态夹的子窗口不存在说明状态夹窗口分离了，易语言会创建一个顶级窗口，然后把子窗口分离过去
		{
			//先查找所有顶级窗口
			do
			{
				TempWnd = FindWindowEx(0, TempWnd, "Afx:400000:8:10003:0:0", 0); //窗口标题是“状态夹”，如果隐藏则是NULL
				if (TempWnd == 0)
				{
					return; //没有找到
				}
				if (GetParent(TempWnd) == g_EhWnd) //父窗口必须是易语言的IDE窗口
				{
					StatusWnd = GetDlgItem(TempWnd, 59423);
					if (StatusWnd)
					{
						StatusWnd = GetDlgItem(StatusWnd, 130); //这就是要找的子窗口
					}
				}
			} while (StatusWnd == 0);
		}
		else
		{
			StatusWnd = GetDlgItem(TempWnd, 130); //在状态夹里面只有一个子窗口
		}

	} while (StatusWnd == 0); //如果这个子窗口不存在，这就继续循环到找到为止
	TabWnd = GetDlgItem(GetDlgItem(StatusWnd, 0), 1000);
	g_EOutputhWnd = GetDlgItem(TabWnd, 1011);
}

BOOL EOutput(LPCSTR txt)
{
	SendMessage(g_EOutputhWnd, EM_SETSEL, -2, -1);  //移动光标到末尾
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
		EOutput("\r\n★★ EOC插件开启 ★★\r\n");
		break;
	case NL_FREE_LIB_DATA:
		break;
	case NL_UNLOAD_FROM_IDE:
		SetWindowLong(g_EhWnd, GWL_WNDPROC, (int)g_EWndProc);
		IATHook(GetModuleHandle(NULL), "comdlg32.dll", "GetSaveFileNameA", (PROC)Hook_GetSaveFileNameA);
		EOutput("\r\n☆☆ EOC插件关闭 ☆☆\r\n");
		return NR_DELAY_FREE;
	default:
		nRet = NR_ERR;
		break;
	}

	//调用用户代码
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

// 使用指定文本数据建立易程序中使用的文本数据。
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

// 使用指定文本数据建立易程序中使用的文本数据。
//   nTextLen用作指定文本部分的长度（不包含结束零），
// 如果为-1，则取ps的全部长度。
char* CloneTextData (char* ps, INT nTextLen)
{
    if (nTextLen <= 0)
        return NULL;

    char* pd = (char*)NotifySys (NRS_MALLOC, (DWORD)(nTextLen + 1), 0);
    memcpy (pd, ps, nTextLen);
    pd [nTextLen] = '\0';
    return pd;
}

// 使用指定数据建立易程序中使用的字节集数据。
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

// 报告运行时错误。
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

// 返回数组的数据部分首地址及成员数目。
LPBYTE GetAryElementInf (void* pAryData, LPINT pnElementCount)
{
	LPINT pnData = (LPINT)pAryData;
	INT nArys = *pnData++;  // 取得维数。
	// 计算成员数目。
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
// 取回数据类型的类别。
INT GetDataTypeType (DATA_TYPE dtDataType)
{
	if (dtDataType == _SDT_NULL)
		return DTT_IS_NULL_DATA_TYPE;

	DWORD dw = dtDataType & 0xC0000000;
	return dw == DTM_SYS_DATA_TYPE_MASK ? DTT_IS_SYS_DATA_TYPE :
			dw == DTM_USER_DATA_TYPE_MASK ? DTT_IS_USER_DATA_TYPE :
			DTT_IS_LIB_DATA_TYPE;
}


//以下由liigo添加


/*
//!!! 以下函数均假设数据类型之成员全部为INT类型(或同样占用4个字节)
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
        /* 在繁体版Windows中DEFAULT_GUI_FONT中的字体名是“新細明體”，
           而该字体是变宽的，所以必须强制改为“細明體”。
           下面为細明體BIG5内码。为了避免被转换工具替换，必须以这种格式写到程序中。 */
        _tcscpy (pLogfont->lfFaceName, _T("\xB2\xD3\xA9\xFA\xC5\xE9"));
    #elif (__COMPILE_LANG_VER == __SJIS_JP_LANG_VER)
        /* 在日文版Windows中DEFAULT_GUI_FONT中的字体名是“MS UI Gothic”，
           而该字体是变宽的，所以必须强制改为“MS Gothic”。*/
        pLogfont->lfCharSet = SHIFTJIS_CHARSET;
        _tcscpy (pLogfont->lfFaceName, "MS Gothic");
    #elif (__COMPILE_LANG_VER == __ENGLISH_LANG_VER)
        _tcscpy (pLogfont->lfFaceName, _T("Courier New"));
    #endif
}
