#include "stdafx.h"

DWORD _Wow64Transition = 0;

// WOW64系统调用入口
__declspec(naked) void Wow64SystemServiceCall()
{
	__asm 
	{
		jmp ds:_Wow64Transition;
	}
}

// 初始化WOW64系统调用入口
void InitWow64Transition()
{
	__asm
	{
		mov edx, fs:[0x18];
		mov edx,[edx + 0xC0];
		mov _Wow64Transition, edx;
	}
}

//// 重写的 NtAllocateVirtualMemory
//__declspec(naked) NTSTATUS WINAPI NtAllocateVirtualMemory(
//	HANDLE    ProcessHandle,
//	PVOID     *BaseAddress,
//	ULONG_PTR ZeroBits,
//	PSIZE_T   RegionSize,
//	ULONG     AllocationType,
//	ULONG     Protect
//	)
//{
//	__asm
//	{
//		//mov eax, 18h;
//		//mov edx, Wow64SystemServiceCall;
//		//call edx;
//		push 18h;
//		pop eax;
//		call Wow64SystemServiceCall;
//		retn 18h;
//	}
//}

// 重写的 VirtualAlloc
LPVOID WINAPI MyVirtualAlloc(
	LPVOID lpAddress,
	SIZE_T dwSize,
	DWORD  flAllocationType,
	DWORD  flProtect
	)
{
	ULONG_PTR RegionSize = dwSize;
	LPVOID BaseAddress = lpAddress;
	NTSTATUS status;
	//status = NtAllocateVirtualMemory((HANDLE)0xFFFFFFFF, &BaseAddress, 0, &RegionSize, (flAllocationType & 0xFFFFFFC0), flProtect);
	__asm
	{
		push flProtect;
		mov eax,0xFFFFFFC0;
		and eax,flAllocationType;
		push eax;
		lea eax,RegionSize;
		push eax;
		push 0;
		lea ebx, BaseAddress;
		push ebx;
		push 0xFFFFFFFF;		
		push returnaddr;
		push 18h;
		pop eax;
		push returnaddr2;
		jmp _Wow64Transition;
returnaddr2:
		retn 18h;
returnaddr:
		mov status,eax;
	}
	if (status >= 0)
	{
		return BaseAddress;
	}
	return NULL;
}

//__declspec(naked) NTSTATUS WINAPI NtProtectVirtualMemory(HANDLE,LPVOID*,SIZE_T*,DWORD,PDWORD)
//{
//	__asm
//	{
//		//mov eax, 50h;
//		//mov edx, Wow64SystemServiceCall;
//		//call edx;
//		push 50h;
//		pop eax;
//		call Wow64SystemServiceCall;
//		retn 14h;
//	}
//}

BOOL WINAPI MyVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect)
{
	SIZE_T NumberOfBytesToProtect = dwSize;
	LPVOID BaseAddress = lpAddress;
	NTSTATUS status;
	//status = NtProtectVirtualMemory((HANDLE)0xFFFFFFFF, &BaseAddress, &NumberOfBytesToProtect, flNewProtect, lpflOldProtect);
	__asm
	{
		push lpflOldProtect;
		push flNewProtect;
		lea eax,NumberOfBytesToProtect;
		push eax;
		lea ebx, BaseAddress;
		push ebx;
		push 0xFFFFFFFF;
		//call NtProtectVirtualMemory;
		push returnaddr;
		push 50h;
		pop eax;
		push returnaddr2;
		jmp _Wow64Transition;
		returnaddr2:
		retn 14h;
		returnaddr:
		mov status,eax;
	}
	if (status >= 0)
		return 1;
	return 0;
}