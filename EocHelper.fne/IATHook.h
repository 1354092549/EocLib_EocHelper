#pragma once
#include "stdafx.h"

PROC IATHook(HMODULE ModuleHandle, LPCSTR DLLName, LPCSTR ProcName, PROC Callback);
