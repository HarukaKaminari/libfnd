#include "SPI.h"


#ifndef USE_HARDWARE_SPI
// 软件SPI的管脚定义
sbit SPI_MOSI	= P0^0;
sbit SPI_MISO	= P0^1;
sbit SPI_CLK	= P0^2;
#endif
// 每个SPI设备的/CS要在各自的.h中单独定义


void SPI_Init(){
#ifdef USE_HARDWARE_SPI
#ifdef HARDWARE_SPI_AT_P4
	AUXR1 |= 0x20;	// 将硬件SPI设置到P4口上
	P4M0 &= 0xF1;
	P4M1 &= 0xF1;	// 将SPI_CLK（P4.3）、SPI_MOSI（P4.1）和SPI_MISO（P4.2）设置成准双向
#else
	AUXR1 &= 0xDF;	// 将硬件SPI设置到P1口上
	P1M0 &= 0x1F;
	P1M1 &= 0x1F;	// 将SPI_CLK（P1.7）、SPI_MOSI（P1.5）和SPI_MISO（P1.6）设置成准双向
#endif
	SPDAT = 0;		// 初始化SPI缓冲区
	SPSTAT = 0xC0;	// 初始化SPI状态
	SPCTL = 0xDC;	// 开启硬件SPI，主机模式，高位先发，下降沿发送上升沿接收，CLK频率为1/4主频
#else
	P0M0 &= 0xF8;
	P0M1 &= 0xF8;	// 将SPI_CLK、SPI_MOSI和SPI_MISO设置成准双向
#endif
}

u8 SPI_SendAndReceive(u8 value){
#ifdef USE_HARDWARE_SPI
	SPDAT = value;				// 填充缓冲区
	while(!(SPSTAT & 0x80));	// 等待传输完成
	SPSTAT = 0xC0;				// 清标志位
	return SPDAT;				// 返回从机发送过来的数据
#else
	u8 i;
	for(i=0;i<8;++i){
		SPI_CLK = 0;
		SPI_MOSI = value & 0x80;
		SPI_CLK = 1;
		value <<= 1;
		value |= SPI_MISO;
	}
	return value;
#endif
}
