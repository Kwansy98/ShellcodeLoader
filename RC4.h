#pragma once

void rc4_init(unsigned char *s, unsigned char *key, unsigned long Len);
void rc4_crypt(unsigned char *s, unsigned char *Data, unsigned long Len);
void Rc4Encrypt(void *pData, size_t byteDataLen, const void *pKey, const size_t byteKeyLen);
void Rc4Decrypt(void *pData, size_t byteDataLen, const void *pKey, const size_t byteKeyLen);