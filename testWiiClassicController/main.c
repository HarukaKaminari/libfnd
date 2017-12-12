#include "../libfnd/typedef.h"
#include "../libfnd/Uart.h"
#include "../libfnd/I2C.h"
#include "../libfnd/WiiClassicController.h"


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
	Uart_Init();
	I2C_Init();
	WiiClassicController_Init();
	//CheckI2CSlaveValidity();
	while(1){
		WiiClassicController_Read();
		
	}
}
