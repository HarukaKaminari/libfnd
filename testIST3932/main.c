#include "../libfnd/STC12C5A60S2.h"
#include "../libfnd/SPI.h"
#include "../libfnd/LCM12864.h"
#include "../libfnd/Utility.h"


static void Init(){
	P4SW |= 0x70;		// ����P4.4��P4.5��P4.6��ΪGPIO
	P0M0 = 0;P0M1 = 0;
	P1M0 = 0;P1M1 = 0;
	P2M0 = 0;P2M1 = 0;
	P3M0 = 0;P3M1 = 0;
	P4M0 = 0;P4M1 = 0;	// ��������P�ھ�Ϊ������
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	P4 = 0xFF;			// ����P�ھ�����ߵ�ƽ
}

void main(){
	Init();
	SPI_Init();
	LCM12864_Init();
	
	LCM12864_Backlight(1);
	while(1){
		LCM12864_Clear();
		//LCM12864_FillRect(0, 0, 8, 8, 1);
		LCM12864_DrawPixel(0, 0, 1);
		LCM12864_Refresh();
	}
}
