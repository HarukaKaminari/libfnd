#ifdef LCM_DRIVER_IST3932


#define USE_TABLE


#include "IST3932.h"
#include "LCM12864.h"
#include "SPI.h"


// �Դ滺�������ݸ�ʽ
//		0		1		2		.	F
//   +-------+-------+-------+---+-------+
// 00|Byte000|Byte001|Byte002|...|Byte00F|
//   +-------+-------+-------+---+-------+
// 01|Byte010|Byte011|Byte012|...|Byte01F|
//   +-------+-------+-------+---+-------+
// ..|.......|.......|.......|...|.......|
//   +-------+-------+-------+---+-------+
// 1F|Byte1F0|Byte1F1|Byte1F2|...|Byte1FF|
//   +-------+-------+-------+---+-------+ <--- 12832����Ϊֹ
// ..|.......|.......|.......|...|.......|
//   +-------+-------+-------+---+-------+
// 3F|Byte3F0|Byte3F1|Byte3F2|...|Byte3FF|
//   +-------+-------+-------+---+-------+ <--- 12864����Ϊֹ


// ����ͷβ�ߵ��ı��
#ifdef USE_TABLE
static u8 code reverse[] = {0x0, 0x8, 0x7, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0x3, 0xB, 0x7, 0xF};
#endif


/*
�����е�ַ����Ȼrowȡֵ��Χֻ��Ϊ0~31��63
*/
static void IST3932_SetRow(u8 row){
	LCM12864_SendCommand(row & 0xF);
	LCM12864_SendCommand(0x10 | (row >> 4));
}

/*
��Ļ�ϵ�
*/
static void IST3932_Power(bit isOn){
	if(isOn == 0){
		LCM12864_SendCommand(0x2D);
	}else{
		LCM12864_SendCommand(0x2F);
	}
}

/*
����ƫѹ�ȡ�ȡֵ��ΧΪ5��1��0��2��3��4��ò�ƺͶԱȶ��йء���
*/
static void IST3932_SetBias(u8 bias){
	LCM12864_SendCommand(0x30 | (bias & 0x7));
}

/*
˯��ģʽ
*/
static void IST3932_Sleep(bit isSleep){
	if(isSleep == 0)
		LCM12864_SendCommand(0x38);
	else
		LCM12864_SendCommand(0x39);
}

/*
δ֪
*/
static void IST3932_Osc(bit isOff){
	if(isOff == 0){
		LCM12864_SendCommand(0x3A);
	}else{
		LCM12864_SendCommand(0x3B);
	}
}

/*
��/����ʾ
*/
static void IST3932_Display(bit isOn){
	if(isOn == 0){
		LCM12864_SendCommand(0x3C);
	}else{
		LCM12864_SendCommand(0x3D);
	}
}

/*
������ʼ��ַ��ȡֵ��ΧΪ0~64��ò�����������ô�ֱ����ƫ������
*/
static void IST3932_SetStartingLine(u8 line){
	LCM12864_SendCommand(0x40 | (line & 0xF));
	LCM12864_SendCommand(0x50 | (line >> 4));
}

/*
����������ʾ����һ�����������Ƿ�Ӳ��ȫ����ֱ���񣬵ڶ���������Ӳ��ȫ��ˮƽ�����йأ���������map����ᵼ�´��ң���ñ���Ϊ0�������������������Ƿ�ȫ�������ĸ����������Ƿ���
*/
static void IST3932_DriverControl(bit isVFlip, bit isADC, bit isDisplayAll, bit isNegative){
	u8 cmd = 0x60;
	if(isVFlip)cmd |= (1 << 3);
	if(isADC)cmd |= (1 << 2);
	if(isDisplayAll)cmd |= (1 << 1);
	if(isNegative)cmd |= (1 << 0);
	LCM12864_SendCommand(cmd);
}

/*
��λ
*/
static void IST3932_SoftReset(){
	LCM12864_SendCommand(0x76);
}

/*
����ռ�ձȡ�ȡֵ��ΧΪ1~65��ò�ƺͶԱȶ��й�
*/
static void IST3932_SetDuty(u8 duty){
	LCM12864_SendCommand(0x90 | (duty & 0xF));
	LCM12864_SendCommand(0xA0 | (duty >> 4));
}

/*
�����е�ַ��colȡֵ��ΧΪ0~31
*/
static void IST3932_SetCol(u8 col){
	LCM12864_SendCommand(0xC0 | (col & 0x1F));
}

/*
���öԱȶȡ�ȡֵ��ΧΪ0~255
*/
static void IST3932_SetContrast(u8 contrast){
	LCM12864_SendCommand(0xB1);
	LCM12864_SendCommand(contrast);
}

/*
δ֪
*/
static void IST3932_SetFrameControl(u8 value0, u8 value1){
	LCM12864_SendCommand(0xB2);
	LCM12864_SendCommand(value0);
	LCM12864_SendCommand(value1);
}

/*
��ָ�ͬʱҲ���˳����ģʽ��ISTģʽ��ָ��������Ҳ��������ñ��ģʽ����
*/
static void IST3932_Nop(){
	LCM12864_SendCommand(0xE3);
}

/*
����ISTģʽ
*/
static void IST3932_BeginIST(){
	LCM12864_SendCommand(0x88);
	LCM12864_SendCommand(0x88);
	LCM12864_SendCommand(0x88);
	LCM12864_SendCommand(0x88);
}

/*
�����н���ģʽ
*/
static void IST3932_SetInterleave(bit isInterleave){
	if(isInterleave == 0){
		LCM12864_SendCommand(0x60);
	}else{
		LCM12864_SendCommand(0x61);
	}
}

/*
�˳�ISTģʽ����Ч��NOP
*/
static void IST3932_EndIST(){
	LCM12864_SendCommand(0xE3);
}

void IST3932_Init(){
	LCM12864_CS = 0;

	// ������ʱ100ms���Զ�Ӳ����λ���
	Delay(100);
	// Set Power
	IST3932_Power(1);
	// Set Bias
	IST3932_SetBias(3);
	// Set Contrast
	IST3932_SetContrast(0xDF);
	// Set FrameControl
	IST3932_SetFrameControl(0x90, 0x01);
	// Set Duty
	IST3932_SetDuty(0x40);
	// Set Driver Display Control
	IST3932_DriverControl(0, 0, 0, 0);
	// Set Display ON
	IST3932_Display(1);
	// Set Interleave
	IST3932_BeginIST();
	IST3932_SetInterleave(0);
	IST3932_EndIST();

	LCM12864_CS = 1;
}

void IST3932_Refresh(){
	u8 x, y;
	LCM12864_CS = 0;
#ifdef USE_12832
	for(y=0;y<32;++y){
#else
	for(y=0;y<64;++y){
#endif
		IST3932_SetRow(y);
		IST3932_SetCol(0);
		for(x=0;x<16;++x){
			u8 d = VRAM[x + (y << 4)];
#ifdef USE_TABLE
			d = (reverse[d & 0xF] << 4) | (reverse[d >> 4]);
#else
			d = ((d >> 1) & 0x55) | ((d & 0x55) << 1);
			d = ((d >> 2) & 0x33) | ((d & 0x33) << 2);
			d = ((d >> 4)		) | ((d		  ) << 4);
#endif			
			LCM12864_SendData(d);
		}
	}
	LCM12864_CS = 1;
}

/*
���ַ��������е��ַ����Ƶ�VRAM������
x��СȡֵΪ-fontSize�����ȡֵΪ127
y��СȡֵΪ-fontSize�����ȡֵΪ31��63
*/
void IST3932_DrawChar(u8 fontSize, s8 x, s8 y, bit c){
	u8 i;
	s8 page;
    if(x < -fontSize)x = -fontSize;
    if(x > 127)return;
    if(y < -fontSize)y = -fontSize;
#ifdef USE_12832
	if(y > 31)return;
#else
	if(y > 63)return;
#endif
	page = x >> 3;
	
	for(i=0;i<fontSize;++i){
		// �����ֱ����С��0�������ٽ��л���
		if(y + i < 0)continue;
		// �����ֱ���򳬹�32��64�������ٽ��л���
#ifdef USE_12832
		if(y + i < 32){
#else
		if(y + i < 64){
#endif
			// ���VRAMƫ����
			s16 vramOffset = ((y + i) << 4) + page;
			// ���ҳ��ƫ����
			u8 offsetInPage = x & 7;
			// ���page<0���һ�λ��ƺ���
			if(page >= 0){
				if(vramOffset >= 0){
					// д����
					if(c)
						VRAM[vramOffset] |= (tmpBuf[i] << offsetInPage) & 0xFF;
					else
						VRAM[vramOffset] &= ~(tmpBuf[i] << offsetInPage) & 0xFF;
				}
			}
			// ƫ�Ƶ���һ��ҳ
			vramOffset++;
			// ���ˮƽ���򳬹�128�������ٽ��л���
			if(vramOffset >= ((y + i + 1) << 4))continue;
			if(vramOffset >= 0){
				if(c)
					VRAM[vramOffset] |= (tmpBuf[i] >> (8 - offsetInPage)) & 0xFF;
				else
					VRAM[vramOffset] &= ~(tmpBuf[i] >> (8 - offsetInPage)) & 0xFF;
			}
			vramOffset++;
			// ���ˮƽ���򳬹�128�������ٽ��л���
			if(vramOffset >= ((y + i + 1) << 4))continue;
			if(vramOffset >= 0){
				if(c)
					VRAM[vramOffset] |= (tmpBuf[i] >> (16 - offsetInPage)) & 0xFF;
				else
					VRAM[vramOffset] &= ~(tmpBuf[i] >> (16 - offsetInPage)) & 0xFF;
			}
		}
	}
}

/*
�ڻ���������λ�û���
*/
void IST3932_DrawPixel(u8 x, u8 y, bit c){
	if(x > 127)return;
#ifdef USE_12832
	if(y > 31)return;
#else
	if(y > 63)return;
#endif
	if(c)
		VRAM[(x >> 3) + (y << 4)] |= 1 << (x & 7);
	else
		VRAM[(x >> 3) + (y << 4)] &= ~(1 << (x & 7));
}

/*
�ڻ���������λ��������
*/
void IST3932_FillRect(u8 x, u8 y, u8 w, u8 h, bit c){
	u8 i, j;
	if(x > 127)return;
	if(x + w > 128)w = 128 - x;
#ifdef USE_12832
	if(y > 31)return;
	if(y + h > 32)h = 32 - y;
#else
	if(y > 63)return;
	if(y + h > 64)h = 64 - y;
#endif
	
	for(i=y;i<=y+h-1;++i){
		u8 page0 = x >> 3;
		u8 page1 = (x + w) >> 3;
		u8 _x0 = x & 7;
		u8 _x1 = (x + w) & 7;
		if(page0 == page1){
			u8 mask = 0;
			for(j=_x0;j<=_x1;++j){
				mask |= (1 << j);
			}
			if(c){
				VRAM[page0 + (i << 4)] |= mask;
			}else{
				VRAM[page0 + (i << 4)] &= ~mask;
			}
		}else{
			for(j=page0;j<=page1;++j){
				if(j == page0){
					if(c)
						VRAM[j + (i << 4)] |= (0xFF << _x0);
					else
						VRAM[j + (i << 4)] &= ~(0xFF << _x0);
				}else if(j == page1){
					if(c)
						VRAM[j + (i << 4)] |= (0xFF >> (7 - _x1));
					else
						VRAM[j + (i << 4)] &= ~(0xFF >> (7 - _x1));
				}else{
					if(c)
						VRAM[j + (i << 4)] = 0xFF;
					else
						VRAM[j + (i << 4)] = 0;
				}
			}
		}
	}
}


#endif
