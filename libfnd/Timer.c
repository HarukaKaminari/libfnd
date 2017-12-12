#include "Timer.h"


static Func_TimerCallback timerCallback = 0;


static void Timer0_Isr() interrupt 1 using 1{
	if(timerCallback){
		timerCallback();
	}
}

void Timer_Init(){
	AUXR |= 0x80;		// ��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;
	TMOD |= 0x02;		// ���ö�ʱ��ģʽ
	TL0 = 0xE2;			// ���ö�ʱ��ֵ
	TH0 = 0xE2;			// ���ö�ʱ����ֵ
	TF0 = 0;			// ���TF0��־
	ET0 = 1;			// ����Timer0�ж�
	EA = 1;				// �����ж��ܿ���
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
