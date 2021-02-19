#include "stdafx.h"

extern DWORD _Wow64Transition;

void InitWow64Transition();

LPVOID WINAPI MyVirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD  flAllocationType,DWORD  flProtect);

BOOL WINAPI MyVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);