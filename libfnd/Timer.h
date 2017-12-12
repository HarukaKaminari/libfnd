#ifndef TIMER_H_
#define TIMER_H_


#include <STC12C5A60S2.h>
#include "typedef.h"


typedef void (*Func_TimerCallback)(void);


// Ö÷ÆµÎª30MHz
void Timer_Init();
void Timer_SetCallback(Func_TimerCallback callback);
void Timer_Start();
void Timer_Stop();


#endif	/* TIMER_H_ */
