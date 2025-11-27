#ifndef _spi_H
#define _spi_H

#include "system.h"

void MY_SPI_Init(void);
void AT45DB161_CS_H(void);
void AT45DB161_CS_L(void);
u8 SPI_RW_Byte(u8 data);

#endif
