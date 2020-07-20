#include "stdafx.h"
#include "EocHelper.h"
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

EXTERN_C void EOCAttribute_AutoParam(PMDATA_INF pRetData, INT iArgCount, PMDATA_INF pArgInf)
{
	return;
}

/*
������ʵ�ֶ���Ҫ�����ں�������Ա㾲̬�Ͷ�̬�ⶼ��ʹ�ã���ExecuteCommand��Commands��ֻ�趨���ں�����湩��̬��ʹ�á�
pRetData �������ָ�롣����ӦCMD_INFO��m_dtRetTypeΪ_SDT_NULL���������޷���ֵ��ʱ��pRetData��Ч��
iArgCount ������������
pArgInf ��������ָ��
*/



#ifndef __E_STATIC_LIB
PFN_EXECUTE_CMD ExecuteCommand[] =
{
	EocHelper_ToIntPtr, // ������Ҫ���е��õĺ�������������ö��Ÿ���
	EOCAttribute_AutoParam
};
static const char* const CommandNames[] =
{
	"EocHelper_ToIntPtr", // ������Ҫ���е��õĺ�������д������ö��Ÿ���
	"EOCAttribute_AutoParam"
};
ARG_INFO ArgsInfo_ToIntPtr[] =
{
	/* { ��������, ��������, ͼ������, ͼ������, ��������(�μ�SDT_), Ĭ����ֵ, �������(�μ�AS_) } */
	{ _WT("����"), _WT("��ת��������"), 0, 0, _SDT_ALL, NULL, NULL }
};
ARG_INFO ArgsInfo_EOCAttribute_AutoParam[] =
{
	{ _WT("����"), _WT("����ǵ��ӳ������"), 0, 0, _SDT_ALL, NULL, NULL }
};
static CMD_INFO Commands[] =
{
	/* { ��������, Ӣ������, ��������, �������(-1���������͵ķ���), ����״̬(CT_), ��������(SDT_), ��ֵ����, ����ȼ�(LVL_), ͼ������, ͼ������, ��������, ������Ϣ } */
	{ _WT("��ƽ̨����"), _WT("ToIntPtr"), _WT("���� ������/��������"), 1, 0, 1, 0, LVL_HIGH, 0, 0, 1, ArgsInfo_ToIntPtr },
	{ _WT("EOC���_����Ӧ����"), _WT("EOCAttribute_AutoParam"), _WT("������������ӳ���ͷ�����ڷǳ�Ա�����п��ã����ڽ��������Ϊ����Ӧ��Auto�����ͣ���EOC�����Զ�����"), 1, CT_DISABLED_IN_RELEASE, _SDT_NULL, 0, LVL_HIGH, 0, 0, 1, ArgsInfo_EOCAttribute_AutoParam },
};
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


