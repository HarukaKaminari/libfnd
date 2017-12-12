#include <STRING.H>
#include "Uart.h"


void Uart_Init(){
	PCON &= 0x7F;	// 波特率不倍速
	SCON = 0x50;	// 8位数据，可变波特率
	AUXR |= 0x40;	// 定时器1时钟为Fosc，即1T
	AUXR &= 0xFE;	// 串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;	// 清除定时器1模式位
	TMOD |= 0x20;	// 设定定时器1为8位自动重装方式
	TL1 = 0xFD;		// 设定定时初值（波特率115200）
	TH1 = 0xFD;		// 设定定时器重装值
	ET1 = 0;		// 禁止定时器1中断
	TR1 = 1;		// 启动定时器1
	TI = 1;			// 这个位是由硬件自动置位的，代表串行中断请求。不明白为什么在这里手动置位……
}

void Uart_Put(u8 value){
	SBUF = value;	// 向串口送出数据
	while(TI == 0);	// 等待串行中断请求
	TI = 0;			// 手动清除串行中断请求
}

bit Uart_Get(u8* value){
	if(RI == 1){		// 如果检测到串行中断请求
		*value = SBUF;	// 从串口接收数据
		RI = 0;			// 手动清除串行中断请求
		return 1;
	}
	return 0;
}

//void Uart_Log(const char* fmt, ...){
//	va_list args;
//	va_start(args, fmt);
//	TI = 1;
//	vprintf(fmt, args);
//	va_end(args);
//}

void Uart_String(const char* str){
	u8 i = 0, len = strlen(str);
	for(;i<len;++i)Uart_Put(str[i]);
}

void Uart_Dec(u8 val){
	u8 i = 0;
	u8 strBuf[4];
	u8 len = Dec2String(val, strBuf);
	for(;i<len;++i){
		Uart_Put(strBuf[i]);
	}
}

void Uart_Hex(u8 val){
	u8 i = 0;
	u8 strBuf[3];
	u8 len = Hex2String(val, strBuf);
	for(;i<len;++i){
		Uart_Put(strBuf[i]);
	}
}

