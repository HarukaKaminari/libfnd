#include <string.h>
#include "LCM12864.h"
#if defined LCM_DRIVER_UC1701X
#include "UC1701X.h"
#elif defined LCM_DRIVER_IST3932
#include "IST3932.h"
#elif defined LCM_DRIVER_SSD1306
#include "SSD1306.h"
#endif
#include "SPI.h"
//#include "WinbondFlash.h"
#include "FontText.h"


#ifndef ABS
#define ABS(a)	((a) > 0 ? (a) : -(a))
#endif


#ifdef USE_12832
u8 xdata VRAM[512];		// 12832的显存为512字节
#else
u8 xdata VRAM[1024];	// 12864的显存为1K字节
#endif
u16 xdata tmpBuf[0x10];	// 16*2的字模缓冲区


void LCM12864_SendCommand(u8 value){
	LCM12864_CS = 0;
	LCM12864_CD = 0;
	SPI_SendAndReceive(value);
}

void LCM12864_SendData(u8 value){
	LCM12864_CS = 0;
	LCM12864_CD = 1;
	SPI_SendAndReceive(value);
}

void LCM12864_Init(){
	P0M0 &= 0xCF;
	P0M1 &= 0xCF;	// 设置/CS和CD为准双向
	P0M0 |= 0x20;
	P0M1 &= 0xDF;	// 设置LED为强推挽
	
#if defined LCM_DRIVER_UC1701X
	UC1701X_Init();
#elif defined LCM_DRIVER_IST3932
	IST3932_Init();
#elif defined LCM_DRIVER_SSD1306
	SSD1306_Init();
#endif
}

void LCM12864_Clear(){
	memset(VRAM, 0, sizeof(VRAM));
}

void LCM12864_Refresh(){
#if defined LCM_DRIVER_UC1701X
	UC1701X_Refresh();
#elif defined LCM_DRIVER_IST3932
	IST3932_Refresh();
#elif defined LCM_DRIVER_SSD1306
	SSD1306_Refresh();
#endif
}

/*
打开/关闭背光
*/
void LCM12864_Backlight(bit isOn){
	LCM12864_LED = isOn;
}

/*
从字符获得32位地址偏移量
*/
static u32 LCM12864_GetOffsetFromChar(u8 fontSize, u16 chr){
	//u8 hi = chr >> 8;
	//u8 lo = chr & 0xFF;
	//u32 offset = ((lo - 0x40) + (0xFE - 0x40 + 1) * (hi - 0x81)) * 2 * fontSize;
	//return offset;
	u8 i = 0;
	for(;i<CHARACTER_COUNT;++i){
		if(textMap[i] == chr)
			break;
	}
	if(i >= CHARACTER_COUNT)
		i = 0;

	return i * 2 * fontSize;
}

static void WinbondFlash_Read(u32 offset, u8* buf, u8 size){
	memcpy(buf, fontText + offset, size);
}

void LCM12864_DrawChar(u8 fontSize, u16 chr, s8 x, s8 y, bit c){
	WinbondFlash_Read(LCM12864_GetOffsetFromChar(fontSize, chr), (u8*)tmpBuf, fontSize * sizeof(u16));
#if defined LCM_DRIVER_UC1701X
	UC1701X_DrawChar(fontSize, x, y, c);
#elif defined LCM_DRIVER_IST3932
	IST3932_DrawChar(fontSize, x, y, c);
#elif defined LCM_DRIVER_SSD1306
	SSD1366_DrawChar(fontSize, x, y, c);
#endif
}

void LCM12864_DrawString(u8 fontSize, const char* str, s16 x, s16 y, bit c){
	u8 i = 0, len = strlen(str);
	while(i < len){
		if((u8)str[i] < 0x81){
            if(x >= -fontSize && x <= 127)
                LCM12864_DrawChar(fontSize, str[i], x, y, c);
			i++;
			x += (fontSize >> 1);
		}else{
			u16 tmp = str[i];
			tmp <<= 8;
			tmp |= (u8)str[i + 1];
            if(x >= -fontSize && x <= 127)
                LCM12864_DrawChar(fontSize, tmp, x, y, c);
			i += 2;
			x += fontSize;
		}
	}
}

/*
在缓冲区任意位置画点
*/
void LCM12864_DrawPixel(u8 x, u8 y, bit c){
#if defined LCM_DRIVER_UC1701X
	UC1701X_DrawPixel(x, y, c);
#elif defined LCM_DRIVER_IST3932
	IST3932_DrawPixel(x, y, c);
#elif defined LCM_DRIVER_SSD1306
	SSD1306_DrawPixel(x, y, c);
#endif
}

/*
在缓冲区任意位置画直线
*/
void LCM12864_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, bit c){
	s8 dx = x1 - x0;
	s8 dy = y1 - y0;
	s8 ux = ((dx > 0) << 1) - 1;	// x的增量方向，正负1
	s8 uy = ((dy > 0) << 1) - 1;	// y的增量方向，正负1
	s8 x = x0, y = y0, eps = 0;		// eps为累加误差

	dx = ABS(dx);
	dy = ABS(dy); 
	if(dx > dy){
		for(;x!=x1;x+=ux){
			LCM12864_DrawPixel(x, y, c);
			eps += dy;
			if((eps << 1) >= dx){
				y += uy;
				eps -= dx;
			}
		}
	}else{
		for(;y!=y1;y+=uy){
			LCM12864_DrawPixel(x, y, c);
			eps += dx;
			if((eps << 1) >= dy){
				x += ux;
				eps -= dy;
			}
		}
	}
}

/*
在缓冲区任意位置填充矩形
*/
void LCM12864_FillRect(u8 x, u8 y, u8 w, u8 h, bit c){
#if defined LCM_DRIVER_UC1701X
	UC1701X_FillRect(x, y, w, h, c);
#elif defined LCM_DRIVER_IST3932
	IST3932_FillRect(x, y, w, h, c);
#elif defined LCM_DRIVER_SSD1306
	SSD1306_FillRect(x, y, w, h, c);
#endif
}

/*
在缓冲区任意位置画圆
*/
void LCM12864_DrawCircle(u8 x1, u8 y1, u8 r, bit c){
	s16 x = 0, y = r;
	s8 d = 1 - r;
	while(y > x){
		LCM12864_DrawPixel(x1 + x, y1 + y, c);
		LCM12864_DrawPixel(x1 + y, y1 + x, c);
		LCM12864_DrawPixel(x1 - x, y1 + y, c);
		LCM12864_DrawPixel(x1 - y, y1 + x, c);
		LCM12864_DrawPixel(x1 - x, y1 - y, c);
		LCM12864_DrawPixel(x1 - y, y1 - x, c);
		LCM12864_DrawPixel(x1 + x, y1 - y, c);
		LCM12864_DrawPixel(x1 + y, y1 - x, c);
		if(d < 0){
			d += 2 * x + 3;
		}else{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}
