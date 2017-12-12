#include "../libfnd/Timer.h"


static void Timer_Callback(){
	P0 = ~P0;
}

static void Init(){
	P0M0 = 0;
	P0M1 = 0;
	P1M0 = 0;
	P1M1 = 0;
	P2M0 = 0;
	P2M1 = 0;
	P3M0 = 0;
	P3M1 = 0;
	P4M0 = 0;
	P4M1 = 0;
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	P4 = 0xFF;
}

void main(){
	Init();
	Timer_Init();
	Timer_SetCallback(Timer_Callback);
	Timer_Start();
	
	while(1){
		
	}
}
