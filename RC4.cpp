#include "stdafx.h"
#include "RC4.h"



void rc4_init(unsigned char *s, unsigned char *key, unsigned long Len)
{
	int i = 0, j = 0;
	unsigned char k[256] = { 0 };
	unsigned char tmp = 0;
	for (i = 0; i < 256; i++) {
		s[i] = i;
		k[i] = key[i % Len];
	}
	for (i = 0; i < 256; i++) {
		j = (j + s[i] + k[i]) % 256;
		tmp = s[i];
		s[i] = s[j]; //����s[i]��s[j]
		s[j] = tmp;
	}
}

void rc4_crypt(unsigned char *s, unsigned char *Data, unsigned long Len)
{
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;
	for (k = 0; k < Len; k++) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j]; //����s[x]��s[y]
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		Data[k] ^= s[t];
	}
}

void Rc4Encrypt(void *pData, size_t byteDataLen, const void *pKey, const size_t byteKeyLen)
{
	unsigned char s[256] = { 0 }; //S-box
	rc4_init(s, (unsigned char *)pKey, byteKeyLen); //��ʼ����Կ
	rc4_crypt(s, (unsigned char *)pData, byteDataLen);//����
}

void Rc4Decrypt(void *pData, size_t byteDataLen, const void *pKey, const size_t byteKeyLen)
{
	unsigned char s[256] = { 0 }; //S-box

	rc4_init(s, (unsigned char *)pKey, byteKeyLen); //�Ѿ�����˳�ʼ��
	rc4_crypt(s, (unsigned char *)pData, byteDataLen);//����	
}
