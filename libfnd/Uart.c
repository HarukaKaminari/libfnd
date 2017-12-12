#include <STRING.H>
#include "Uart.h"


void Uart_Init(){
	PCON &= 0x7F;	// �����ʲ�����
	SCON = 0x50;	// 8λ���ݣ��ɱ䲨����
	AUXR |= 0x40;	// ��ʱ��1ʱ��ΪFosc����1T
	AUXR &= 0xFE;	// ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;	// �����ʱ��1ģʽλ
	TMOD |= 0x20;	// �趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		// �趨��ʱ��ֵ��������115200��
	TH1 = 0xFD;		// �趨��ʱ����װֵ
	ET1 = 0;		// ��ֹ��ʱ��1�ж�
	TR1 = 1;		// ������ʱ��1
	TI = 1;			// ���λ����Ӳ���Զ���λ�ģ��������ж����󡣲�����Ϊʲô�������ֶ���λ����
}

void Uart_Put(u8 value){
	SBUF = value;	// �򴮿��ͳ�����
	while(TI == 0);	// �ȴ������ж�����
	TI = 0;			// �ֶ���������ж�����
}

bit Uart_Get(u8* value){
	if(RI == 1){		// �����⵽�����ж�����
		*value = SBUF;	// �Ӵ��ڽ�������
		RI = 0;			// �ֶ���������ж�����
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

