#include "stdafx.h"
#include "IATHook.h"

PROC IATHook(HMODULE ModuleHandle, LPCSTR DLLName, LPCSTR ProcName, PROC Callback) //成功返回原函数，失败返回NULL
{
	DWORD ProcAddress = (DWORD)GetProcAddress(GetModuleHandle(DLLName), ProcName);
	if (!ProcAddress)
	{
		return NULL;
	}
	PBYTE PEAddress = (PBYTE)ModuleHandle + ((PIMAGE_DOS_HEADER)ModuleHandle)->e_lfanew;
	DWORD dwRVA = *((DWORD*)& PEAddress[0x80]);
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pFirstThunk;
	PIMAGE_THUNK_DATA pOriginalFirstThunk;
	DWORD dwOldProtect;
	PIMAGE_IMPORT_BY_NAME pProcName;
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)ModuleHandle + dwRVA);
	for (; pImportDesc->Name; pImportDesc++)
	{
		
		if (!_stricmp((LPCSTR)((DWORD)ModuleHandle + pImportDesc->Name), DLLName))
		{
			pOriginalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD)ModuleHandle + pImportDesc->OriginalFirstThunk);
			pFirstThunk = (PIMAGE_THUNK_DATA)((DWORD)ModuleHandle + pImportDesc->FirstThunk);
			
			while (pOriginalFirstThunk->u1.Function)
			{
				if (pOriginalFirstThunk->u1.Ordinal < 0)
				{
					//按序号

				}
				else
				{
					//按名称
					pProcName = (PIMAGE_IMPORT_BY_NAME)((DWORD)ModuleHandle + pOriginalFirstThunk->u1.AddressOfData);
					if (!strcmp((LPCSTR)pProcName->Name, ProcName))
					{
						PROC OldFunction = (PROC)pFirstThunk->u1.Function;
						VirtualProtect((LPVOID)& pFirstThunk->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
						pFirstThunk->u1.Function = (DWORD)Callback;
						VirtualProtect((LPVOID)& pFirstThunk->u1.Function, 4, dwOldProtect, &dwOldProtect);
						return OldFunction;
					}
				}
				pOriginalFirstThunk++;
				pFirstThunk++;
			}

		}

	}
	return NULL;
}