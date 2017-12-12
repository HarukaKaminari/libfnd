#include "SPI.h"


#ifndef USE_HARDWARE_SPI
// ���SPI�ĹܽŶ���
sbit SPI_MOSI	= P0^0;
sbit SPI_MISO	= P0^1;
sbit SPI_CLK	= P0^2;
#endif
// ÿ��SPI�豸��/CSҪ�ڸ��Ե�.h�е�������


void SPI_Init(){
#ifdef USE_HARDWARE_SPI
#ifdef HARDWARE_SPI_AT_P4
	AUXR1 |= 0x20;	// ��Ӳ��SPI���õ�P4����
	P4M0 &= 0xF1;
	P4M1 &= 0xF1;	// ��SPI_CLK��P4.3����SPI_MOSI��P4.1����SPI_MISO��P4.2�����ó�׼˫��
#else
	AUXR1 &= 0xDF;	// ��Ӳ��SPI���õ�P1����
	P1M0 &= 0x1F;
	P1M1 &= 0x1F;	// ��SPI_CLK��P1.7����SPI_MOSI��P1.5����SPI_MISO��P1.6�����ó�׼˫��
#endif
	SPDAT = 0;		// ��ʼ��SPI������
	SPSTAT = 0xC0;	// ��ʼ��SPI״̬
	SPCTL = 0xDC;	// ����Ӳ��SPI������ģʽ����λ�ȷ����½��ط��������ؽ��գ�CLKƵ��Ϊ1/4��Ƶ
#else
	P0M0 &= 0xF8;
	P0M1 &= 0xF8;	// ��SPI_CLK��SPI_MOSI��SPI_MISO���ó�׼˫��
#endif
}

u8 SPI_SendAndReceive(u8 value){
#ifdef USE_HARDWARE_SPI
	SPDAT = value;				// ��仺����
	while(!(SPSTAT & 0x80));	// �ȴ��������
	SPSTAT = 0xC0;				// ���־λ
	return SPDAT;				// ���شӻ����͹���������
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
