#ifndef LCM_12864_H_
#define LCM_12864_H_


#include "Utility.h"


sbit LCM12864_CS	= P0^0;	// Ƭѡ��/CS��
sbit LCM12864_CD	= P0^2;	// ָ��/���ݼĴ���ѡ��
sbit LCM12864_LED	= P0^7;	// ����


#ifdef USE_12832
extern u8 xdata VRAM[512];		// 12832���Դ�Ϊ512�ֽ�
#else
extern u8 xdata VRAM[1024];		// 12864���Դ�Ϊ1K�ֽ�
#endif
extern u16 xdata tmpBuf[0x10];	// 16*2����ģ������


void LCM12864_SendCommand(u8 value);
void LCM12864_SendData(u8 value);

void LCM12864_Init();
void LCM12864_Clear();
void LCM12864_Refresh();

void LCM12864_Backlight(bit isOn);

void LCM12864_DrawChar(u8 fontSize, u16 chr, s8 x, s8 y, bit c);
void LCM12864_DrawString(u8 fontSize, const char* str, s16 x, s16 y, bit c);
void LCM12864_DrawPixel(u8 x, u8 y, bit c);
void LCM12864_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, bit c);
void LCM12864_FillRect(u8 x, u8 y, u8 w, u8 h, bit c);
void LCM12864_DrawCircle(u8 x1, u8 y1, u8 r, bit c);


#endif	/* LCM_12864_H_ */
