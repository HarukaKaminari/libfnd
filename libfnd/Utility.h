#ifndef UTILITY_H_
#define UTILITY_H_


#include "STC12C5A60S2.h"
#include "typedef.h"


// ��ƵΪ11.0592MHz�������ʱ
void Delay(u16 ms);

u8 Dec2String(u8 dec, u8* strBuf);
u8 Hex2String(u8 hex, u8* strBuf);


#endif	/* UTILITY_H_ */
