#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "UART_1_AVR128DA64.h"
#include <string.h>

static void SPI0_init(void);
char receiveData=0; 

static void SPI0_init(void)
{
	PORTA.DIR &= ~PIN4_bm; 
	PORTA.DIR |= PIN5_bm; 
	PORTA.DIR &= ~PIN6_bm; 
	PORTA.DIR &= ~PIN7_bm; 

	SPI0.CTRLB |= (1 << 7);  

	SPI0.CTRLA = SPI_ENABLE_bm; 
	SPI0_CTRLA &= ~SPI_MASTER_bm;
	SPI0.INTCTRL = SPI_RXCIE_bm; 
}

ISR(SPI0_INT_vect)
{
	SPI0.INTFLAGS = (1 << 7); 
	receiveData = SPI0.DATA;
	USART1_sendChar(receiveData);
}                     
int main(void)
{
	USART1_init(9600);
	SPI0_init();
	sei(); 
	while (1)
	{
		_delay_ms(10);
	}
}
