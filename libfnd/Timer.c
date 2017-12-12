#include "Timer.h"


static Func_TimerCallback timerCallback = 0;


static void Timer0_Isr() interrupt 1 using 1{
	if(timerCallback){
		timerCallback();
	}
}

void Timer_Init(){
	AUXR |= 0x80;		// 定时器时钟1T模式
	TMOD &= 0xF0;
	TMOD |= 0x02;		// 设置定时器模式
	TL0 = 0xE2;			// 设置定时初值
	TH0 = 0xE2;			// 设置定时重载值
	TF0 = 0;			// 清除TF0标志
	ET0 = 1;			// 允许Timer0中断
	EA = 1;				// 开启中断总开关
}

void Timer_SetCallback(Func_TimerCallback callback){
	timerCallback = callback;
}

void Timer_Start(){
	TR0 = 1;
}

void Timer_Stop(){
	TR0 = 0;
}
