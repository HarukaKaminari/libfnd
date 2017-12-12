#ifndef SPI_H_
#define SPI_H_


#include "typedef.h"
#include "STC12C5A60S2.h"
#include "Utility.h"


void SPI_Init();
u8 SPI_SendAndReceive(u8 value);


#endif	/* SPI_H_ */
