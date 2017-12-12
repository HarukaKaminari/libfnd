#ifndef UART_H_
#define UART_H_


#include "STC12C5A60S2.h"
#include "typedef.h"
#include "Utility.h"


// 主频11.0592MHz，波特率115200，硬件串口1，TxD=P3.1，RxD=P3.0
void Uart_Init();

void Uart_Put(u8 value);
bit Uart_Get(u8* value);
void Uart_String(const char* str);
void Uart_Dec(u8 val);
void Uart_Hex(u8 val);


#endif	/* UART_H_ */
